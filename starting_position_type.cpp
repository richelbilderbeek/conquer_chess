#include "starting_position_type.h"

std::string to_str(const starting_position_type t) noexcept
{
  switch(t)
  {
    case starting_position_type::standard: return "standard";
    default:
    case starting_position_type::kings_only: return "kings_only";
  }
}
