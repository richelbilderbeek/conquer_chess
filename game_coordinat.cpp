#include "game_coordinat.h"

#include "helper.h"
#include "square.h"

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


double calc_distance(const game_coordinat& lhs, const game_coordinat& rhs) noexcept
{
  const double dx{lhs.get_x() - rhs.get_x()};
  const double dy{lhs.get_y() - rhs.get_y()};
  return calc_distance(dx, dy);
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
  const square& from,
  const square& to,
  const side player
)
{
  const bool is_right{to.get_x() > from.get_x()};
  return (player == side::rhs && !is_right)
    || (player == side::lhs && is_right)
  ;
}

void test_game_coordinat()
{
  #ifndef NDEBUG
  // calc_length
  {
    const game_coordinat c(3.0, 4.0);
    assert(is_close(calc_length(c), 5.0, 0.01));
  }
  // center_on_center
  {
    const game_coordinat c(3.2, 4.7);
    const game_coordinat expected(3.5, 4.5);
    const game_coordinat created(center_on_center(c));
    assert(expected == created);
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
  // to_notation
  {
    assert(to_notation(game_coordinat(0.5, 0.5)) == "a1");
    assert(to_notation(game_coordinat(9.5, 9.5)) == "--");
  }
  // operator ==
  {
    const game_coordinat a(1.2345, 6.7890);
    const game_coordinat b(1.2345, 6.7890);
    const game_coordinat c(0.0, 0.0);
    assert(a == b);
    assert(!(a == c));
  }
  // operator+
  {
    const game_coordinat a(1.1, 2.2);
    const game_coordinat b(3.3, 4.4);
    const game_coordinat c{a + b};
    assert(c.get_x() == a.get_x() + b.get_x());
    assert(c.get_y() == a.get_y() + b.get_y());
  }
  // operator-
  {
    const game_coordinat a(1.23, 4.56);
    const game_coordinat b(7.89, 0.12);
    const game_coordinat c{a - b};
    assert(c.get_x() == a.get_x() - b.get_x());
    assert(c.get_y() == a.get_y() - b.get_y());
  }
  // operator !=
  {
    const game_coordinat a(1.2345, 6.7890);
    const game_coordinat b(1.2345, 6.7890);
    const game_coordinat c(0.0, 0.0);
    assert(!(a != b));
    assert(a != c);
  }
  // operator*
  {
    const game_coordinat a(1.2345, 6.7890);
    const double factor{1.23};
    const auto b{a * factor};
    assert(b.get_x() == a.get_x() * factor);
    assert(b.get_y() == a.get_y() * factor);
  }
  // operator/
  {
    const game_coordinat a(1.2345, 6.7890);
    const double factor{2.34};
    const auto b{a / factor};
    assert(b.get_x() == a.get_x() / factor);
    assert(b.get_y() == a.get_y() / factor);
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
  if (g.get_x() >= 0.0
    && g.get_x() < 8.0
    && g.get_y() >= 0.0
    && g.get_y() < 8.0
  )
  {
    return to_str(square(g));
  }
  return "--";
}

bool operator==(const game_coordinat& lhs, const game_coordinat& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x()
    && lhs.get_y() == rhs.get_y()
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

game_coordinat operator*(const game_coordinat& c, const double factor) noexcept
{
  return game_coordinat(
    c.get_x() * factor,
    c.get_y() * factor
  );
}
