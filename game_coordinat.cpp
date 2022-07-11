#include "game_coordinat.h"

#include <cmath>
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
