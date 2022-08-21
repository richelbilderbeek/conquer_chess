#include "square.h"

#include "game_coordinat.h"
#include "game_rect.h"
#include "helper.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <regex>
#include <sstream>

square::square(const std::string& pos)
{
  assert(pos.size() == 2);
  assert(std::regex_match(pos, std::regex("^[a-h][1-8]$")));
  m_y = pos[0] - 'a';
  m_x = pos[1] - '1';
  assert(is_valid_square_xy(m_x, m_y));
}

square::square(const game_coordinat& g)
  : m_x{static_cast<int>(std::trunc(g.get_x()))},
    m_y{static_cast<int>(std::trunc(g.get_y()))}
{
  assert(is_valid_square_xy(m_x, m_y));
}

square::square(const int x, const int y)
  : m_x{x}, m_y{y}
{
  assert(is_valid_square_xy(m_x, m_y));
}

bool are_adjacent(const square& a, const square& b) noexcept
{
  return std::abs(static_cast<int>(a.get_x() - b.get_x())) <= 1
    && std::abs(static_cast<int>(a.get_y() - b.get_y())) <= 1
  ;
}
bool are_adjacent_for_knight(const square& a, const square& b) noexcept
{
  const int dx{std::abs(static_cast<int>(a.get_x() - b.get_x()))};
  const int dy{std::abs(static_cast<int>(a.get_y() - b.get_y()))};
  return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

bool are_all_unique(std::vector<square> squares)
{
  assert(!squares.empty());
  if (squares.size() == 1) return true;
  std::sort(std::begin(squares), std::end(squares));
  return std::unique(std::begin(squares), std::end(squares))
    == std::end(squares)
  ;
}

bool are_on_adjacent_diagonal(const square& a, const square& b) noexcept
{
  return are_on_same_diagonal(a, b)
    && std::abs(static_cast<int>(a.get_x() - b.get_x())) == 1
  ;
}

bool are_on_same_diagonal(const square& a, const square& b) noexcept
{
  return std::abs(a.get_x() - b.get_x())
   == std::abs(a.get_y() - b.get_y())
  ;
}

bool are_on_same_file(const square& a, const square& b) noexcept
{
  return a.get_y() == b.get_y();
}

bool are_on_same_half_diagonal(const square& a, const square& b) noexcept
{
  const int dx{std::abs(a.get_x() - b.get_x())};
  const int dy{std::abs(a.get_y() - b.get_y())};
  return dx == dy * 2 || dy == dx * 2;
}

bool are_on_same_rank(const square& a, const square& b) noexcept
{
  return a.get_x() == b.get_x();
}

std::vector<std::pair<int, int>> collect_all_bishop_delta_pairs() noexcept
{
  return {
    std::make_pair( 1, -1), // NE
    std::make_pair( 1,  1), // SE
    std::make_pair(-1,  1), // SW
    std::make_pair(-1, -1)  // SE
  };
}

std::vector<std::vector<square>> collect_all_bishop_target_squares(const square& s) noexcept
{
  std::vector<std::vector<square>> targetses; // Reduplicated plural
  const auto x{s.get_x()};
  const auto y{s.get_y()};

  const std::vector<std::pair<int, int>> delta_pairs{
    collect_all_bishop_delta_pairs()
  };
  for (const auto& delta_pair: delta_pairs)
  {
    std::vector<square> targets;
    for (int distance{1}; distance != 8; ++distance)
    {
      const int new_x{x + (delta_pair.first * distance)};
      const int new_y{y + (delta_pair.second * distance)};
      if (is_valid_square_xy(new_x, new_y))
      {
        targets.push_back(square(new_x, new_y));
      }
    }
    targetses.push_back(targets);
  }
  return targetses;
}

std::vector<square> collect_all_king_target_squares(const square& s) noexcept
{
  std::vector<square> targets;
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
    const int x{s.get_x() + delta_pair.first};
    const int y{s.get_y() + delta_pair.second};
    if (is_valid_square_xy(x, y))
    {
      targets.push_back(square(x, y));
    }
  }
  assert(!targets.empty());
  return targets;
}

