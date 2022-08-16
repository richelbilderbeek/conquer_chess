#ifndef SCREEN_COORDINAT_H
#define SCREEN_COORDINAT_H

#include <iosfwd>

/// Coordinats on the screen.
/// @see game_coordinat for the coordinats in the game
class screen_coordinat
{
public:
  explicit screen_coordinat(const int x = 0, const int y = 0);

  int get_x() const noexcept { return m_x; }
  int get_y() const noexcept { return m_y; }

private:

  int m_x;
  int m_y;
};

/// Calculate the angle in degrees
/// dx | dy | o'clock | degrees
/// ---|----|---------|--------
///  1 |  0 |       3 | 0
///  0 |  1 |       6 | 270
/// -1 |  0 |       9 | 180
///  0 | -1 |      12 | 90
double calc_angle_degrees(const screen_coordinat& from, const screen_coordinat& to);

/// Calculate the angle in degrees
/// dx | dy | o'clock | degrees
/// ---|----|---------|--------
///  1 |  0 |       3 | 0
///  0 |  1 |       6 | 270
/// -1 |  0 |       9 | 180
///  0 | -1 |      12 | 90
double calc_angle_degrees(const screen_coordinat& delta);

/// Calculate the Euclidean distance
double calc_distance(const screen_coordinat& a, const screen_coordinat& b) noexcept;

/// Get the default main about screen size, where x is the width and y is the height
screen_coordinat get_default_about_screen_size() noexcept;

/// Get the default controls screen size, where x is the width and y is the height
screen_coordinat get_default_controls_screen_size() noexcept;

/// Get the default main menu screen size, where x is the width and y is the height
screen_coordinat get_default_main_menu_screen_size() noexcept;

/// Get the default played game screen size, where x is the width and y is the height
screen_coordinat get_default_played_game_screen_size() noexcept;

/// Get the default screen size, where x is the width and y is the height
screen_coordinat get_default_screen_size() noexcept;


std::ostream& operator<<(std::ostream& os, const screen_coordinat& coordinat);

bool operator==(const screen_coordinat& lhs, const screen_coordinat& rhs) noexcept;

screen_coordinat operator+(const screen_coordinat& lhs, const screen_coordinat& rhs) noexcept;
screen_coordinat operator-(const screen_coordinat& lhs, const screen_coordinat& rhs) noexcept;
screen_coordinat operator*(const screen_coordinat& lhs, const double& factor) noexcept;
screen_coordinat operator/(const screen_coordinat& lhs, const double& factor);

screen_coordinat& operator+=(screen_coordinat& lhs, const screen_coordinat& rhs) noexcept;

/// Run the tests for the screen coordinat
void test_screen_coordinat();

#endif // SCREEN_COORDINAT_H
