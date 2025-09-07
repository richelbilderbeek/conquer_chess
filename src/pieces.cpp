#include "pieces.h"

#include "game_coordinate.h"
#include "fen_string.h"

// Ignore warnings on Windows, thanks https://stackoverflow.com/a/5850405/3364162
#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "../chess-library/include/chess.hpp"

// Ignore warnings on Windows, thanks https://stackoverflow.com/a/5850405/3364162
#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif // _MSC_VER

#include <algorithm>
#include <cassert>
#include <numeric>
#include <sstream>
#include <iterator>
#include <optional>
#include <regex>

#ifdef GAME_CONTROLLER_H
#error 'pieces' must know nothing about 'game_controller'
#error Instead, it is the other way around...
#endif

#ifdef LOBBY_OPTIONS_H
#error 'pieces' must know nothing about 'lobby_options'
#error Instead, all player sides (e.g. LHS) need to be converted to colors (e.g. white)
#endif

#ifdef SIDE_H
#error 'pieces' must know nothing about 'side'
#error This is managed by 'lobby_options'.
#endif


std::vector<std::string> add_coordinats(
  const std::vector<std::string>& strs
)
{
  assert(strs.size() == 8);
  assert(strs[0].size() == 8);
  // Step 1: add the files, strs -> strs_with_files
  std::vector<std::string> strs_with_files{strs};
  for (int i = 0; i!=8; ++i)
  {
    strs_with_files[i] =
      std::string(1, 'A' + static_cast<unsigned char>(i))
      + std::string("|")
      + strs_with_files[i]
      + std::string("|")
      + std::string(1, 'A' + static_cast<unsigned char>(i))
    ;
  }
  // Step 2: add the ranks, strs_with_files -> t
  std::vector<std::string> t;
  t.reserve(12);
  t.push_back("  12345678  ");
  t.push_back(" +--------+ ");
  std::copy(
    std::begin(strs_with_files),
    std::end(strs_with_files),
    std::back_insert_iterator(t)
  );
  t.push_back(" +--------+ ");
  t.push_back("  12345678  ");
  return t;
}

std::vector<std::string> add_legend(
  const std::vector<std::string>& strs
)
{
  assert(strs.size() >= 8);
  std::vector<std::string> text_with_legend{strs};
  text_with_legend[0] += " Type  |White|Black";
  text_with_legend[1] += " ------|-----|-----";
  text_with_legend[2] += " bishop| B   | b   ";
  text_with_legend[3] += " king  | K   | k   ";
  text_with_legend[4] += " knight| N   | n   ";
  text_with_legend[5] += " pawn  | P   | p   ";
  text_with_legend[6] += " queen | Q   | q   ";
  text_with_legend[7] += " rook  | R   | r   ";
  return text_with_legend;
}

std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinate& coordinat
) {
  std::vector<double> distances;
  distances.reserve(pieces.size());
  std::transform(
    std::begin(pieces),
    std::end(pieces),
    std::back_inserter(distances),
    [coordinat](const auto& piece) {
      return calc_distance(
        coordinat,
        to_coordinat(piece.get_current_square())
      );
    }
  );
  assert(distances.size() == pieces.size());
  return distances;
}

bool can_move(
  const std::vector<piece>& pieces,
  const chess_color player_color,
  const piece_type selected_piece_type,
  const square& selected_piece_square,
  const square& destination_square
)
{
  if (
    !can_move_on_empty_board(
      player_color,
      selected_piece_type,
      selected_piece_square,
      destination_square
    )
  )
  {
    return false;
  }
  if (is_piece_at(pieces, destination_square))
  {
    return false;
  }
  if (selected_piece_type == piece_type::knight)
  {
   return true;
  }
  // Inclusive
  auto squares{get_intermediate_squares(selected_piece_square, destination_square)};
  assert(squares.size() >= 2); // includes start and end
  if (squares.size() == 2) return true;

  // Only keep the middle
  squares.erase(squares.begin());
  squares.pop_back();
  assert(!squares.empty());
  for (const auto s: squares)
  {
    if (is_piece_at(pieces, s)) return false;
  }
  return true;
}

action_history collect_action_history(const std::vector<piece>& pieces)
{
  std::vector<action_history> histories;
  histories.reserve(pieces.size());
  std::transform(
    std::begin(pieces),
    std::end(pieces),
    std::back_inserter(histories),
    [](const auto& p)
    {
      return p.get_action_history();
    }
  );
  const auto history{merge_action_histories(histories)};
  return history;
}

int count_dead_pieces(
  const std::vector<piece>& pieces
)
{
  return std::count_if(
    std::begin(pieces),
    std::end(pieces),
    [](const auto& piece)
    {
      return is_dead(piece);
    }
  );
}

int count_piece_actions(
  const std::vector<piece>& pieces,
  const chess_color player
)
{
  return std::accumulate(
    std::begin(pieces),
    std::end(pieces),
    0,
    [player](const int n, const auto& piece)
    {
      if (piece.get_color() == player)
      {
        return n + count_piece_actions(piece);
      }
      return n;
    }
  );
}

std::vector<piece> create_pieces_from_fen_string(const fen_string& fen_str)
{
  const std::string s{fen_str.get()};
  assert(!s.empty());

  std::vector<piece> pieces;

  int square_index{0}; // a8, a7, a6, etc...
  int str_index{0};
  while(1)
  {
    const char c{s[str_index]};
    if (c >= '1' && c <= '8')
    {
      const int n_skips = static_cast<int>(c - '1');
      assert(n_skips >= 0 && n_skips <= 8);
      square_index += n_skips;
      ++str_index;
      ++square_index;
    }
    else if (c == '/')
    {
      // Move to the next
      ++str_index;
    }
    else if (c == ' ')
    {
      // Done!
      break;
    }
    else
    {
      const piece_type type{to_piece_type(std::toupper(c))};
      const chess_color color{std::isupper(c) ? chess_color::white : chess_color::black };
      const char file_char = 'a' + (square_index % 8);
      const std::string file_str{file_char};
      const int rank{8 - (square_index / 8)}; // FEN strings start from above
      assert(rank >= 1 && rank <= 8);
      const square sq(file_str + std::to_string(rank));
      const piece p(color, type, sq);
      pieces.push_back(p);
      ++str_index;
      ++square_index;
    }
  }
  assert(!pieces.empty());
  assert(pieces.size() <= 32);
  return pieces;
}

square get_current_king_square(
  const std::vector<piece>& pieces,
  const chess_color c
)
{
  assert(!pieces.empty());
  std::optional<square> s;
  for (const auto& p: pieces)
  {
    if (p.get_color() == c && p.get_type() == piece_type::king)
    {
      s = p.get_current_square();
      break;
    }
  }
  assert(s.has_value());
  return s.value();
}

std::vector<piece> get_pieces_kings_only(
  const race white_race,
  const race black_race
) noexcept
{
  const auto all_pieces{
    get_standard_starting_pieces(white_race, black_race)
  };
  std::vector<piece> pieces;
  pieces.reserve(2);
  std::copy_if(
    std::begin(all_pieces),
    std::end(all_pieces),
    std::back_inserter(pieces),
    [](const auto& piece) { return piece.get_type() == piece_type::king; }
  );
  return pieces;
}

