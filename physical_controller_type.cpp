#include "physical_controller_type.h"

#include <cassert>
#include <iostream>
#include <sstream>

physical_controller_type get_next(const physical_controller_type t) noexcept
{
  if (t == physical_controller_type::mouse) return physical_controller_type::keyboard;
  assert(t == physical_controller_type::keyboard);
  return physical_controller_type::mouse;
}

physical_controller_type get_previous(const physical_controller_type t) noexcept
{
  if (t == physical_controller_type::mouse) return physical_controller_type::keyboard;
  assert(t == physical_controller_type::keyboard);
  return physical_controller_type::mouse;
}

void test_physical_controller_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(physical_controller_type::mouse) == "mouse");
    assert(to_str(physical_controller_type::keyboard) == "keyboard");
  }
  // get_next
  {
    assert(get_next(physical_controller_type::mouse) == physical_controller_type::keyboard);
    assert(get_next(physical_controller_type::keyboard) == physical_controller_type::mouse);

  }
  // get_previous
  {
    assert(get_previous(get_next(physical_controller_type::mouse)) == physical_controller_type::mouse);
    assert(get_previous(get_next(physical_controller_type::keyboard)) == physical_controller_type::keyboard);

  }
  // operator<<
  {
    std::stringstream s;
    s << physical_controller_type::mouse;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_str(const physical_controller_type t) noexcept
{
  if (t == physical_controller_type::mouse) return "mouse";
  assert(t == physical_controller_type::keyboard);
  return "keyboard";
}

std::ostream& operator<<(std::ostream& os, const physical_controller_type t) noexcept
{
  os << to_str(t);
  return os;
}