std::vector<std::pair<int, int>> collect_all_knight_delta_pairs() noexcept
{
  return {
    std::make_pair( 1, -2), // 1 o'clock
    std::make_pair( 2, -1), // 2 o'clock
    std::make_pair( 2,  1), // 4 o'clock
    std::make_pair( 1,  2), // 5 o'clock
    std::make_pair(-1,  2), // 7 o'clock
    std::make_pair(-2,  1), // 8 o'clock
    std::make_pair(-2, -1), // 10 o'clock
    std::make_pair(-1, -2)  // 11 o'clock
  };
}


std::vector<square> collect_all_knight_target_squares(const square& s) noexcept
{
  std::vector<square> targets;
  const std::vector<std::pair<int, int>> delta_pairs{
    collect_all_knight_delta_pairs()
  };
  for (const auto& delta_pair: delta_pairs)
  {
    const int x{s.get_x() + delta_pair.first};
    const int y{s.get_y() + delta_pair.second};
    if (is_valid_square_xy(x, y))
    {
      targets.push_back(square(x, y));
    }
  }
  assert(!targets.empty());
  return targets;
}

std::vector<std::pair<int, int>> collect_all_queen_delta_pairs() noexcept
{
  return {
    std::make_pair( 0, -1), // N
    std::make_pair( 1, -1), // NE
    std::make_pair( 1,  0), // E
    std::make_pair( 1,  1), // SE
    std::make_pair( 0,  1), // S
    std::make_pair(-1,  1), // SW
    std::make_pair(-1,  0), // W
    std::make_pair(-1, -1)  // SE
  };
}

std::vector<std::vector<square>> collect_all_queen_target_squares(const square& s) noexcept
{
  std::vector<std::vector<square>> targetses; // Reduplicated plural
  const auto x{s.get_x()};
  const auto y{s.get_y()};

  const std::vector<std::pair<int, int>> delta_pairs{
    collect_all_queen_delta_pairs()
  };
  for (const auto& delta_pair: delta_pairs)
  {
    std::vector<square> targets;
    for (int distance{1}; distance != 8; ++distance)
    {
      const int new_x{x + (delta_pair.first * distance)};
      const int new_y{y + (delta_pair.second * distance)};
      if (is_valid_square_xy(new_x, new_y))
      {
        targets.push_back(square(new_x, new_y));
      }
    }
    targetses.push_back(targets);
  }
  return targetses;
}

std::vector<std::pair<int, int>> collect_all_rook_delta_pairs() noexcept
{
  return {
    std::make_pair( 0, -1), // N
    std::make_pair( 1,  0), // E
    std::make_pair( 0,  1), // S
    std::make_pair(-1,  0)  // W
  };
}

std::vector<std::vector<square>> collect_all_rook_target_squares(const square& s) noexcept
{
  std::vector<std::vector<square>> targetses; // Reduplicated plural
  const auto x{s.get_x()};
  const auto y{s.get_y()};

  const std::vector<std::pair<int, int>> delta_pairs{
    collect_all_rook_delta_pairs()
  };
  for (const auto& delta_pair: delta_pairs)
  {
    std::vector<square> targets;
    for (int distance{1}; distance != 8; ++distance)
    {
      const int new_x{x + (delta_pair.first * distance)};
      const int new_y{y + (delta_pair.second * distance)};
      if (is_valid_square_xy(new_x, new_y))
      {
        targets.push_back(square(new_x, new_y));
      }
    }
    targetses.push_back(targets);
  }
  return targetses;
}

std::vector<square> concatenate(
  const std::vector<square>& a,
  const std::vector<square>& b
)
{
  std::vector<square> c = a;
  c.reserve(a.size() + b.size());
  std::copy(std::begin(b), std::end(b), std::back_inserter(c));
  assert(c.size() == a.size() + b.size());
  return c;
}

square create_random_square(
  std::default_random_engine& rng_engine
)
{
  std::uniform_int_distribution<int> distribution{0, 8 - 1}; // -1 as inclusive
  const auto x{distribution(rng_engine)};
  assert(x >= 0);
  assert(x < 8);
  const auto y{distribution(rng_engine)};
  assert(y >= 0);
  assert(y < 8);
  return square(x, y);
}

char get_file(const square& s) noexcept
{
  return 'a' + s.get_y();
}

