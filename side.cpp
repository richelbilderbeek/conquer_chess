#include "side.h"

#include <cassert>

std::vector<side> get_all_sides() noexcept
{
  return
  {
    side::lhs,
    side::rhs
  };
}

side get_other_side(const side s) noexcept
{
  if (s == side::lhs) return side::rhs;
  assert(s == side::rhs);
  return side::lhs;
}

void test_side()
{
#ifndef NDEBUG
  // get_all_sides
  {
    assert(!get_all_sides().empty());
  }
  // get_other_side, all
  {
    assert(get_other_side(side::lhs) == side::rhs);
    assert(get_other_side(side::rhs) == side::lhs);
  }
#endif // NDEBUG

}
