#ifndef PIECE_TYPE_H
#define PIECE_TYPE_H

#include <iosfwd>

enum class piece_type
{
  bishop,
  king,
  knight,
  pawn,
  queen,
  rook
};

/// Get the maximum health for a piece
double get_max_health(const piece_type type);

/// Test this class and its free functions
void test_piece_type();

/// Convert a piece_type to a string
std::string to_str(const piece_type type) noexcept;

std::ostream& operator<<(std::ostream& os, const piece_type type) noexcept;

#endif // PIECE_TYPE_H