int get_max_pieces_value() noexcept
{
  return 40;
}

std::vector<square> get_occupied_squares(const std::vector<piece>& pieces)
{
  std::vector<square> squares;
  squares.reserve(pieces.size());
  for (const auto& p: pieces)
  {
    squares.push_back(p.get_current_square());
  }
  return squares;
}

std::vector<square> get_unique_occupied_squares(const std::vector<piece>& pieces)
{
  const auto squares{get_occupied_squares(pieces)};
  assert(are_all_unique(squares));
  return squares;
}

const piece& get_piece_at(
  const std::vector<piece>& pieces,
  const square& coordinat
)
{
  const auto there{
    std::find_if(
      std::begin(pieces),
      std::end(pieces),
      [coordinat](const auto& piece)
      {
        return piece.get_current_square() == coordinat;
      }
    )
  };
  assert(there != pieces.end());
  return *there;
}

piece& get_piece_at(
  std::vector<piece>& pieces,
  const square& coordinat
)
{
  assert(is_piece_at(pieces, coordinat));
  const auto there{
    std::find_if(
      std::begin(pieces),
      std::end(pieces),
      [coordinat](const auto& piece)
      {
        return piece.get_current_square() == coordinat;
      }
    )
  };
  assert(there != pieces.end());
  return *there;
}

std::vector<square> get_possible_bishop_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
)
{
  assert(!pieces.empty());
  //assert(has_piece_with_id(pieces, focal_piece.get_id()));
  assert(focal_piece.get_type() == piece_type::bishop);
  const int x{focal_piece.get_current_square().get_x()};
  const int y{focal_piece.get_current_square().get_y()};

  std::vector<square> moves;
  std::vector<std::pair<int, int>> delta_pairs{
    std::make_pair( 1, -1), // NE
    std::make_pair( 1,  1), // SE
    std::make_pair(-1,  1), // SW
    std::make_pair(-1, -1)  // NW
  };
  for (const auto& delta_pair: delta_pairs)
  {
    for (int distance{1}; distance != 8; ++distance)
    {
      const int new_x{x + (delta_pair.first * distance)};
      const int new_y{y + (delta_pair.second * distance)};
      if (!is_valid_square_xy(new_x, new_y)) break;
      const square there(new_x, new_y);
      if (is_piece_at(pieces, there)
        && get_piece_at(pieces, there).get_color() == focal_piece.get_color()
      )
      {
        break;
      }
      moves.push_back(there);
    }
  }
  return moves;
}

std::vector<square> get_possible_king_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
)
{
  assert(!pieces.empty());
  //assert(has_piece_with_id(pieces, focal_piece.get_id()));
  assert(focal_piece.get_type() == piece_type::king);
  const int x{focal_piece.get_current_square().get_x()};
  const int y{focal_piece.get_current_square().get_y()};
  const auto enemy_color{get_other_color(focal_piece.get_color())};
  std::vector<std::pair<int, int>> xys{
    std::make_pair(x + 0, y - 1),
    std::make_pair(x + 1, y - 1),
    std::make_pair(x + 1, y - 0),
    std::make_pair(x + 1, y + 1),
    std::make_pair(x - 0, y + 1),
    std::make_pair(x - 1, y + 1),
    std::make_pair(x - 1, y + 0),
    std::make_pair(x - 1, y - 1)
  };
  const auto squares_on_board{to_squares(xys)};
  std::vector<square> squares;
  std::copy_if(
    std::begin(squares_on_board),
    std::end(squares_on_board),
    std::back_inserter(squares),
    [pieces, enemy_color](const auto& square)
    {
      return !is_piece_at(pieces, square)
        || get_piece_at(pieces, square).get_color() == enemy_color
      ;
    }
  );
  return squares;
}

std::vector<square> get_possible_knight_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
)
{
  assert(!pieces.empty());
  //assert(has_piece_with_id(pieces, focal_piece.get_id()));
  assert(focal_piece.get_type() == piece_type::knight);
  const int x{focal_piece.get_current_square().get_x()};
  const int y{focal_piece.get_current_square().get_y()};
  std::vector<square> moves;
  std::vector<std::pair<int, int>> delta_pairs{
    std::make_pair( 1, -2), // 1 o'clock
    std::make_pair( 2, -1), // 2 o'clock
    std::make_pair( 2,  1), // 4 o'clock
    std::make_pair( 1,  2), // 5 o'clock
    std::make_pair(-1,  2), // 7 o'clock
    std::make_pair(-2,  1), // 8 o'clock
    std::make_pair(-2, -1), // 10 o'clock
    std::make_pair(-1, -2)  // 11 o'clock
  };
  for (const auto& delta_pair: delta_pairs)
  {
    for (int distance{1}; distance != 4; ++distance)
    {
      const int new_x{x + (delta_pair.first * distance)};
      const int new_y{y + (delta_pair.second * distance)};
      if (!is_valid_square_xy(new_x, new_y)) break;
      const square there(new_x, new_y);
      if (is_piece_at(pieces, there)
        && get_piece_at(pieces, there).get_color() == focal_piece.get_color()
      )
      {
        break;
      }
      moves.push_back(there);
    }
  }
  return moves;
}

std::vector<square> get_possible_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
)
{
  assert(!pieces.empty());
  //assert(has_piece_with_id(pieces, focal_piece.get_id()));
  switch (focal_piece.get_type())
  {
    case piece_type::king: return get_possible_king_moves(pieces, focal_piece);
    case piece_type::pawn: return get_possible_pawn_moves(pieces, focal_piece);
    case piece_type::rook: return get_possible_rook_moves(pieces, focal_piece);
    case piece_type::queen: return get_possible_queen_moves(pieces, focal_piece);
    case piece_type::bishop: return get_possible_bishop_moves(pieces, focal_piece);
    default:
    case piece_type::knight:
      assert(focal_piece.get_type() == piece_type::knight);
      return get_possible_knight_moves(pieces, focal_piece);
  }
}

std::vector<square> get_possible_pawn_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
)
{
  assert(!pieces.empty());
  //assert(has_piece_with_id(pieces, focal_piece.get_id()));
  assert(focal_piece.get_type() == piece_type::pawn);
  const int x{focal_piece.get_current_square().get_x()};
  const int y{focal_piece.get_current_square().get_y()};

  // Can attack to where?
  const int dx{focal_piece.get_color() == chess_color::white ? 1 : -1};
  const std::vector<std::pair<int, int>> attack_xys{
    std::make_pair(x + dx, y - 1),
    std::make_pair(x + dx, y + 1)
  };
  const auto attack_squares{to_squares(attack_xys)};
  std::vector<square> valid_attack_squares;
  std::copy_if(
    std::begin(attack_squares),
    std::end(attack_squares),
    std::back_inserter(valid_attack_squares),
    [pieces, focal_piece](const auto& square)
    {
      return is_piece_at(pieces, square)
        && get_piece_at(pieces, square).get_color() != focal_piece.get_color();
    }
  );

  // Move forward
  const std::vector<std::pair<int, int>> move_xys{
    std::make_pair(x + (1 * dx), y),
    std::make_pair(x + (2 * dx), y),
    std::make_pair(x + (3 * dx), y),
    std::make_pair(x + (4 * dx), y),
    std::make_pair(x + (5 * dx), y),
    std::make_pair(x + (6 * dx), y),
    std::make_pair(x + (7 * dx), y)
  };
  const auto move_squares{to_squares(move_xys)};
  std::vector<square> valid_move_squares;
  for (const auto s: move_squares)
  {
    // Move until a piece
    if (is_piece_at(pieces, s)) break;
    valid_move_squares.push_back(s) ;
  }

  // Concatenate the vectors
  return concatenate(valid_attack_squares, valid_move_squares);
}

