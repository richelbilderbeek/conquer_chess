#ifndef GAME_COORDINAT_H
#define GAME_COORDINAT_H

#include <iosfwd>

/// Coordinat on the board
/// @see use screen_coordinat for a coordinat on the screen
class game_coordinat
{
public:
  game_coordinat(const double x = 0, const double y = 0);

  double get_x() const noexcept { return m_x; }
  double get_y() const noexcept { return m_y; }

private:

  double m_x;
  double m_y;
};


/// Calculate the distance between two points
double calc_distance(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;

void test_game_coordinat();

std::ostream& operator<<(std::ostream& os, const game_coordinat& coordinat);

game_coordinat operator-(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat operator+(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;


#endif // GAME_COORDINAT_H
