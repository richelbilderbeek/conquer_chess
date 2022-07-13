#include "piece_type.h"

#include <cassert>
#include <iostream>

double get_max_health(const piece_type type)
{
  switch (type)
  {
    case piece_type::king: return 100.0;
    case piece_type::queen: return 100.0;
    case piece_type::rook: return 100.0;
    default: assert(!"Should not get here"); break;
  }
  return 3.14;
}

std::ostream& operator<<(std::ostream& os, const piece_type type)
{
  switch (type)
  {
    case piece_type::king: os << "king"; break;
    case piece_type::queen: os << "queen"; break;
    case piece_type::rook: os << "rook"; break;
    default: assert(!"Should not get here"); os << "";
  }
  return os;
}
