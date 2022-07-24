#ifndef GAME_COORDINAT_H
#define GAME_COORDINAT_H

#include <iosfwd>

#include "ccfwd.h"
#include "side.h"

/// Coordinat on the board
/// @see use screen_coordinat for a coordinat on the screen
///
///        x coordinat
/// |-----------------------|
///
/// 0  1  2  3  4  5  6  7  8
/// +--+--+--+--+--+--+--+--+ 0  -
/// |  |  |  |  |  |  |  |  |    |
/// +--+--+--+--+--+--+--+--+ 1  |
/// .  .  .  .  .  .  .  .  .    | y coordinat
/// .  .  .  .  .  .  .  .  .    |
/// +--+--+--+--+--+--+--+--+ 7  |
/// |  |  |  |  |  |  |  |  |    |
/// +--+--+--+--+--+--+--+--+ 8  -
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

/// Calculate the euclidean distance between two points
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

/// Rotate the coordinat,
/// i.e. turn the board 180 degrees
game_coordinat get_rotated_coordinat(const game_coordinat& coordinat) noexcept;

/// Is the 'to' coordinat forward, i.e. can a pawn move/attack
/// in that direction?
bool is_forward(
  const square& from,
  const square& to,
  const side player
);

/// Test this class and its free function
void test_game_coordinat();

/// Convert to coordinat to chess notation, e.g. 'e2'
/// For invalid coordinats, returns '--'
std::string to_notation(const game_coordinat& g);

/// center a coordinat on the center of a square,
/// i.e. at coorddinat (x.5, y.5)
game_coordinat center_on_center(const game_coordinat& coordinat);

std::ostream& operator<<(std::ostream& os, const game_coordinat& coordinat);

bool operator==(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;
bool operator!=(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat operator-(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat operator+(const game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat& operator+=(game_coordinat& lhs, const game_coordinat& rhs) noexcept;
game_coordinat operator/(const game_coordinat& coordinat, const double factor) noexcept;

#endif // GAME_COORDINAT_H
