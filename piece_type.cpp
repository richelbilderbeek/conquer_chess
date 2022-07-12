#include "piece_type.h"

#include <cassert>
#include <iostream>

double get_max_health(const piece_type type)
{
  switch (type)
  {
    case piece_type::king: return 100.0;
    default: assert(!"Should not get here"); break;
  }
  return 3.14;
}

std::ostream& operator<<(std::ostream& os, const piece_type type)
{
  switch (type)
  {
    case piece_type::king: os << "king"; break;
    default: assert(!"Should not get here"); os << "";
  }
  return os;
}
