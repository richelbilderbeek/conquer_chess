#include "pieces.h"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <iostream>
#include <sstream>

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
  text_with_legend[2] += " bishop| b   | B   ";
  text_with_legend[3] += " king  | k   | K   ";
  text_with_legend[4] += " knight| n   | N   ";
  text_with_legend[5] += " pawn  | p   | P   ";
  text_with_legend[6] += " queen | q   | Q   ";
  text_with_legend[7] += " rook  | r   | R   ";
  return text_with_legend;
}

std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
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

int count_selected_units(
  const std::vector<piece>& pieces
)
{
  return std::count_if(
    std::begin(pieces),
    std::end(pieces),
    [](const auto& piece)
    {
      return piece.is_selected();
    }
  );
}

int count_selected_units(
  const std::vector<piece>& pieces,
  const chess_color player
)
{
  return std::count_if(
    std::begin(pieces),
    std::end(pieces),
    [player](const auto& piece)
    {
      return piece.is_selected()
        && piece.get_color() == player;
    }
  );
}

std::vector<piece> get_kings_only_starting_pieces(
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

std::vector<square> get_occupied_squares(const std::vector<piece>& pieces) noexcept
{
  std::vector<square> squares;
  squares.reserve(pieces.size());
  for (const auto& p: pieces)
  {
    squares.push_back(p.get_current_square());
  }
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

piece get_piece_with_id(
  const std::vector<piece>& pieces,
  const id& i
)
{
  const auto there{
    std::find_if(
      std::begin(pieces),
      std::end(pieces),
      [i](const auto& p)
      {
        return p.get_id() == i;
      }
    )
  };
  assert(there != std::end(pieces));
  return *there;
}

std::vector<square> get_possible_bishop_moves(
  const std::vector<piece>& pieces,
  const piece& focal_piece
)
{
  assert(!pieces.empty());
  assert(has_piece_with_id(pieces, focal_piece.get_id()));
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
  assert(has_piece_with_id(pieces, focal_piece.get_id()));
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
  assert(has_piece_with_id(pieces, focal_piece.get_id()));
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
  assert(has_piece_with_id(pieces, focal_piece.get_id()));
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
  assert(has_piece_with_id(pieces, focal_piece.get_id()));
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
  assert(has_piece_with_id(pieces, focal_piece.get_id()));
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
  assert(has_piece_with_id(pieces, focal_piece.get_id()));
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
  return pieces;
}

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
    piece(chess_color::white, piece_type::pawn, square("a2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("b2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("c2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("d2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("e2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("f2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("g2"), white_race),
    piece(chess_color::white, piece_type::pawn, square("h2"), white_race),
    piece(chess_color::black, piece_type::rook, square("a8"), black_race),
    piece(chess_color::black, piece_type::king, square("e8"), black_race),
    piece(chess_color::black, piece_type::rook, square("h8"), black_race),
    piece(chess_color::black, piece_type::pawn, square("a7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("b7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("c7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("d7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("e7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("f7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("g7"), black_race),
    piece(chess_color::black, piece_type::pawn, square("h7"), black_race)
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
      return get_kings_only_starting_pieces(white_race, black_race);
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

bool has_piece_with_id(
  const std::vector<piece>& pieces,
  const id& i
)
{
  assert(!pieces.empty());
  const auto there{
    std::find_if(
      std::begin(pieces),
      std::end(pieces),
      [i](const auto& piece) { return piece.get_id() == i; }
    )
  };
  return there != std::end(pieces);
}

bool is_piece_at(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat,
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

void test_pieces()
{
#ifndef NDEBUG
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
  // get_piece_at, const
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(get_piece_at(pieces, square("d1")).get_type() == piece_type::queen);
  }
  // get_piece_at, non-const
  {
    auto pieces{get_standard_starting_pieces()};
    auto& piece{get_piece_at(pieces, square("d1"))};
    piece.set_selected(true); // Just needs to compile
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
  // has_piece_with_id
  {
    const auto pieces{get_starting_pieces(starting_position_type::kings_only)};
    //has_piece_with_id(pieces, )
  }
  // is_piece_at, const
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(is_piece_at(pieces, square("d1")));
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
    board[y][x] = to_char(p);
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


std::string to_pgn(const std::vector<piece>& pieces)
{
  const auto history{collect_action_history(pieces)};
  std::stringstream s;
  s << history;
  return s.str();
}

void unselect_all_pieces(
  std::vector<piece>& pieces,
  const chess_color color
)
{
  for (auto& piece: pieces)
  {
    if (piece.get_color() == color) unselect(piece);
  }
}
