#include "controller_type.h"

#include <cassert>
#include <iostream>
#include <sstream>

controller_type get_next(const controller_type t) noexcept
{
  if (t == controller_type::mouse) return controller_type::keyboard;
  assert(t == controller_type::keyboard);
  return controller_type::mouse;
}

void test_controller_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(controller_type::mouse) == "mouse");
    assert(to_str(controller_type::keyboard) == "keyboard");
  }
  // get_next
  {
    assert(get_next(controller_type::mouse) == controller_type::keyboard);
    assert(get_next(controller_type::keyboard) == controller_type::mouse);

  }
  // operator<<
  {
    std::stringstream s;
    s << controller_type::mouse;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_str(const controller_type t) noexcept
{
  if (t == controller_type::mouse) return "mouse";
  assert(t == controller_type::keyboard);
  return "keyboard";
}

std::ostream& operator<<(std::ostream& os, const controller_type t) noexcept
{
  os << to_str(t);
  return os;
}
