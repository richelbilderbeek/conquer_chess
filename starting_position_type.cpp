#include "starting_position_type.h"

#include <cassert>

starting_position_type get_next(const starting_position_type starting_position) noexcept
{
  if (starting_position == starting_position_type::standard)
  {
    return starting_position_type::kings_only;
  }
  assert(starting_position == starting_position_type::kings_only);
  return starting_position_type::standard;
}


void test_starting_position_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(starting_position_type::standard) == "standard");
    assert(to_str(starting_position_type::kings_only) == "kings_only");
  }
  // get_next
  {
    assert(get_next(starting_position_type::standard) == starting_position_type::kings_only);
    assert(get_next(starting_position_type::kings_only) == starting_position_type::standard);
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
