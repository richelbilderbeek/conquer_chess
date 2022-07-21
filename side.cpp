#include "side.h"

#include <cassert>

side get_other_side(const side s) noexcept
{
  if (s == side::lhs) return side::rhs;
  assert(s == side::rhs);
  return side::lhs;
}

