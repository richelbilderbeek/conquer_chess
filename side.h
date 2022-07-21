#ifndef SIDE_H
#define SIDE_H

/// The side the player/piece is on
enum class side
{
  lhs, // left-hand side
  rhs  // right-hand side
};

side get_other_side(const side s) noexcept;

/// Test this class and its free functions
void test_side();

#endif // SIDE_H
