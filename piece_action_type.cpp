#include "piece_action_type.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum.hpp"

void test_piece_action_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(piece_action_type::move) == "move");
    assert(to_str(piece_action_type::attack) == "attack");
  }
  // operator<<
  {
    std::stringstream s;
    s << piece_action_type::move;
    assert(s.str() == to_str(piece_action_type::move));
  }
#endif // DEBUG

}

std::string to_str(const piece_action_type t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}

std::ostream& operator<<(std::ostream& os, const piece_action_type& p) noexcept
{
  os << to_str(p);
  return os;
}
