#include "controller_type.h"

#include <cassert>
std::string to_str(const controller_type t) noexcept
{
  if (t == controller_type::mouse) return "mouse";
  assert(t == controller_type::keyboard);
  return "keyboard";
}
