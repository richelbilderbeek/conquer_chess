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

