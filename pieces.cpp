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

std::vector<square> get_occupied_squares(const std::vector<piece>& pieces) noexcept
{
  std::vector<square> squares;
  squares.reserve(pieces.size());
  for (const auto& p: pieces)
  {
    squares.push_back(p.get_current_square());
  }
  #ifndef NDEBUG
  std::sort(std::begin(squares), std::end(squares));
  assert(
    std::unique(std::begin(squares), std::end(squares))
    == std::end(squares)
  );
  #endif
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

std::vector<piece> get_pieces_pawn_all_out_assault(
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
    piece(chess_color::white, piece_type::pawn,   f(square("a4")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("b4")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("c4")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("d4")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("e4")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("f4")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("g4")), white_side),
    piece(chess_color::white, piece_type::pawn,   f(square("h4")), white_side),
    piece(chess_color::black, piece_type::rook,   f(square("a8")), black_side),
    piece(chess_color::black, piece_type::knight, f(square("b8")), black_side),
    piece(chess_color::black, piece_type::bishop, f(square("c8")), black_side),
    piece(chess_color::black, piece_type::queen,  f(square("d8")), black_side),
    piece(chess_color::black, piece_type::king,   f(square("e8")), black_side),
    piece(chess_color::black, piece_type::bishop, f(square("f8")), black_side),
    piece(chess_color::black, piece_type::knight, f(square("g8")), black_side),
    piece(chess_color::black, piece_type::rook,   f(square("h8")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("a5")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("b5")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("c5")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("d5")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("e5")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("f5")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("g5")), black_side),
    piece(chess_color::black, piece_type::pawn,   f(square("h5")), black_side)
  };
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

std::vector<piece> get_pieces_queen_endgame(
  const chess_color left_player_color
) noexcept
{
  const side white_side{
    left_player_color == chess_color::white
    ? side::lhs
    : side::rhs
  };
  const side black_side{get_other_side(white_side)};
  std::vector<piece> pieces{
    piece(chess_color::white, piece_type::queen,  square("d1"), white_side),
    piece(chess_color::white, piece_type::king,   square("e1"), white_side),
    piece(chess_color::black, piece_type::queen,  square("d8"), black_side),
    piece(chess_color::black, piece_type::king,   square("e8"), black_side)
  };
  if (left_player_color == chess_color::black)
  {
    pieces = get_rotated_pieces(pieces);
  }
  return pieces;
}

std::vector<piece> get_rotated_pieces(const std::vector<piece>& pieces) noexcept
{
  std::vector<piece> rps;
  rps.reserve(pieces.size());
  std::transform(
    std::begin(pieces),
    std::end(pieces),
    std::back_inserter(rps),
    [](const auto& p)
    {
      return get_rotated_piece(p);
    }
  );
  return rps;
}

std::vector<piece> get_standard_starting_pieces(
  const chess_color left_player_color
) noexcept
{
  const side white_side{
    left_player_color == chess_color::white
    ? side::lhs
    : side::rhs
  };
  const side black_side{get_other_side(white_side)};
  std::vector<piece> pieces{
    piece(chess_color::white, piece_type::rook,   square("a1"), white_side),
    piece(chess_color::white, piece_type::knight, square("b1"), white_side),
    piece(chess_color::white, piece_type::bishop, square("c1"), white_side),
    piece(chess_color::white, piece_type::queen,  square("d1"), white_side),
    piece(chess_color::white, piece_type::king,   square("e1"), white_side),
    piece(chess_color::white, piece_type::bishop, square("f1"), white_side),
    piece(chess_color::white, piece_type::knight, square("g1"), white_side),
    piece(chess_color::white, piece_type::rook,   square("h1"), white_side),
    piece(chess_color::white, piece_type::pawn,   square("a2"), white_side),
    piece(chess_color::white, piece_type::pawn,   square("b2"), white_side),
    piece(chess_color::white, piece_type::pawn,   square("c2"), white_side),
    piece(chess_color::white, piece_type::pawn,   square("d2"), white_side),
    piece(chess_color::white, piece_type::pawn,   square("e2"), white_side),
    piece(chess_color::white, piece_type::pawn,   square("f2"), white_side),
    piece(chess_color::white, piece_type::pawn,   square("g2"), white_side),
    piece(chess_color::white, piece_type::pawn,   square("h2"), white_side),
    piece(chess_color::black, piece_type::rook,   square("a8"), black_side),
    piece(chess_color::black, piece_type::knight, square("b8"), black_side),
    piece(chess_color::black, piece_type::bishop, square("c8"), black_side),
    piece(chess_color::black, piece_type::queen,  square("d8"), black_side),
    piece(chess_color::black, piece_type::king,   square("e8"), black_side),
    piece(chess_color::black, piece_type::bishop, square("f8"), black_side),
    piece(chess_color::black, piece_type::knight, square("g8"), black_side),
    piece(chess_color::black, piece_type::rook,   square("h8"), black_side),
    piece(chess_color::black, piece_type::pawn,   square("a7"), black_side),
    piece(chess_color::black, piece_type::pawn,   square("b7"), black_side),
    piece(chess_color::black, piece_type::pawn,   square("c7"), black_side),
    piece(chess_color::black, piece_type::pawn,   square("d7"), black_side),
    piece(chess_color::black, piece_type::pawn,   square("e7"), black_side),
    piece(chess_color::black, piece_type::pawn,   square("f7"), black_side),
    piece(chess_color::black, piece_type::pawn,   square("g7"), black_side),
    piece(chess_color::black, piece_type::pawn,   square("h7"), black_side)
  };
  if (left_player_color == chess_color::black)
  {
    pieces = get_rotated_pieces(pieces);
  }
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
    case starting_position_type::queen_end_game: return get_pieces_queen_endgame(left_player_color);
    case starting_position_type::bishop_and_knight_end_game:
      return get_pieces_bishop_and_knight_end_game(left_player_color);
    default:
    case starting_position_type::pawn_all_out_assault:
      assert(t == starting_position_type::pawn_all_out_assault);
      return get_pieces_pawn_all_out_assault(left_player_color);
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
  // get_standard_starting_pieces
  {
    const auto pieces_1{get_standard_starting_pieces(chess_color::white)};
    assert(pieces_1.size() == 32);
    const auto pieces_2{get_standard_starting_pieces(chess_color::black)};
    assert(pieces_1 != pieces_2);
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
