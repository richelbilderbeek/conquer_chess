#ifndef SQUARE_H
#define SQUARE_H

#include <iosfwd>
#include <string>

#include "ccfwd.h"

/// A chess square, e.g. e4
class square
{
public:
  square(const std::string& pos);
  square(const game_coordinat& g);

  /// Get the position as a string
  const auto& get_pos() const noexcept { return m_pos; }

  /// Get the x coordinat, starting from 0 for a1/b1/c1/etc.
  /// As the board goes from a1 at top-left,
  /// to a8 at top-right,
  /// the x coordinat is the rank
  int get_x() const;

  /// Get the y coordinat, starting from 0 for a1/a2/a3/etc.
  /// As the board goes from a1 at top-left,
  /// to a8 at top-right,
  /// the y coordinat is the file
  int get_y() const;

private:

  std::string m_pos;
};

/// Are the squares on the same diagonal, e.g. d1 and a4
bool are_on_same_diagonal(const square& a, const square& b) noexcept;

/// Are the squares on the same file, e.g. e2 and e4
bool are_on_same_file(const square& a, const square& b) noexcept;

/// Are the squares on the same half-diagonal,
/// also 'the jump of a knight'
/// e.g. b1 and c3
bool are_on_same_half_diagonal(const square& a, const square& b) noexcept;

/// Are the squares on the same rank, e.g. a1 and a8
bool are_on_same_rank(const square& a, const square& b) noexcept;

/// Rotate the (coordinator of the) square,
/// i.e. turn the board 180 degrees
square get_rotated_square(const square& position) noexcept;

/// Test this class and its free functions
void test_square();

// a1 == (0.5, 0.5)
// b1 == (0.5, 1.5)
game_coordinat to_coordinat(const square& s) noexcept;

// a1 == (0.5, 0.5)
// b1 == (0.5, 1.5)
game_coordinat to_coordinat(const std::string& notation) noexcept;

/// Convert a square to a rectangle
/// For example, a1 == ((0,0)-(1,1)) (notation is top-left, then bottom-left)
/// For example, b1 == ((0,1)-(1,2)) (notation is top-left, then bottom-left)
game_rect to_game_rect(const square& s) noexcept;

std::string to_str(const square& s) noexcept;

bool operator==(const square& lhs, const square& rhs) noexcept;
bool operator!=(const square& lhs, const square& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const square& s) noexcept;

#endif // SQUARE_H