std::vector<square> get_possible_queen_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
)
{
  assert(!pieces.empty());
  //assert(has_piece_with_id(pieces, focal_piece.get_id()));
  assert(focal_piece.get_type() == piece_type::queen);
  const int x{focal_piece.get_current_square().get_x()};
  const int y{focal_piece.get_current_square().get_y()};

  std::vector<square> moves;
  std::vector<std::pair<int, int>> delta_pairs{
    std::make_pair( 0, -1), // N
    std::make_pair( 1, -1), // NE
    std::make_pair( 1,  0), // E
    std::make_pair( 1,  1), // SE
    std::make_pair( 0,  1), // S
    std::make_pair(-1,  1), // SW
    std::make_pair(-1,  0), // W
    std::make_pair(-1, -1)  // SE
  };
  for (const auto& delta_pair: delta_pairs)
  {
    for (int distance{1}; distance != 8; ++distance)
    {
      const int new_x{x + (delta_pair.first * distance)};
      const int new_y{y + (delta_pair.second * distance)};
      if (!is_valid_square_xy(new_x, new_y)) break;
      const square there(new_x, new_y);
      if (is_piece_at(pieces, there)
        && get_piece_at(pieces, there).get_color() == focal_piece.get_color()
      )
      {
        break;
      }
      moves.push_back(there);
    }
  }
  return moves;

}

std::vector<square> get_possible_rook_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
)
{
  assert(!pieces.empty());
  //assert(has_piece_with_id(pieces, focal_piece.get_id()));
  assert(focal_piece.get_type() == piece_type::rook);
  const int x{focal_piece.get_current_square().get_x()};
  const int y{focal_piece.get_current_square().get_y()};

  std::vector<square> moves;
  std::vector<std::pair<int, int>> delta_pairs{
    std::make_pair( 0, -1), // N
    std::make_pair( 1,  0), // E
    std::make_pair( 0,  1), // S
    std::make_pair(-1,  0)  // W
  };
  for (const auto& delta_pair: delta_pairs)
  {
    for (int distance{1}; distance != 8; ++distance)
    {
      const int new_x{x + (delta_pair.first * distance)};
      const int new_y{y + (delta_pair.second * distance)};
      if (!is_valid_square_xy(new_x, new_y)) break;
      const square there(new_x, new_y);
      if (is_piece_at(pieces, there)
        && get_piece_at(pieces, there).get_color() == focal_piece.get_color()
      )
      {
        break;
      }
      moves.push_back(there);
    }
  }
  return moves;
}


/*
std::vector<piece> get_selected_pieces(
  const std::vector<piece>& all_pieces,
  const chess_color player
)
{
  std::vector<piece> pieces;
  std::copy_if(
    std::begin(all_pieces),
    std::end(all_pieces),
    std::back_inserter(pieces),
    [player](const auto& piece)
    {
      return piece.is_selected() && piece.get_color() == player;
    }
  );
  // Selection is exclusive
  assert(pieces.size() < 2);
  return pieces;
}
*/

