#ifndef SQUARE_H
#define SQUARE_H

#include <string>

#include "ccfwd.h"

/// A chess square, e.g. e4
class square
{
public:
  square(const std::string& pos);
  square(const game_coordinat& g);

  const auto& get_pos() const noexcept { return m_pos; }

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

std::string to_str(const square& s) noexcept;

bool operator==(const square& lhs, const square& rhs) noexcept;

#endif // SQUARE_H
