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

/// Calculate the length of the vector,
/// a.k.a. the distance between the coordinat and the origin
double calc_length(const game_coordinat& coordinat) noexcept;

/// Get the game coordinat one square above this one,
/// i.e. when the player presses up
game_coordinat get_above(const game_coordinat& coordinat) noexcept;

/// Get the game coordinat one square below this one,
/// i.e. when the player presses down
game_coordinat get_below(const game_coordinat& coordinat) noexcept;

/// Get the game coordinat one square left of this one,
/// i.e. when the player presses left
game_coordinat get_left(const game_coordinat& coordinat) noexcept;

/// Get the game coordinat one square right of this one,
/// i.e. when the player presses right
game_coordinat get_right(const game_coordinat& coordinat) noexcept;

void test_game_coordinat();

/// center a coordinat on the center of a square,
/// i.e. at coorddinat (x.5, y.5)
game_coordinat center_on_center(const game_coordinat& coordinat);

std::ostream& operator<<(std::ostream& os, const game_coordinat& coordinat);

bool operator==(game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat operator-(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat operator+(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat& operator+=(game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat operator/(const game_coordinat& coordinat, const double factor) noexcept;




#endif // GAME_COORDINAT_H
