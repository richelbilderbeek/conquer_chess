#include "pieces.h"

#include <algorithm>
#include <cassert>
#include <numeric>

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
        piece.get_coordinat()
      );
    }
  );
  assert(distances.size() == pieces.size());
  return distances;
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
  const chess_color left_player_color
) noexcept
{
  const auto all_pieces{get_standard_starting_pieces(left_player_color)};
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

const piece& get_piece_at(
  const std::vector<piece>& pieces,
  const square& coordinat
)
{
  const auto there{
    std::find_if(
      std::begin(pieces),
      std::end(pieces),
      [coordinat](const auto& piece) { return piece.get_coordinat() == coordinat; }
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
  const auto there{
    std::find_if(
      std::begin(pieces),
      std::end(pieces),
      [coordinat](const auto& piece) { return piece.get_coordinat() == coordinat; }
    )
  };
  assert(there != pieces.end());
  return *there;
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


std::vector<piece> get_pieces_before_scholars_mate(
  const chess_color left_player_color
) noexcept
{
  const auto f{
    left_player_color == chess_color::white
    ? [](const square& position) { return position; }
    : [](const square& position) { return get_rotated_square(position); }
  };
  const side white_side{
    left_player_color == chess_color::white
    ? side::lhs
    : side::rhs
  };
  const side black_side{get_other_side(white_side)};

  std::vector<piece> pieces{
    piece(chess_color::white, piece_type::rook,   f(square("a1")), white_side),
    piece(chess_color::white, piece_type::knight, f(square("b1")), white_side),
    piece(chess_color::white, piece_type::bishop, f(square("c1")), white_side),
    piece(chess_color::white, piece_type::queen,  f(square("h5")), white_side), // 2. Qh5
    piece(chess_color::white, piece_type::king,   f(square("e1")), white_side),
    piece(chess_color::white, piece_type::bishop, f(square("c4")), white_side), // 3. Bc4
    piece(chess_color::white, piece_type::knight, f(square("g1")), white_side),
    piece(chess_color::white, piece_type::rook,   f(square("h1")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("a2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("b2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("c2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("d2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("e4")), white_side), // 1. e2-e4
    piece(chess_color::white, piece_type::pawn,   f(square("f2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("g2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("h2")), white_side),
    piece(chess_color::black, piece_type::rook,   f(square("a8")), black_side),
    piece(chess_color::black, piece_type::knight, f(square("c6")), black_side), // 2. ... Nc6
    piece(chess_color::black, piece_type::bishop, f(square("c8")), black_side),
    piece(chess_color::black, piece_type::queen,  f(square("d8")), black_side),
    piece(chess_color::black, piece_type::king,   f(square("e8")), black_side),
    piece(chess_color::black, piece_type::bishop, f(square("f8")), black_side),
    piece(chess_color::black, piece_type::knight, f(square("f6")), black_side), // 3. ... Nf6??
    piece(chess_color::black, piece_type::rook,   f(square("h8")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("a7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("b7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("c7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("d7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("e5")), black_side), //1. ... e7-e5
    piece(chess_color::black, piece_type::pawn,   f(square("f7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("g7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("h7")), black_side)
  };
  return pieces;
}

std::vector<piece> get_pieces_bishop_and_knight_end_game(
  const chess_color left_player_color
) noexcept
{
  const auto f{
    left_player_color == chess_color::white
    ? [](const square& position) { return position; }
    : [](const square& position) { return get_rotated_square(position); }
  };
  const side white_side{
    left_player_color == chess_color::white
    ? side::lhs
    : side::rhs
  };
  const side black_side{get_other_side(white_side)};
  std::vector<piece> pieces{
    piece(chess_color::white, piece_type::knight, f(square("c4")), white_side),
    piece(chess_color::white, piece_type::bishop, f(square("g4")), white_side),
    piece(chess_color::white, piece_type::king,   f(square("e6")), white_side),
    piece(chess_color::black, piece_type::king,   f(square("d2")), black_side)
  };
  return pieces;
}

std::vector<piece> get_standard_starting_pieces(
  const chess_color left_player_color
) noexcept
{
  const auto f{
    left_player_color == chess_color::white
    ? [](const square& position) { return position; }
    : [](const square& position) { return get_rotated_square(position); }
  };
  const side white_side{
    left_player_color == chess_color::white
    ? side::lhs
    : side::rhs
  };
  const side black_side{get_other_side(white_side)};

  std::vector<piece> pieces{
    piece(chess_color::white, piece_type::rook,   f(square("a1")), white_side),
    piece(chess_color::white, piece_type::knight, f(square("b1")), white_side),
    piece(chess_color::white, piece_type::bishop, f(square("c1")), white_side),
    piece(chess_color::white, piece_type::queen,  f(square("d1")), white_side),
    piece(chess_color::white, piece_type::king,   f(square("e1")), white_side),
    piece(chess_color::white, piece_type::bishop, f(square("f1")), white_side),
    piece(chess_color::white, piece_type::knight, f(square("g1")), white_side),
    piece(chess_color::white, piece_type::rook,   f(square("h1")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("a2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("b2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("c2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("d2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("e2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("f2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("g2")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("h2")), white_side),
    piece(chess_color::black, piece_type::rook,   f(square("a8")), black_side),
    piece(chess_color::black, piece_type::knight, f(square("b8")), black_side),
    piece(chess_color::black, piece_type::bishop, f(square("c8")), black_side),
    piece(chess_color::black, piece_type::queen,  f(square("d8")), black_side),
    piece(chess_color::black, piece_type::king,   f(square("e8")), black_side),
    piece(chess_color::black, piece_type::bishop, f(square("f8")), black_side),
    piece(chess_color::black, piece_type::knight, f(square("g8")), black_side),
    piece(chess_color::black, piece_type::rook,   f(square("h8")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("a7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("b7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("c7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("d7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("e7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("f7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("g7")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("h7")), black_side)
  };
  return pieces;
}

std::vector<piece> get_starting_pieces(
  const starting_position_type t,
  const chess_color left_player_color
) noexcept
{
  switch (t)
  {
    case starting_position_type::standard: return get_standard_starting_pieces(left_player_color);
    case starting_position_type::kings_only: return get_kings_only_starting_pieces(left_player_color);
    case starting_position_type::before_scholars_mate: return get_pieces_before_scholars_mate(left_player_color);
    default:
    case starting_position_type::bishop_and_knight_end_game:
      assert(t == starting_position_type::bishop_and_knight_end_game);
      return get_pieces_bishop_and_knight_end_game(left_player_color);
  }
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
    [coordinat](const auto& piece) { return piece.get_coordinat() == coordinat; }
  );
  return iter != std::end(pieces);
}

void test_pieces()
{
#ifndef NDEBUG
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

  // is_piece_at, const
  {
    const auto pieces{get_standard_starting_pieces()};
    assert(is_piece_at(pieces, square("d1")));
  }
#endif
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
