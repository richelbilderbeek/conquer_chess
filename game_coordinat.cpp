#include "game_coordinat.h"

#include "helper.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

game_coordinat::game_coordinat(
  const double x,
  const double y
) : m_x{x}, m_y{y}
{

}

bool are_on_same_diagonal(const game_coordinat& a, const game_coordinat& b) noexcept
{
  return std::abs(static_cast<int>(to_notation(a)[0] - to_notation(b)[0]))
   == std::abs(static_cast<int>(to_notation(a)[1] - to_notation(b)[1]))
  ;
}

bool are_on_same_file(const game_coordinat& a, const game_coordinat& b) noexcept
{
  return to_notation(a)[0] == to_notation(b)[0];
}

bool are_on_same_half_diagonal(const game_coordinat& a, const game_coordinat& b) noexcept
{
  const int dx{
    std::abs(static_cast<int>(to_notation(a)[0] - to_notation(b)[0]))
  };
  const int dy{
     std::abs(static_cast<int>(to_notation(a)[1] - to_notation(b)[1]))
  };
  return dx == dy * 2 || dy == dx * 2;
}

bool are_on_same_rank(const game_coordinat& a, const game_coordinat& b) noexcept
{
  return to_notation(a)[1] == to_notation(b)[1];

}

double calc_distance(const game_coordinat& lhs, const game_coordinat& rhs) noexcept
{
  const double dx{lhs.get_x() - rhs.get_x()};
  const double dy{lhs.get_y() - rhs.get_y()};
  return std::sqrt((dx * dx) + (dy * dy));
}

double calc_length(const game_coordinat& coordinat) noexcept
{
  return calc_distance(
    coordinat,
    game_coordinat()
  );
}

game_coordinat center_on_center(const game_coordinat& coordinat)
{
  return game_coordinat(
    std::trunc(coordinat.get_x()) + 0.5,
    std::trunc(coordinat.get_y()) + 0.5
  );
}

game_coordinat get_above(const game_coordinat& coordinat) noexcept
{
  game_coordinat pos{coordinat + game_coordinat(0.0, -1.0)};
  if (pos.get_y() < 0.0)
  {
    pos += game_coordinat(0.0, 8.0);
  }
  return pos;
}

game_coordinat get_below(const game_coordinat& coordinat) noexcept
{
  game_coordinat pos{coordinat + game_coordinat(0.0, 1.0)};
  if (pos.get_y() > 8.0)
  {
    pos += game_coordinat(0.0, -8.0);
  }
  return pos;
}

game_coordinat get_coordinat(const std::string& notation)
{
  assert(notation.size() == 2);
  const int x_int{notation[1] - '1'};
  const int y_int{notation[0] - 'a'};
  return game_coordinat(
    0.5 + static_cast<double>(x_int),
    0.5 + static_cast<double>(y_int)
  );
}

game_coordinat get_left(const game_coordinat& coordinat) noexcept
{
  game_coordinat pos{coordinat + game_coordinat(-1.0, 0.0)};
  if (pos.get_x() < 0.0)
  {
    pos += game_coordinat(8.0, 0.0);
  }
  return pos;
}

game_coordinat get_right(const game_coordinat& coordinat) noexcept
{
  game_coordinat pos{coordinat + game_coordinat(1.0, 0.0)};
  if (pos.get_x() > 8.0)
  {
    pos += game_coordinat(-8.0, 0.0);
  }
  return pos;
}

game_coordinat get_rotated_coordinat(const game_coordinat& coordinat) noexcept
{
  return game_coordinat(
    8.0 - coordinat.get_x(),
    8.0 - coordinat.get_y()
  );
}

bool is_forward(
  const game_coordinat& from,
  const game_coordinat& to,
  const side player
)
{
  const bool is_right{to_notation(to)[1] > to_notation(from)[1]};
  return (player == side::rhs && !is_right)
    || (player == side::lhs && is_right)
  ;
}

