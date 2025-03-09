#include "user_input_type.h"

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

user_input_type create_random_user_input_type(
  std::default_random_engine& rng_engine
)
{
  const auto control_types{get_all_user_input_types()};
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

bool does_input_type_need_coordinat(const user_input_type t) noexcept
{
  return t == user_input_type::mouse_move;
}

std::vector<user_input_type> get_all_user_input_types() noexcept
{
  const auto a{magic_enum::enum_values<user_input_type>()};
  std::vector<user_input_type> v;
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
      create_random_user_input_type(rng_engine);
    }
  }
  // does_input_type_need_coordinat
  {
    assert(!does_input_type_need_coordinat(user_input_type::press_action_1));
    assert(!does_input_type_need_coordinat(user_input_type::press_action_2));
    assert(!does_input_type_need_coordinat(user_input_type::press_action_3));
    assert(!does_input_type_need_coordinat(user_input_type::press_action_4));
    assert(!does_input_type_need_coordinat(user_input_type::press_down));
    assert(!does_input_type_need_coordinat(user_input_type::press_left));
    assert(!does_input_type_need_coordinat(user_input_type::press_right));
    assert(!does_input_type_need_coordinat(user_input_type::press_up));
    assert(!does_input_type_need_coordinat(user_input_type::lmb_down));
    assert(!does_input_type_need_coordinat(user_input_type::rmb_down));
    assert(does_input_type_need_coordinat(user_input_type::mouse_move));
  }
  // get_all_control_action_types
  {
    const auto v{get_all_user_input_types()};
    assert(!v.empty());
  }
#endif
}

std::ostream& operator<<(std::ostream& os, const user_input_type t) noexcept
{
  os << std::string(magic_enum::enum_name(t));
  return os;
}
