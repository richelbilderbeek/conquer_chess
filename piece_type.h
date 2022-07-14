#ifndef PIECE_TYPE_H
#define PIECE_TYPE_H

#include <iosfwd>

enum class piece_type
{
  bishop,
  king,
  knight,
  queen,
  rook
};

/// Get the maximum health for a piece
double get_max_health(const piece_type type);

std::ostream& operator<<(std::ostream& os, const piece_type type);

#endif // PIECE_TYPE_H