void test_game_coordinat()
{
  #ifndef NDEBUG
  // are_on_same_diagonal
  {
    assert(are_on_same_diagonal(get_coordinat("d1"), get_coordinat("a4")));
  }
  // are_on_same_file
  {
    assert(are_on_same_file(get_coordinat("e2"), get_coordinat("e4")));
  }
  // are_on_same_half_diagonal
  {
    assert(are_on_same_half_diagonal(get_coordinat("b1"), get_coordinat("c3")));
  }
  // are_on_same_rank
  {
    assert(are_on_same_rank(get_coordinat("a1"), get_coordinat("h1")));
  }
  // get
  {
    const double x{12.34};
    const double y{23.45};
    const game_coordinat c(x, y);
    assert(c.get_x() == x);
    assert(c.get_y() == y);
  }
  // get_above
  {
    const game_coordinat c(3.5, 3.5);
    const auto above{get_above(c)};
    assert(above.get_y() < c.get_y());
  }
  // get_above loops
  {
    const game_coordinat c(3.5, 0.5);
    const auto above{get_above(c)};
    assert(above.get_y() > c.get_y());
  }
  // get_below
  {
    const game_coordinat c(3.5, 3.5);
    const auto below{get_below(c)};
    assert(below.get_y() > c.get_y());
  }
  // get_coordinat
  {
    assert(get_coordinat("a1") == game_coordinat(0.5, 0.5));
    const auto a8{get_coordinat("a8")};
    assert(a8 == game_coordinat(7.5, 0.5));
    assert(get_coordinat("a8") == game_coordinat(7.5, 0.5));
    assert(get_coordinat("h1") == game_coordinat(0.5, 7.5));
    assert(get_coordinat("h8") == game_coordinat(7.5, 7.5));
  }
  // get_below loops
  {
    const game_coordinat c(3.5, 7.5);
    const auto below{get_below(c)};
    assert(below.get_y() < c.get_y());
  }
  // get_left
  {
    const game_coordinat c(3.5, 3.5);
    const auto left{get_left(c)};
    assert(left.get_x() < c.get_x());
  }
  // get_left loops
  {
    const game_coordinat c(0.5, 3.5);
    const auto left{get_left(c)};
    assert(left.get_x() > c.get_x());
  }
  // get_right
  {
    const game_coordinat c(3.5, 3.5);
    const auto right{get_right(c)};
    assert(right.get_x() > c.get_x());
  }
  // get_right loops
  {
    const game_coordinat c(7.5, 3.5);
    const auto right{get_right(c)};
    assert(right.get_x() < c.get_x());
  }
  // get_rotated_coordinat
  {
    const game_coordinat c(1.5, 2.5);
    const auto rotated{get_rotated_coordinat(c)};
    assert(is_close(rotated.get_x(), 6.5, 0.1));
    assert(is_close(rotated.get_y(), 5.5, 0.1));
  }
  // operator ==
  {
    const game_coordinat a(1.2345, 6.7890);
    const game_coordinat b(1.2345, 6.7890);
    const game_coordinat c(0.0, 0.0);
    assert(a == b);
    assert(!(a == c));
  }
  // operator !=
  {
    const game_coordinat a(1.2345, 6.7890);
    const game_coordinat b(1.2345, 6.7890);
    const game_coordinat c(0.0, 0.0);
    assert(!(a != b));
    assert(a != c);
  }
  // operator<<
  {
    std::stringstream s;
    const game_coordinat c(1.2345, 6.7890);
    s << c;
    const std::string expected{"(1.2, 6.8)"};
    const std::string created{s.str()};
    assert(expected == created);
  }
  #endif // NDEBUG
}

std::string to_notation(const game_coordinat& g)
{
  const int x{static_cast<int>(std::trunc(g.get_x()))};
  const int y{static_cast<int>(std::trunc(g.get_y()))};
  if (x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    const char first = 'a' + y;
    const char second = '1' + x;
    std::string s;
    s += first;
    s += second;
    return s;
  }
  else
  {
    return "--";
  }
}

bool operator==(const game_coordinat& lhs, const game_coordinat& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x()
    && rhs.get_y() == rhs.get_y()
  ;
}

bool operator!=(const game_coordinat& lhs, const game_coordinat& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const game_coordinat& coordinat)
{
  const double x{std::round(coordinat.get_x() * 10.0) / 10.0};
  const double y{std::round(coordinat.get_y() * 10.0) / 10.0};
  os << "(" << x << ", "  << y << ")";
  return os;
}

game_coordinat operator-(const game_coordinat& lhs, const game_coordinat& rhs) noexcept
{
  return game_coordinat(
    lhs.get_x() - rhs.get_x(),
    lhs.get_y() - rhs.get_y()
  );
}

game_coordinat operator+(const game_coordinat& lhs, const game_coordinat& rhs) noexcept
{
  return game_coordinat(
    lhs.get_x() + rhs.get_x(),
    lhs.get_y() + rhs.get_y()
  );
}

game_coordinat& operator+=(game_coordinat& lhs, const game_coordinat& rhs) noexcept
{
  lhs = game_coordinat(lhs + rhs);
  return lhs;
}

game_coordinat operator/(const game_coordinat& coordinat, const double factor) noexcept
{
  return game_coordinat(
    coordinat.get_x() / factor,
    coordinat.get_y() / factor
  );

}

