#include "square.h"

#include "game_coordinat.h"
#include "game_rect.h"
#include "helper.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <regex>

square::square(const std::string& pos)
  : m_pos{pos}
{
  assert(m_pos.size() == 2);
  assert(std::regex_match(m_pos, std::regex("^[a-h][1-8]$")));
}

square::square(const game_coordinat& g)
{
  const int x{static_cast<int>(std::trunc(g.get_x()))};
  const int y{static_cast<int>(std::trunc(g.get_y()))};
  assert(x >= 0 && x < 8 && y >= 0 && y < 8);
  const char first = 'a' + y;
  const char second = '1' + x;
  m_pos += first;
  m_pos += second;
  assert(std::regex_match(m_pos, std::regex("^[a-h][1-8]$")));
}

square::square(const int x, const int y)
{
  assert(x >= 0 && x < 8 && y >= 0 && y < 8);
  const char first = 'a' + y;
  const char second = '1' + x;
  m_pos += first;
  m_pos += second;
  assert(std::regex_match(m_pos, std::regex("^[a-h][1-8]$")));
}

bool are_on_same_diagonal(const square& a, const square& b) noexcept
{
  return std::abs(static_cast<int>(a.get_pos()[0] - b.get_pos()[0]))
   == std::abs(static_cast<int>(a.get_pos()[1] - b.get_pos()[1]))
  ;
}

bool are_on_same_file(const square& a, const square& b) noexcept
{
  return a.get_pos()[0] == b.get_pos()[0];
}

bool are_on_same_half_diagonal(const square& a, const square& b) noexcept
{
  const int dx{
    std::abs(static_cast<int>(a.get_pos()[0] - b.get_pos()[0]))
  };
  const int dy{
     std::abs(static_cast<int>(a.get_pos()[1] - b.get_pos()[1]))
  };
  return dx == dy * 2 || dy == dx * 2;
}

bool are_on_same_rank(const square& a, const square& b) noexcept
{
  return a.get_pos()[1] == b.get_pos()[1];
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

square get_rotated_square(const square& position) noexcept
{
  const char first_char{position.get_pos()[0]};
  const char second_char{position.get_pos()[1]};
  const char first_new_char = 'h' - (first_char - 'a');
  const char second_new_char = '8' - (second_char - '1');
  std::string s;
  s += first_new_char;
  s += second_new_char;
  return square(s);
}

int square::get_x() const
{
  assert(m_pos.size() == 2);
  const int x_int{m_pos[1] - '1'};
  //const int y_int{m_pos[0] - 'a'};
  return x_int;
}

int square::get_y() const
{
  assert(m_pos.size() == 2);
  //const int x_int{m_pos[1] - '1'};
  const int y_int{m_pos[0] - 'a'};
  return y_int;
}

void test_square()
{
#ifndef NDEBUG
  // square::get_pos
  {
    const std::string pos("a2");
    const square s(pos);
    assert(s.get_pos() == pos);
  }
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
#endif // NDEBUG
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

std::string to_str(const square& s) noexcept
{
  return s.get_pos();
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
  return lhs.get_pos() == rhs.get_pos();
}

bool operator!=(const square& lhs, const square& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const square& s) noexcept
{
  os << to_str(s);
  return os;
}
