#include "side.h"

#include <cassert>
#include <iostream>
#include <sstream>

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
  // to_str
  {
    assert(to_str(side::lhs) == "lhs");
    assert(to_str(side::rhs) == "rhs");
  }
  // operator<<
  {
    std::stringstream s;
    s << side::lhs;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

std::string to_str(const side& s) noexcept
{
  if (s == side::lhs) return "lhs";
  assert(s == side::rhs);
  return "rhs";
}

std::ostream& operator<<(std::ostream& os, const side s) noexcept
{
  os << to_str(s);
  return os;
}
