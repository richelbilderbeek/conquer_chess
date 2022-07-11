#ifndef SCREEN_COORDINAT_H
#define SCREEN_COORDINAT_H


/// Coordinats on the screen.
/// @see game_coordinat for the coordinats in the game
class screen_coordinat
{
public:
  screen_coordinat(const int x = 0, const int y = 0);

  int get_x() const noexcept { return m_x; }
  int get_y() const noexcept { return m_y; }

private:

  int m_x;
  int m_y;
};

#endif // SCREEN_COORDINAT_H
