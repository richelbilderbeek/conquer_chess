#include "side.h"

#include <cassert>

side get_other_side(const side s) noexcept
{
  if (s == side::lhs) return side::rhs;
  assert(s == side::rhs);
  return side::lhs;
}

void test_side()
{
#ifndef NDEBUG
  {
    assert(get_other_side(side::lhs) == side::rhs);
    assert(get_other_side(side::rhs) == side::lhs);
  }
#endif // NDEBUG

}