std::vector<square> get_intermediate_squares(
  const square& from,
  const square& to
)
{
  assert(from != to);
  std::vector<square> squares;
  const int dx{std::abs(to.get_x() - from.get_x())};
  const int dy{std::abs(to.get_y() - from.get_y())};
  // Horizontal
  if (dy == 0)
  {
    const int y{from.get_y()};
    const std::vector<int> xs{
      make_sequence(from.get_x(), to.get_x())
    };
    squares.reserve(xs.size());
    std::transform(
      std::begin(xs),
      std::end(xs),
      std::back_inserter(squares),
      [y](const int x) { return square(x, y); }
    );
  }
  else if (dx == 0)
  {
    const int x{from.get_x()};
    const std::vector<int> ys{
      make_sequence(from.get_y(), to.get_y())
    };
    squares.reserve(ys.size());
    std::transform(
      std::begin(ys),
      std::end(ys),
      std::back_inserter(squares),
      [x](const int y) { return square(x, y); }
    );

  }
  else if (dx == dy)
  {
    const std::vector<int> xs{
      make_sequence(from.get_x(), to.get_x())
    };
    const std::vector<int> ys{
      make_sequence(from.get_y(), to.get_y())
    };
    assert(xs.size() == ys.size());
    const int n{static_cast<int>(xs.size())};
    squares.reserve(ys.size());
    for (int i{0}; i != n; ++i)
    {
      squares.push_back(square(xs[i], ys[i]));
    }
  }
  else
  {
    assert(dx == 2 * dy || dy == 2 * dx);
    const int x_inc{dx > dy ? 2 : 1};
    const int y_inc{dx > dy ? 1 : 2};
    const std::vector<int> xs{
      make_sequence(from.get_x(), to.get_x(), x_inc)
    };
    const std::vector<int> ys{
      make_sequence(from.get_y(), to.get_y(), y_inc)
    };
    assert(xs.size() == ys.size());
    const int n{static_cast<int>(xs.size())};
    squares.reserve(ys.size());
    for (int i{0}; i != n; ++i)
    {
      squares.push_back(square(xs[i], ys[i]));
    }
  }
  assert(squares.size() >= 2);
  assert(squares.front() == from);
  assert(squares.back() == to);
  return squares;
}

int get_rank(const square& s) noexcept
{
  return 1 + s.get_x();
}

square get_rotated_square(const square& position) noexcept
{
  return square(
    7 - position.get_x(),
    7 - position.get_y()
  );
}

bool is_occupied(
  const square& s,
  const std::vector<square>& occupied_squares
) noexcept
{
  return std::find(
    std::begin(occupied_squares),
    std::end(occupied_squares),
    s
  ) != std::end(occupied_squares);
}

