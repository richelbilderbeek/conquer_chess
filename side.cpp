#include "side.h"

#include <cassert>
#include <iostream>
#include <sstream>

side create_random_side(
  std::default_random_engine& rng_engine
)
{
  const auto sides{get_all_sides()};
  assert(!sides.empty());
  std::uniform_int_distribution<int> distribution{
    0,
    static_cast<int>(sides.size()) - 1 // -1 as inclusive
  };
  const auto i{distribution(rng_engine)};
  assert(i >= 0);
  assert(i < static_cast<int>(sides.size()));
  return sides[i];
}

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
  // create_random_side
  {
    const int seed{314};
    std::default_random_engine rng_engine(seed);
    for (int i{0}; i!=10; ++i)
    {
      create_random_side(rng_engine);
    }
  }
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
