#ifndef SIDE_H
#define SIDE_H

/// The side the player/piece is on
enum class side
{
  lhs, // left-hand side
  rhs  // right-hand side
};

side get_other_side(const side s) noexcept;

#endif // SIDE_H
