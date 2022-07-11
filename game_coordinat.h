#ifndef GAME_COORDINAT_H
#define GAME_COORDINAT_H

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


game_coordinat operator-(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat operator+(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;

double calc_distance(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;

#endif // GAME_COORDINAT_H
