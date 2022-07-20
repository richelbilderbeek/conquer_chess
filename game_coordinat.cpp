#include "game_coordinat.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

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

void test_game_coordinat()
{
  #ifndef NDEBUG
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

