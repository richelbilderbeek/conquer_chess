#include "control_action_type.h"

#include "../magic_enum/include/magic_enum.hpp"

control_action_type create_random_control_action_type(
  std::default_random_engine& rng_engine
)
{
  const auto control_types{get_all_control_action_types()};
  assert(!control_types.empty());
  std::uniform_int_distribution<int> distribution{
    0,
    static_cast<int>(control_types.size()) - 1 // -1 as inclusive
  };
  const auto i{distribution(rng_engine)};
  assert(i >= 0);
  assert(i < static_cast<int>(control_types.size()));
  return control_types[i];
}

std::vector<control_action_type> get_all_control_action_types() noexcept
{
  const auto a{magic_enum::enum_values<control_action_type>()};
  std::vector<control_action_type> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

void test_control_action_type()
{
#ifndef NDEBUG
  // create_random_control_action_type
  {
    const int seed{314};
    std::default_random_engine rng_engine(seed);
    for (int i{0}; i!=100; ++i)
    {
      create_random_control_action_type(rng_engine);
    }
  }
  // get_all_control_action_types
  {
    const auto v{get_all_control_action_types()};
    assert(!v.empty());
  }
#endif
}
