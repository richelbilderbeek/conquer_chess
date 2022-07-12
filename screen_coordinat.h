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

/// Get the default screen size, where x is the width and y is the height
screen_coordinat get_default_screen_size() noexcept;

screen_coordinat operator+(const screen_coordinat& lhs, const screen_coordinat& rhs) noexcept;

screen_coordinat& operator+=(screen_coordinat& lhs, const screen_coordinat& rhs) noexcept;

/// Run the tests for the screen coordinat
void test_screen_coordinat();

#endif // SCREEN_COORDINAT_H