std::vector<piece> get_pieces_before_en_passant(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::king,   square("e1"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("a5"), white_race), // b7-b5, a5xb6 e.p.
    piece(chess_color::white, piece_type::pawn,   square("c5"), white_race), // b7-b5, c5xb6 e.p.
    piece(chess_color::white, piece_type::pawn,   square("g2"), white_race),
    piece(chess_color::black, piece_type::king,   square("e8"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("b7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("f4"), black_race), // g2-g4, f4xg3 e.p.
    piece(chess_color::black, piece_type::pawn,   square("h4"), black_race) // g2-g4, h4xg3 e.p.
  };
}

std::vector<piece> get_pieces_kasparov_vs_topalov(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::rook,   square("b7"), white_race),
    piece(chess_color::white, piece_type::queen,  square("f6"), white_race),
    piece(chess_color::white, piece_type::king,   square("b1"), white_race),
    piece(chess_color::white, piece_type::bishop, square("h3"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("a3"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("b4"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("c2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("f3"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("g3"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("h2"), white_race),
    piece(chess_color::black, piece_type::rook,   square("d8"), black_race),
    piece(chess_color::black, piece_type::queen,  square("c4"), black_race),
    piece(chess_color::black, piece_type::king,   square("a4"), black_race),
    piece(chess_color::black, piece_type::rook,   square("h8"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("a6"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("b5"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("f7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("g6"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("h7"), black_race)
  };
}

std::vector<piece> get_pieces_pawn_all_out_assault(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::rook,   square("a1"), white_race),
    piece(chess_color::white, piece_type::knight, square("b1"), white_race),
    piece(chess_color::white, piece_type::bishop, square("c1"), white_race),
    piece(chess_color::white, piece_type::queen,  square("d1"), white_race),
    piece(chess_color::white, piece_type::king,   square("e1"), white_race),
    piece(chess_color::white, piece_type::bishop, square("f1"), white_race),
    piece(chess_color::white, piece_type::knight, square("g1"), white_race),
    piece(chess_color::white, piece_type::rook,   square("h1"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("a4"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("b4"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("c4"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("d4"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("e4"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("f4"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("g4"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("h4"), white_race),
    piece(chess_color::black, piece_type::rook,   square("a8"), black_race),
    piece(chess_color::black, piece_type::knight, square("b8"), black_race),
    piece(chess_color::black, piece_type::bishop, square("c8"), black_race),
    piece(chess_color::black, piece_type::queen,  square("d8"), black_race),
    piece(chess_color::black, piece_type::king,   square("e8"), black_race),
    piece(chess_color::black, piece_type::bishop, square("f8"), black_race),
    piece(chess_color::black, piece_type::knight, square("g8"), black_race),
    piece(chess_color::black, piece_type::rook,   square("h8"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("a5"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("b5"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("c5"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("d5"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("e5"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("f5"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("g5"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("h5"), black_race)
  };
}

std::vector<piece> get_pieces_pawns_at_promotion(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::king, square("e1"), white_race),
    piece(chess_color::white, piece_type::pawn, square("a8"), white_race),
    piece(chess_color::black, piece_type::king, square("e8"), black_race),
    piece(chess_color::black, piece_type::pawn, square("h1"), black_race)
  };
}

std::vector<piece> get_pieces_pawns_near_promotion(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::king,   square("e1"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("a7"), white_race),
    piece(chess_color::black, piece_type::king,   square("e8"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("h2"), black_race)
  };
}

std::vector<piece> get_pieces_pawns_nearly_near_promotion(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::king,   square("e1"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("a6"), white_race),
    piece(chess_color::black, piece_type::king,   square("e8"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("h3"), black_race)
  };
}

std::vector<piece> get_pieces_before_scholars_mate(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::rook,   square("a1"), white_race),
    piece(chess_color::white, piece_type::knight, square("b1"), white_race),
    piece(chess_color::white, piece_type::bishop, square("c1"), white_race),
    piece(chess_color::white, piece_type::queen,  square("h5"), white_race), // 2. Qh5
    piece(chess_color::white, piece_type::king,   square("e1"), white_race),
    piece(chess_color::white, piece_type::bishop, square("c4"), white_race), // 3. Bc4
    piece(chess_color::white, piece_type::knight, square("g1"), white_race),
    piece(chess_color::white, piece_type::rook,   square("h1"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("a2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("b2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("c2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("d2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("e4"), white_race), // 1. e2-e4
    piece(chess_color::white, piece_type::pawn,   square("f2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("g2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("h2"), white_race),
    piece(chess_color::black, piece_type::rook,   square("a8"), black_race),
    piece(chess_color::black, piece_type::knight, square("c6"), black_race), // 2. ... Nc6
    piece(chess_color::black, piece_type::bishop, square("c8"), black_race),
    piece(chess_color::black, piece_type::queen,  square("d8"), black_race),
    piece(chess_color::black, piece_type::king,   square("e8"), black_race),
    piece(chess_color::black, piece_type::bishop, square("f8"), black_race),
    piece(chess_color::black, piece_type::knight, square("f6"), black_race), // 3. ... Nf6??
    piece(chess_color::black, piece_type::rook,   square("h8"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("a7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("b7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("c7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("d7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("e5"), black_race), //1. ... e7-e5
    piece(chess_color::black, piece_type::pawn,   square("f7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("g7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("h7"), black_race)
  };
}

std::vector<piece> get_pieces_bishop_and_knight_end_game(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::knight, square("c4"), white_race),
    piece(chess_color::white, piece_type::bishop, square("g4"), white_race),
    piece(chess_color::white, piece_type::king,   square("e6"), white_race),
    piece(chess_color::black, piece_type::king,   square("d2"), black_race)
  };
}

std::vector<piece> get_pieces_queen_endgame(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::queen,  square("d1"), white_race),
    piece(chess_color::white, piece_type::king,   square("e1"), white_race),
    piece(chess_color::black, piece_type::queen,  square("d8"), black_race),
    piece(chess_color::black, piece_type::king,   square("e8"), black_race)
  };
}

std::vector<piece> get_pieces_ready_to_castle(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::rook, square("a1"), white_race),
    piece(chess_color::white, piece_type::king, square("e1"), white_race),
    piece(chess_color::white, piece_type::rook, square("h1"), white_race),
    #ifdef INCLUDE_PAWNS_IN_READY_TO_CASTLE
    piece(chess_color::white, piece_type::pawn, square("a2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("b2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("c2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("d2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("e2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("f2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("g2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("h2"), white_race),
    piece(chess_color::black, piece_type::pawn, square("a7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("b7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("c7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("d7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("e7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("f7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("g7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("h7"), black_race),
    #endif // INCLUDE_PAWNS_IN_READY_TO_CASTLE
    piece(chess_color::black, piece_type::rook, square("a8"), black_race),
    piece(chess_color::black, piece_type::king, square("e8"), black_race),
    piece(chess_color::black, piece_type::rook, square("h8"), black_race)
  };
}

std::vector<piece> get_pieces_ready_to_not_castle(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::rook,   square("a1"), white_race),
    piece(chess_color::white, piece_type::knight, square("c6"), white_race),
    piece(chess_color::white, piece_type::king,   square("e1"), white_race),
    piece(chess_color::white, piece_type::knight, square("h6"), white_race),
    piece(chess_color::white, piece_type::rook,   square("h1"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("a2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("b2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("c2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("d2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("e2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("f2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("g2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("h2"), white_race),
    piece(chess_color::black, piece_type::rook,   square("a8"), black_race),
    piece(chess_color::black, piece_type::knight, square("c3"), black_race),
    piece(chess_color::black, piece_type::king,   square("e8"), black_race),
    piece(chess_color::black, piece_type::knight, square("h3"), black_race),
    piece(chess_color::black, piece_type::rook,   square("h8"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("a7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("b7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("c7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("d7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("e7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("f7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("g7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("h7"), black_race)
  };
}


std::vector<piece> get_standard_starting_pieces(
  const race white_race,
  const race black_race
) noexcept
{
  return
  {
    piece(chess_color::white, piece_type::rook,   square("a1"), white_race),
    piece(chess_color::white, piece_type::knight, square("b1"), white_race),
    piece(chess_color::white, piece_type::bishop, square("c1"), white_race),
    piece(chess_color::white, piece_type::queen,  square("d1"), white_race),
    piece(chess_color::white, piece_type::king,   square("e1"), white_race),
    piece(chess_color::white, piece_type::bishop, square("f1"), white_race),
    piece(chess_color::white, piece_type::knight, square("g1"), white_race),
    piece(chess_color::white, piece_type::rook,   square("h1"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("a2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("b2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("c2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("d2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("e2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("f2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("g2"), white_race),
    piece(chess_color::white, piece_type::pawn,   square("h2"), white_race),
    piece(chess_color::black, piece_type::rook,   square("a8"), black_race),
    piece(chess_color::black, piece_type::knight, square("b8"), black_race),
    piece(chess_color::black, piece_type::bishop, square("c8"), black_race),
    piece(chess_color::black, piece_type::queen,  square("d8"), black_race),
    piece(chess_color::black, piece_type::king,   square("e8"), black_race),
    piece(chess_color::black, piece_type::bishop, square("f8"), black_race),
    piece(chess_color::black, piece_type::knight, square("g8"), black_race),
    piece(chess_color::black, piece_type::rook,   square("h8"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("a7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("b7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("c7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("d7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("e7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("f7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("g7"), black_race),
    piece(chess_color::black, piece_type::pawn,   square("h7"), black_race)
  };
}

/*
std::vector<piece> get_starting_pieces(
  const starting_position_type spt
) noexcept
{
  return get_starting_pieces(
    spt,
    get_race_of_color(chess_color::white),
    get_race_of_color(chess_color::black)
  );
}
*/

std::vector<piece> get_starting_pieces(
  const starting_position_type t,
  const race white_race,
  const race black_race
) noexcept
{
  switch (t)
  {
    case starting_position_type::before_en_passant:
      return get_pieces_before_en_passant(white_race, black_race);
    case starting_position_type::before_scholars_mate:
      return get_pieces_before_scholars_mate(white_race, black_race);
    case starting_position_type::bishop_and_knight_end_game:
      return get_pieces_bishop_and_knight_end_game(white_race, black_race);
    case starting_position_type::kasparov_vs_topalov:
      return get_pieces_kasparov_vs_topalov(white_race, black_race);
    case starting_position_type::kings_only:
      return get_pieces_kings_only(white_race, black_race);
    case starting_position_type::pawn_all_out_assault:
      return get_pieces_pawn_all_out_assault(white_race, black_race);
    case starting_position_type::pawns_at_promotion:
      return get_pieces_pawns_at_promotion(white_race, black_race);
    case starting_position_type::pawns_near_promotion:
      return get_pieces_pawns_near_promotion(white_race, black_race);
    case starting_position_type::pawns_nearly_near_promotion:
      return get_pieces_pawns_nearly_near_promotion(white_race, black_race);
    case starting_position_type::queen_end_game:
      return get_pieces_queen_endgame(white_race, black_race);
    case starting_position_type::ready_to_castle:
      return get_pieces_ready_to_castle(white_race, black_race);
    case starting_position_type::ready_to_not_castle:
      return get_pieces_ready_to_not_castle(white_race, black_race);
    case starting_position_type::standard:
    default:
      assert(t == starting_position_type::standard);
      return get_standard_starting_pieces(white_race, black_race);
  }
}



int get_total_pieces_value(
  const std::vector<piece>& pieces,
  const chess_color c
)
{
  int sum{0};
  for (const auto& p: pieces)
  {
    if (p.get_color() != c) continue;
    if (p.get_type() == piece_type::king) continue;
    sum += get_piece_value(p.get_type());
  }
  return sum;
}

bool has_king(
  const std::vector<piece>& pieces,
  const chess_color c
)
{
  bool has_king{false};
  for (const auto& p: pieces)
  {
    if (p.get_color() == c && p.get_type() == piece_type::king)
    {
      has_king = true;
      break;
    }
  }
  return has_king;
}

bool is_checkmate(
  const std::vector<piece>& pieces,
  const chess_color player_in_checkmate
)
{
  const std::string fen_string{to_fen_str(pieces, player_in_checkmate)};
  const chess::Board board(fen_string);
  const auto p = board.isGameOver();
  return p.first == chess::GameResultReason::CHECKMATE;
}

bool is_draw(const std::vector<piece>& pieces)
{
  return pieces.size() <= 2;
}

bool is_king_under_attack(
  const std::vector<piece>& pieces,
  const chess_color player_color
)
{
  const square king_square{get_current_king_square(pieces, player_color)};
  const chess_color opponent_color{get_other_color(player_color)};
  return is_square_attacked(pieces, king_square, opponent_color);
}

bool is_piece_at(
  const std::vector<piece>& pieces,
  const game_coordinate& coordinat,
  const double distance
) {
  const std::vector<double> distances{
    calc_distances(pieces, coordinat)
  };
  const auto iter = std::find_if(
    std::begin(distances),
    std::end(distances),
    [distance](const double this_distance) { return this_distance < distance; }
  );
  return iter != std::end(distances);
}

bool is_piece_at(
  const std::vector<piece>& pieces,
  const square& coordinat
) {
  const auto iter = std::find_if(
    std::begin(pieces),
    std::end(pieces),
    [coordinat](const auto& piece)
    {
      return piece.get_current_square() == coordinat;
    }
  );
  return iter != std::end(pieces);
}

bool is_piece_looking_at_square(
  const std::vector<piece>& pieces,
  const square& piece_square,
  const square& target_square
)
{
  assert(is_piece_at(pieces, piece_square));
  const auto& piece{get_piece_at(pieces, piece_square)};
  const auto piece_color{piece.get_color()};
  const auto piece_type{piece.get_type()};

  if (piece_type == piece_type::pawn)
  {
    return is_forward(piece_color, piece_square, target_square)
      && are_on_adjacent_diagonal(piece_square, target_square)
    ;
  }

  assert(piece_type != piece_type::pawn);
  if (!can_move_on_empty_board(
      piece_color,
      piece_type,
      piece_square,
      target_square
    )
  )
  {
    return false;
  }
  // Knight hop over pieces
  if (piece_type == piece_type::knight)
  {
    return true;
  }
  // Inclusive
  auto squares{get_intermediate_squares(piece_square, target_square)};
  assert(squares.size() >= 2); // includes start and end

  // They are adjacent
  if (squares.size() == 2) return true;

  // Only keep the middle
  squares.erase(squares.begin());
  squares.pop_back();
  assert(!squares.empty());
  for (const auto s: squares)
  {
    if (is_piece_at(pieces, s)) return false;
  }
  return true;
}

bool is_square_attacked(
  const std::vector<piece>& pieces,
  const square& s,
  const chess_color attacker_color
)
{
  for (const auto& p: pieces)
  {
    if (p.get_color() != attacker_color) continue;

    if (p.get_current_square() == s)
    {
      // Sure, the attacker can be here
      continue;
    }
    if (is_piece_looking_at_square(pieces, p.get_current_square(), s))
    {
      return true;
    }
  }
  return false;
}

bool is_square_protected(
  const std::vector<piece>& pieces,
  const square& s,
  const chess_color c
)
{
  for (const auto& p: pieces)
  {
    if (p.get_color() != c) continue;

    if (p.get_current_square() == s)
    {
      // The piece we want to find out of if it is protected
      assert(p.get_color() == c);
      continue;
    }
    if (is_piece_looking_at_square(pieces, p.get_current_square(), s))
    {
      return true;
    }
  }
  return false;
}

void test_pieces()
{
#ifndef NDEBUG
  // can_move
  {
    // Standard board
    {
      const auto pieces{get_standard_starting_pieces()};
      assert(can_move(pieces, chess_color::white, piece_type::knight, square("b1"), square("c3")));
      assert(!can_move(pieces, chess_color::white, piece_type::knight, square("b1"), square("d2")));
      assert(!can_move(pieces, chess_color::white, piece_type::queen, square("d1"), square("d2")));
      assert(!can_move(pieces, chess_color::white, piece_type::queen, square("d1"), square("d3")));
      assert(can_move(pieces, chess_color::white, piece_type::knight, square("g1"), square("f3")));
      assert(can_move(pieces, chess_color::white, piece_type::pawn, square("a2"), square("a3")));
      assert(can_move(pieces, chess_color::white, piece_type::pawn, square("a2"), square("a4")));
      assert(!can_move(pieces, chess_color::white, piece_type::pawn, square("a2"), square("a1")));

      assert(can_move(pieces, chess_color::black, piece_type::knight, square("b8"), square("c6")));
      assert(can_move(pieces, chess_color::black, piece_type::knight, square("g8"), square("f6")));
      assert(can_move(pieces, chess_color::black, piece_type::pawn, square("a7"), square("a6")));
      assert(can_move(pieces, chess_color::black, piece_type::pawn, square("a7"), square("a5")));
      assert(!can_move(pieces, chess_color::black, piece_type::pawn, square("a7"), square("a8")));

    }
    // All pawns have moved two squares forward
    {
      const auto pieces{get_pieces_pawn_all_out_assault()};
      assert(can_move(pieces, chess_color::white, piece_type::rook, square("a1"), square("a3")));
      assert(!can_move(pieces, chess_color::white, piece_type::rook, square("a1"), square("a7")));
      assert(can_move(pieces, chess_color::white, piece_type::knight, square("b1"), square("d2")));
      assert(can_move(pieces, chess_color::white, piece_type::bishop, square("c1"), square("e3")));
      assert(can_move(pieces, chess_color::white, piece_type::queen, square("d1"), square("d3")));
      assert(can_move(pieces, chess_color::white, piece_type::king, square("e1"), square("e2")));
      assert(can_move(pieces, chess_color::white, piece_type::bishop, square("f1"), square("h3")));
      assert(can_move(pieces, chess_color::white, piece_type::knight, square("g1"), square("e2")));
      assert(can_move(pieces, chess_color::white, piece_type::rook, square("h1"), square("h3")));

      assert(can_move(pieces, chess_color::black, piece_type::rook, square("a8"), square("a6")));
      assert(can_move(pieces, chess_color::black, piece_type::knight, square("b8"), square("d7")));
      assert(can_move(pieces, chess_color::black, piece_type::bishop, square("c8"), square("e6")));
      assert(can_move(pieces, chess_color::black, piece_type::queen, square("d8"), square("d6")));
      assert(can_move(pieces, chess_color::black, piece_type::king, square("e8"), square("e7")));
      assert(can_move(pieces, chess_color::black, piece_type::bishop, square("f8"), square("h6")));
      assert(can_move(pieces, chess_color::black, piece_type::knight, square("g8"), square("e7")));
      assert(can_move(pieces, chess_color::black, piece_type::rook, square("h8"), square("h6")));
    }
  }
  // count_dead_pieces
  {
    // No dead pieces at the start
    {
      const auto pieces{get_standard_starting_pieces()};
      assert(count_dead_pieces(pieces) == 0);
    }
    // One dead piece if it is killed
    {
      auto pieces{get_standard_starting_pieces()};
      pieces.back().receive_damage(1000000.0);
      assert(count_dead_pieces(pieces) == 1);
    }
  }
  // create_pieces_from_fen_string
  {
    const auto pieces{create_pieces_from_fen_string(get_fen_string_wikipedia_0())};
    assert(pieces.size() == 32);
    assert(is_piece_at(pieces, square("a1")));
    assert(is_piece_at(pieces, square("h1")));
    assert(is_piece_at(pieces, square("a8")));
    assert(is_piece_at(pieces, square("h8")));

    assert(get_piece_at(pieces, square("a1")).get_type() == piece_type::rook);
    assert(get_piece_at(pieces, square("a1")).get_color() == chess_color::white);
    assert(get_piece_at(pieces, square("d8")).get_type() == piece_type::queen);
    assert(get_piece_at(pieces, square("d8")).get_color() == chess_color::black);
  }
  // get_current_king_square
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(get_current_king_square(pieces, chess_color::black) == square("e8"));
    assert(get_current_king_square(pieces, chess_color::white) == square("e1"));
  }
  // get_piece_at, const
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(get_piece_at(pieces, square("d1")).get_type() == piece_type::queen);
  }
  // get_piece_at, non-const
  {
    auto pieces{get_standard_starting_pieces()};
    auto& piece{get_piece_at(pieces, square("d1"))};
    piece.set_current_square(square("e1")); // Just needs to compile
  }
  // get_pieces_before_scholars_mate
  {
    const auto pieces{get_pieces_before_scholars_mate()};
    assert(pieces.size() == 32);
  }
  // get_pieces_bishop_and_knight_end_game
  {
    const auto pieces{get_pieces_bishop_and_knight_end_game()};
    assert(pieces.size() == 4);
  }
  // get_possible_moves
  {
    // At the start, only knights can move, but they can move far
    {
      const auto pieces{get_standard_starting_pieces()};
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("a1"))).size() == 0);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("b1"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("c1"))).size() == 0);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("d1"))).size() == 0);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e1"))).size() == 0);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("f1"))).size() == 0);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("g1"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("h1"))).size() == 0);
      // pawns can move forward as much as they can
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("a2"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("b2"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("c2"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("d2"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e2"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("f2"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("g2"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("h2"))).size() == 4);
    }
    // King-versus_king
    {
      const auto pieces{get_starting_pieces(starting_position_type::kings_only)};
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e1"))).size() == 5);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e8"))).size() == 5);
    }
    // pawn_all_out_assault
    {
      const auto pieces{get_starting_pieces(starting_position_type::pawn_all_out_assault)};
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("a1"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("b1"))).size() == 6);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("c1"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("d1"))).size() == 6);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e1"))).size() == 3);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("f1"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("g1"))).size() == 6);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("h1"))).size() == 2);

      assert(get_possible_moves(pieces, get_piece_at(pieces, square("a4"))).size() == 1);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("b4"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("c4"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("d4"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e4"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("f4"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("g4"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("h4"))).size() == 1);

      assert(get_possible_moves(pieces, get_piece_at(pieces, square("a5"))).size() == 1);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("b5"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("c5"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("d5"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e5"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("f5"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("g5"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("h5"))).size() == 1);

      assert(get_possible_moves(pieces, get_piece_at(pieces, square("a8"))).size() == 2);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("b8"))).size() == 6);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("c8"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("d8"))).size() == 6);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e8"))).size() == 3);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("f8"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("g8"))).size() == 6);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("h8"))).size() == 2);
    }
    // queen_end_game
    {
      const auto pieces{get_starting_pieces(starting_position_type::queen_end_game)};
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("d1"))).size() == 17);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e1"))).size() == 4);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("d8"))).size() == 17);
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("e8"))).size() == 4);
    }
    // bishop_and_knight_end_game
    {
      const auto pieces{get_starting_pieces(starting_position_type::bishop_and_knight_end_game)};
      assert(get_possible_moves(pieces, get_piece_at(pieces, square("g4"))).size() == 6);
    }
  }
  // get_standard_starting_pieces
  {
    const auto pieces_1{get_standard_starting_pieces()};
    assert(pieces_1.size() == 32);
  }
  // get_starting_pieces, manual
  {
    const auto pieces_1{get_starting_pieces(starting_position_type::standard)};
    const auto pieces_2{get_starting_pieces(starting_position_type::kings_only)};
    const auto pieces_3{get_starting_pieces(starting_position_type::before_scholars_mate)};
    const auto pieces_4{get_starting_pieces(starting_position_type::bishop_and_knight_end_game)};
    assert(pieces_1 != pieces_2);
    assert(pieces_1 != pieces_3);
    assert(pieces_1 != pieces_4);
    assert(pieces_2 != pieces_3);
    assert(pieces_2 != pieces_4);
    assert(pieces_3 != pieces_4);
  }
  // get_starting_pieces, collection
  {
    for (const auto t: get_all_starting_position_types())
    {
      assert(!get_starting_pieces(t).empty());
    }
  }
  // get_total_pieces_value
  {
    // Default
    {
      const auto pieces{get_standard_starting_pieces()};
      const int x{get_total_pieces_value(pieces, chess_color::white)};
      assert(x == get_max_pieces_value());
    }
    // Each side has a king and queen
    {
      const auto pieces{get_pieces_queen_endgame()};
      assert(get_total_pieces_value(pieces, chess_color::white) == 10);
      assert(get_total_pieces_value(pieces, chess_color::black) == 10);
    }
    // White has a bishop and knight, black has nothing
    {
      const auto pieces{get_pieces_bishop_and_knight_end_game()};
      assert(get_total_pieces_value(pieces, chess_color::white) == 6);
      assert(get_total_pieces_value(pieces, chess_color::black) == 0);
    }
  }
  // has_king
  {
    auto pieces{get_pieces_queen_endgame()};
    assert(has_king(pieces, chess_color::black));
    assert(has_king(pieces, chess_color::white));
    assert(pieces.back().get_type() == piece_type::king);
    assert(pieces.back().get_color() == chess_color::black);
    pieces.pop_back();
    assert(!has_king(pieces, chess_color::black));
  }
  // is_checkmate
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(!is_checkmate(pieces, chess_color::black));
    assert(!is_checkmate(pieces, chess_color::white));
  }
  // is_king_under_attack
  {
    auto pieces{get_pieces_queen_endgame()};
    assert(!is_king_under_attack(pieces, chess_color::black));
    assert(!is_king_under_attack(pieces, chess_color::white));
  }
  // is_piece_at, const
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(is_piece_at(pieces, square("d1")));
  }
  // is_piece_at, with distance, const
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(is_piece_at(pieces, to_coordinat(square("d1")), 1.0));
    assert(!is_piece_at(pieces, to_coordinat(square("e4")), 1.0));
  }
  // is_piece_looking_at_square
  {
    // Standard
    {
      const auto pieces{get_standard_starting_pieces()};
      assert(is_piece_looking_at_square(pieces, square("d1"), square("d2")));
      assert(!is_piece_looking_at_square(pieces, square("d1"), square("d3")));

      // Pawns look diagonally
      assert(!is_piece_looking_at_square(pieces, square("e2"), square("e3")));
      assert(!is_piece_looking_at_square(pieces, square("e2"), square("e4")));
      assert(is_piece_looking_at_square(pieces, square("e2"), square("d3")));
      assert(is_piece_looking_at_square(pieces, square("e2"), square("f3")));
    }
    // Pawn all-out assault, king can only see 1 square
    {
      const auto pieces{get_pieces_pawn_all_out_assault()};
      assert(is_piece_looking_at_square(pieces, square("e1"), square("e2")));
      assert(!is_piece_looking_at_square(pieces, square("e1"), square("e3")));
    }
    // Pawn all-out assault, queen can see far
    {
      const auto pieces{get_pieces_pawn_all_out_assault()};
      assert(is_piece_looking_at_square(pieces, square("d1"), square("d3")));
    }
  }
  // is_square_protected
  {
    // standard
    {
      const auto pieces{get_standard_starting_pieces()};
      assert(!is_square_protected(pieces, square("a1"), chess_color::white));
      assert(is_square_protected(pieces, square("a2"), chess_color::white));
      assert(is_square_protected(pieces, square("a3"), chess_color::white));
      assert(!is_square_protected(pieces, square("a4"), chess_color::white));
      assert(!is_square_protected(pieces, square("a5"), chess_color::white));
      assert(!is_square_protected(pieces, square("a6"), chess_color::white));
      assert(!is_square_protected(pieces, square("a7"), chess_color::white));
      assert(!is_square_protected(pieces, square("a8"), chess_color::white));

      assert(is_square_protected(pieces, square("b1"), chess_color::white));
      assert(is_square_protected(pieces, square("b2"), chess_color::white));
      assert(is_square_protected(pieces, square("b3"), chess_color::white));
      assert(!is_square_protected(pieces, square("b4"), chess_color::white));
      assert(!is_square_protected(pieces, square("b5"), chess_color::white));
      assert(!is_square_protected(pieces, square("b6"), chess_color::white));
      assert(!is_square_protected(pieces, square("b7"), chess_color::white));
      assert(!is_square_protected(pieces, square("b8"), chess_color::white));

      assert(is_square_protected(pieces, square("c1"), chess_color::white));
      assert(is_square_protected(pieces, square("c2"), chess_color::white));
      assert(is_square_protected(pieces, square("c3"), chess_color::white));
      assert(!is_square_protected(pieces, square("c4"), chess_color::white));
      assert(!is_square_protected(pieces, square("c5"), chess_color::white));
      assert(!is_square_protected(pieces, square("c6"), chess_color::white));
      assert(!is_square_protected(pieces, square("c7"), chess_color::white));
      assert(!is_square_protected(pieces, square("c8"), chess_color::white));

      assert(is_square_protected(pieces, square("d1"), chess_color::white));
      assert(is_square_protected(pieces, square("d2"), chess_color::white));
      assert(is_square_protected(pieces, square("d3"), chess_color::white));
      assert(!is_square_protected(pieces, square("d4"), chess_color::white));
      assert(!is_square_protected(pieces, square("d5"), chess_color::white));
      assert(!is_square_protected(pieces, square("d6"), chess_color::white));
      assert(!is_square_protected(pieces, square("d7"), chess_color::white));
      assert(!is_square_protected(pieces, square("d8"), chess_color::white));

      assert(is_square_protected(pieces, square("e1"), chess_color::white));
      assert(is_square_protected(pieces, square("e2"), chess_color::white));
      assert(is_square_protected(pieces, square("e3"), chess_color::white));
      assert(!is_square_protected(pieces, square("e4"), chess_color::white));
      assert(!is_square_protected(pieces, square("e5"), chess_color::white));
      assert(!is_square_protected(pieces, square("e6"), chess_color::white));
      assert(!is_square_protected(pieces, square("e7"), chess_color::white));
      assert(!is_square_protected(pieces, square("e8"), chess_color::white));

      assert(is_square_protected(pieces, square("f1"), chess_color::white));
      assert(is_square_protected(pieces, square("f2"), chess_color::white));
      assert(is_square_protected(pieces, square("f3"), chess_color::white));
      assert(!is_square_protected(pieces, square("f4"), chess_color::white));
      assert(!is_square_protected(pieces, square("f5"), chess_color::white));
      assert(!is_square_protected(pieces, square("f6"), chess_color::white));
      assert(!is_square_protected(pieces, square("f7"), chess_color::white));
      assert(!is_square_protected(pieces, square("f8"), chess_color::white));
    }
    // Pawn all out assault
    {
      const auto pieces{get_pieces_pawn_all_out_assault()};
      assert(!is_square_protected(pieces, square("e4"), chess_color::white));
    }
    // Kasparov versus Topalov
    {
      const auto pieces{get_pieces_kasparov_vs_topalov()};
      assert(!is_square_protected(pieces, square("a3"), chess_color::white));

      // Pawn at a3 protects the pawn on b4
      assert(is_piece_at(pieces, square("a3")));
      assert(get_piece_at(pieces, square("a3")).get_color() == chess_color::white);
      assert(get_piece_at(pieces, square("a3")).get_type() == piece_type::pawn);
      assert(is_piece_at(pieces, square("b4")));
      assert(get_piece_at(pieces, square("b4")).get_color() == chess_color::white);
      assert(get_piece_at(pieces, square("b4")).get_type() == piece_type::pawn);
      assert(is_square_protected(pieces, square("b4"), chess_color::white));


      assert(!is_square_protected(pieces, square("h2"), chess_color::white));

      // Pawn at h2 protects the pawn on g3
      assert(is_piece_at(pieces, square("h2")));
      assert(get_piece_at(pieces, square("h2")).get_color() == chess_color::white);
      assert(get_piece_at(pieces, square("h2")).get_type() == piece_type::pawn);
      assert(is_piece_at(pieces, square("g3")));
      assert(get_piece_at(pieces, square("g3")).get_color() == chess_color::white);
      assert(get_piece_at(pieces, square("g3")).get_type() == piece_type::pawn);
      assert(is_square_protected(pieces, square("g3"), chess_color::white));

    }
  }
  // is_square_attacked
  {
    const auto pieces{get_pieces_kasparov_vs_topalov()};

    // Black attacks the empty square at a2
    assert(is_square_attacked(pieces, square("a2"), chess_color::black));

    // Black attacks the white pawn at a3
    assert(is_square_attacked(pieces, square("a3"), chess_color::black));

    // Black has an unproteced pawn at a6
    assert(!is_square_attacked(pieces, square("a6"), chess_color::black));

    // Black's pawn at b4 is protected
    assert(is_square_attacked(pieces, square("b4"), chess_color::black));

    // Black cannot attacks the pawn at c2
    assert(is_square_attacked(pieces, square("c2"), chess_color::black));

    // Black cannot attack over the pawn at c2
    assert(!is_square_attacked(pieces, square("c1"), chess_color::black));

    // Black's rook at d8 is protected
    assert(is_square_attacked(pieces, square("d8"), chess_color::black));

    // Black's rook at h8 is protected
    assert(is_square_attacked(pieces, square("h8"), chess_color::black));
  }

  // to_board_strs
  {
    const auto pieces{get_standard_starting_pieces()};
    const auto strs{to_board_strs(pieces)};
    assert(strs.size() == 8);
    assert(strs[0].size() == 8);
  }
  // 35: to_board_strs shows coordinats
  {
    const auto pieces{get_standard_starting_pieces()};
    board_to_text_options options;
    options.set_show_coordinats(true);
    const auto strs{to_board_strs(pieces, options)};
    assert(strs.size() == 8 + 4);
    assert(strs[0].size() == 8 + 4);
    assert(strs[0] == "  12345678  ");
    assert(strs[1] == " +--------+ ");
    assert(strs[2][0] == 'A');
    assert(strs[2][1] == '|');
    assert(strs[2][10] == '|');
    assert(strs[2][11] == 'A');
  }
  // to_board_str
  {
    const auto pieces{get_standard_starting_pieces()};
    const auto str{to_board_str(pieces)};
    assert(str.size() >= (8 * 8));
  }
  // 35: to_board_strs shows coordinats
  {
    const auto pieces{get_standard_starting_pieces()};
    const board_to_text_options no_coords(false);
    assert(!no_coords.get_show_coordinats());
    const auto str_without_coordinats{to_board_strs(pieces, no_coords)};
    const board_to_text_options with_coords(true);
    assert(with_coords.get_show_coordinats());
    const auto str_with_coordinats{to_board_strs(pieces, with_coords)};
    assert(str_with_coordinats.size() >= str_without_coordinats.size());

  }
  // 35: to_board_str shows coordinats
  {
    const auto pieces{get_standard_starting_pieces()};
    const board_to_text_options no_coords(false);
    assert(!no_coords.get_show_coordinats());
    const auto str_without_coordinats{to_board_str(pieces, no_coords)};
    const board_to_text_options with_coords(true);
    assert(with_coords.get_show_coordinats());
    const auto str_with_coordinats{to_board_str(pieces, with_coords)};
    assert(str_with_coordinats.size() >= str_without_coordinats.size());
  }
  // 36: to_board_strs shows legend
  {
    const auto pieces{get_standard_starting_pieces()};
    const board_to_text_options no_legend(false, false);
    assert(!no_legend.get_show_coordinats());
    assert(!no_legend.get_show_legend());
    const auto str_without_coordinats{to_board_strs(pieces, no_legend)};
    const board_to_text_options with_legend(false, true);
    assert(!no_legend.get_show_coordinats());
    assert(!no_legend.get_show_legend());
    const auto str_with_coordinats{to_board_strs(pieces, with_legend)};
    assert(str_with_coordinats.size() >= str_without_coordinats.size());

  }
  // 35: to_board_str shows legend
  {
    const auto pieces{get_standard_starting_pieces()};
    const board_to_text_options no_legend(false, false);
    assert(!no_legend.get_show_legend());
    const auto str_without_coordinats{to_board_str(pieces, no_legend)};
    const board_to_text_options with_legend(false, true);
    assert(with_legend.get_show_legend());
    const auto str_with_coordinats{to_board_str(pieces, with_legend)};
    assert(str_with_coordinats.size() >= str_without_coordinats.size());
  }

  // to_fen_string
  {
    const auto pieces{get_standard_starting_pieces()};
    const auto s{to_fen_str(pieces)};
    assert(s == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  }

  // to_pgn
  {
    const auto pieces{get_standard_starting_pieces()};
    const auto s{to_pgn(pieces)};
    assert(s.empty());
  }
#endif
}

std::string to_board_str(
  const std::vector<piece>& pieces,
  const board_to_text_options& options
) noexcept
{
  const auto strs{to_board_strs(pieces, options)};
  std::string s;
  for (const auto& str: strs) s += str + '\n';
  assert(!s.empty());
  s.pop_back();
  return s;
}

std::vector<std::string> to_board_strs(
  const std::vector<piece>& pieces,
  const board_to_text_options& options
) noexcept
{
  std::vector<std::string> board(8, std::string("........"));
  for (const auto& p: pieces)
  {
    const int x{p.get_current_square().get_x()};
    const int y{p.get_current_square().get_y()};
    board[y][x] = to_fen_char(p);
  }
  if (options.get_show_coordinats())
  {
    board = add_coordinats(board);
  }
  if (options.get_show_legend())
  {
    board = add_legend(board);
  }
  return board;
}

std::string to_fen_str(
  const std::vector<piece>& pieces,
  const chess_color active_color,
  const std::string castling_availability,
  const std::string en_passant_target_square,
  const int halfmove_clock,
  const int fullmove_number
)
{
  std::string s;
  s.reserve(100);

  // FEN starts from rank 8 and moves backwards
  const auto ranks = get_all_ranks_in_reversed_order();
  for (const int rank: ranks)
  {
    for (const char file: get_all_files())
    {
      const square sq(file + std::to_string(rank));
      if (is_piece_at(pieces, sq))
      {
        s += to_fen_char(get_piece_at(pieces, sq));
      }
      else
      {
        s += ".";
      }

    }
    s += "/";
  }
  assert(s.back() == '/');
  s.pop_back();

  // Replace dots by numbers
  s = std::regex_replace(s, std::regex("\\.{8}"), "8" );
  s = std::regex_replace(s, std::regex("\\.{7}"), "7" );
  s = std::regex_replace(s, std::regex("\\.{6}"), "6" );
  s = std::regex_replace(s, std::regex("\\.{5}"), "5" );
  s = std::regex_replace(s, std::regex("\\.{4}"), "4" );
  s = std::regex_replace(s, std::regex("\\.{3}"), "3" );
  s = std::regex_replace(s, std::regex("\\.{2}"), "2" );
  s = std::regex_replace(s, std::regex("\\.{1}"), "1" );
  assert(std::count(s.begin(), s.end(), '.') == 0);

  s += std::string(" ") + to_fen_char(active_color) + " "
    + castling_availability + " "
    + en_passant_target_square + " "
    + std::to_string(halfmove_clock) + " "
    + std::to_string(fullmove_number)
  ;
  return s;
}


std::string to_pgn(const std::vector<piece>& pieces)
{
  const auto history{collect_action_history(pieces)};
  std::stringstream s;
  s << history;
  return s.str();
}
