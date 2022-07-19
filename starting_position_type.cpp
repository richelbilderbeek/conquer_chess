#include "starting_position_type.h"

#include <cassert>

void test_starting_position_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(starting_position_type::standard) == "standard");
    assert(to_str(starting_position_type::kings_only) == "kings_only");
  }
#endif // DEBUG
}

std::string to_str(const starting_position_type t) noexcept
{
  switch(t)
  {
    case starting_position_type::standard: return "standard";
    default:
    case starting_position_type::kings_only: return "kings_only";
  }
}
