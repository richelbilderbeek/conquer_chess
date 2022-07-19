#include "controller_type.h"

#include <cassert>

void test_controller_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(controller_type::mouse) == "mouse");
    assert(to_str(controller_type::keyboard) == "keyboard");
  }
#endif // DEBUG
}

std::string to_str(const controller_type t) noexcept
{
  if (t == controller_type::mouse) return "mouse";
  assert(t == controller_type::keyboard);
  return "keyboard";
}