bool is_valid_square_xy(const int x, const int y) noexcept
{
  return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool is_invalid_square_xy(const int x, const int y) noexcept
{
  return x < 0
    || x > 7
    || y < 0
    || y > 7
  ;
}

void test_square()
{
#ifndef NDEBUG
  // square::get_x
  {
    const std::string pos("a2");
    const square s(pos);
    assert(s.get_x() == 1);
  }
  // square::get_y
  {
    const std::string pos("a2");
    const square s(pos);
    assert(s.get_y() == 0);
  }
  // are_on_adjacent_diagonal
  {
    assert(are_on_adjacent_diagonal(square("d1"), square("c2")));
    assert(are_on_adjacent_diagonal(square("c2"), square("b1")));
    assert(are_on_adjacent_diagonal(square("c2"), square("b3")));
    assert(are_on_adjacent_diagonal(square("c2"), square("d1")));
    assert(are_on_adjacent_diagonal(square("c2"), square("d3")));
    assert(!are_on_adjacent_diagonal(square("d1"), square("a4")));
  }
  // are_on_same_diagonal
  {
    assert(are_on_same_diagonal(square("d1"), square("a4")));
  }
  // are_on_same_file
  {
    assert(are_on_same_file(square("e2"), square("e4")));
  }
  // are_on_same_half_diagonal
  {
    assert(are_on_same_half_diagonal(square("b1"), square("c3")));
  }
  // are_on_same_rank
  {
    assert(are_on_same_rank(square("a1"), square("h1")));
  }
  // create_random_square
  {
    const int seed{314};
    std::default_random_engine rng_engine(seed);
    for (int i{0}; i!=100; ++i)
    {
      create_random_square(rng_engine);
    }
  }
  // get_file
  {
    assert(get_file(square("a1")) == 'a');
    assert(get_file(square("b2")) == 'b');
    assert(get_file(square("g3")) == 'g');
    assert(get_file(square("h4")) == 'h');
  }
  // get_intermediate_squares
  {
    // Horizontally
    assert(get_intermediate_squares(square("a1"), square("a2")).size() == 2);
    assert(get_intermediate_squares(square("b1"), square("b3")).size() == 3);
    assert(get_intermediate_squares(square("c1"), square("c8")).size() == 8);
    assert(get_intermediate_squares(square("d8"), square("d1")).size() == 8);
    assert(get_intermediate_squares(square("e7"), square("e2")).size() == 6);

    // Vertically
    assert(get_intermediate_squares(square("a1"), square("b1")).size() == 2);
    assert(get_intermediate_squares(square("a1"), square("c1")).size() == 3);
    assert(get_intermediate_squares(square("a1"), square("h1")).size() == 8);
    assert(get_intermediate_squares(square("h1"), square("a1")).size() == 8);
    assert(get_intermediate_squares(square("g1"), square("b1")).size() == 6);

    // Diagonally, accent grave
    assert(get_intermediate_squares(square("a1"), square("b2")).size() == 2);
    assert(get_intermediate_squares(square("a1"), square("c3")).size() == 3);
    assert(get_intermediate_squares(square("a1"), square("h8")).size() == 8);
    assert(get_intermediate_squares(square("h8"), square("a1")).size() == 8);
    assert(get_intermediate_squares(square("g7"), square("b2")).size() == 6);

    // Diagonally, accent aigu
    assert(get_intermediate_squares(square("h1"), square("g2")).size() == 2);
    assert(get_intermediate_squares(square("h1"), square("f3")).size() == 3);
    assert(get_intermediate_squares(square("h1"), square("a8")).size() == 8);
    assert(get_intermediate_squares(square("a8"), square("h1")).size() == 8);
    assert(get_intermediate_squares(square("b7"), square("g2")).size() == 6);

    // Knight jumps
    // 1 o'clock
    assert(get_intermediate_squares(square("h1"), square("f2")).size() == 2);
    assert(get_intermediate_squares(square("h1"), square("d3")).size() == 3);
    assert(get_intermediate_squares(square("h1"), square("b4")).size() == 4);

    // 2 o'clock
    assert(get_intermediate_squares(square("d1"), square("c3")).size() == 2);
    assert(get_intermediate_squares(square("d1"), square("b5")).size() == 3);
    assert(get_intermediate_squares(square("d1"), square("a7")).size() == 4);

    // 4 o'clock
    assert(get_intermediate_squares(square("a1"), square("b3")).size() == 2);
    assert(get_intermediate_squares(square("a1"), square("c5")).size() == 3);
    assert(get_intermediate_squares(square("a1"), square("d7")).size() == 4);

    // 5 o'clock
    assert(get_intermediate_squares(square("a1"), square("c2")).size() == 2);
    assert(get_intermediate_squares(square("a1"), square("e3")).size() == 3);
    assert(get_intermediate_squares(square("a1"), square("g4")).size() == 4);

    // 7 o'clock
    assert(get_intermediate_squares(square("f2"), square("h1")).size() == 2);
    assert(get_intermediate_squares(square("d3"), square("h1")).size() == 3);
    assert(get_intermediate_squares(square("b4"), square("h1")).size() == 4);
  }
  // get_rotated_square
  {
    assert(get_rotated_square(square("a1")) == square("h8"));
    assert(get_rotated_square(square("a8")) == square("h1"));
    assert(get_rotated_square(square("h8")) == square("a1"));
    assert(get_rotated_square(square("h1")) == square("a8"));
  }
  // get_rank
  {
    assert(get_rank(square("a1")) == 1);
    assert(get_rank(square("b2")) == 2);
    assert(get_rank(square("g7")) == 7);
    assert(get_rank(square("h8")) == 8);
  }
  // to_color
  {
    assert(to_color(square("a1")) == chess_color::black);
    assert(to_color(square("d1")) == chess_color::white);
    assert(to_color(square("d8")) == chess_color::black);
  }
  // to_coordinat
  {
    assert(to_coordinat(square("a1")) == game_coordinat(0.5, 0.5));
    const auto a8{to_coordinat(square("a8"))};
    assert(a8 == game_coordinat(7.5, 0.5));
    assert(to_coordinat(square("a8")) == game_coordinat(7.5, 0.5));
    assert(to_coordinat(square("h1")) == game_coordinat(0.5, 7.5));
    assert(to_coordinat(square("h8")) == game_coordinat(7.5, 7.5));
  }
  // to_game_rect
  {
    const auto a1_created{to_game_rect(square("a1"))};
    const auto a1_expected{game_rect(game_coordinat(0.0, 0.0), game_coordinat(1.0, 1.0))};
    assert(a1_created == a1_expected);

    const auto a8_created{to_game_rect(square("a8"))};
    const auto a8_expected{game_rect(game_coordinat(7.0, 0.0), game_coordinat(8.0, 1.0))};
    assert(a8_created == a8_expected);
  }
  // to_str, one square
  {
    const std::string pos("a2");
    const square s(pos);
    assert(to_str(s) == pos);
  }
  // to_str, std::vector<square>
  {
    const std::vector<square> squares{square("a1"), square("b2")};
    assert(!to_str(squares).empty());
  }
  // operator==
  {
    const square a("a1");
    const square b("a1");
    const square c("b1");
    assert(a == b);
    assert(!(a == c));
  }
  // operator!=
  {
    const square a("a1");
    const square b("a1");
    const square c("b1");
    assert(!(a != b));
    assert(a != c);
  }
  // operator<< on square
  {
    const square a1("a1");
    std::stringstream s;
    s << a1;
    assert(!s.str().empty());
  }
  // operator<< on std::vector<square>
  {
    const std::vector<square> squares{
      square("a1"), square("a2")
    };
    std::stringstream s;
    s << squares;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

chess_color to_color(const square& s) noexcept
{
  if ((s.get_x() + s.get_y()) % 2 == 0) return chess_color::black;
  return chess_color::white;
}

game_coordinat to_coordinat(const square& s) noexcept
{
  return game_coordinat(
    0.5 + static_cast<double>(s.get_x()),
    0.5 + static_cast<double>(s.get_y())
  );
}

game_coordinat to_coordinat(const std::string& pos) noexcept
{
  return to_coordinat(square(pos));
}

game_rect to_game_rect(const square& s) noexcept
{
  const game_coordinat mid{to_coordinat(s)};
  const game_coordinat tl{
    std::trunc(mid.get_x()),
    std::trunc(mid.get_y())
  };
  const game_coordinat br{
    tl + game_coordinat(1.0, 1.0)
  };
  return game_rect(tl, br);
}

std::vector<square> to_squares(std::vector<std::pair<int, int>> xys)
{
  const auto new_end{
    std::remove_if(
      std::begin(xys),
      std::end(xys),
      [](const auto& p)
      {
        return is_invalid_square_xy(p.first, p.second);
      }
    )
  };
  xys.erase(new_end, std::end(xys));
  std::vector<square> squares;
  squares.reserve(xys.size());
  std::transform(
    std::begin(xys),
    std::end(xys),
    std::back_inserter(squares),
    [](const auto& p)
    {
      return square(p.first, p.second);
    }
  );
  return squares;
}

std::string to_str(const square& s) noexcept
{
  std::stringstream st;
  const char c1 = 'a' + s.get_y();
  const char c2 = '1' + static_cast<char>(s.get_x());
  st << c1 << c2;
  return st.str();

}

std::string to_str(const std::vector<square>& squares) noexcept
{
  if (squares.empty()) return "";
  std::stringstream s;
  std::transform(
    std::begin(squares),
    std::end(squares),
    std::ostream_iterator<std::string>(s, ", "),
    [](const square& sq) { return to_str(sq); }
  );
  std::string t = s.str();
  t.pop_back();
  t.pop_back();
  return t;
}

bool operator==(const square& lhs, const square& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x()
    && lhs.get_y() == rhs.get_y()
  ;
}

bool operator!=(const square& lhs, const square& rhs) noexcept
{
  return !(lhs == rhs);
}

bool operator<(const square& lhs, const square& rhs) noexcept
{
  if (lhs.get_x() < rhs.get_x()) return true;
  if (lhs.get_x() > rhs.get_x()) return false;
  return lhs.get_y() < rhs.get_y();
}

std::ostream& operator<<(std::ostream& os, const square& s) noexcept
{
  os << to_str(s);
  return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<square>& s) noexcept
{
  os << to_str(s);
  return os;
}
