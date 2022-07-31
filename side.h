#ifndef SIDE_H
#define SIDE_H

#include <iosfwd>
#include <string>
#include <vector>

/// The side the player/piece is on
enum class side
{
  lhs, // left-hand side
  rhs  // right-hand side
};

/// Get all sides
std::vector<side> get_all_sides() noexcept;

side get_other_side(const side s) noexcept;

/// Test this class and its free functions
void test_side();

std::string to_str(const side& s) noexcept;

std::ostream& operator<<(std::ostream& os, const side s) noexcept;

#endif // SIDE_H
