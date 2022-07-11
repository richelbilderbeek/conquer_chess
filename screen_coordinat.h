#ifndef SCREEN_COORDINAT_H
#define SCREEN_COORDINAT_H


/// Coordinats on the screen.
/// @see game_coordinat for the coordinats in the game
class screen_coordinat
{
public:
  screen_coordinat(const int x, const int y);

private:

  int m_x;
  int m_y;
};

#endif // SCREEN_COORDINAT_H
