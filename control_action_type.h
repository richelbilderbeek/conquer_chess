#ifndef CONTROL_ACTION_TYPE_H
#define CONTROL_ACTION_TYPE_H

#include <random>

/// The actions done by the controller,
/// such as keyboard, mouse and joystick actions
enum class control_action_type
{
  press_action_1,
  press_action_2,
  press_action_3,
  press_action_4,
  press_down,
  press_left,
  press_right,
  press_up,
  lmb_down,
  rmb_down,
  mouse_move
};

/// Create a random control_action_type
control_action_type create_random_control_action_type(
  std::default_random_engine& rng_engine
);

/// Get all the control_action_types
std::vector<control_action_type> get_all_control_action_types() noexcept;

/// Test this enum class and its fee functions
void test_control_action_type();

std::ostream& operator<<(std::ostream& os, const control_action_type t) noexcept;

#endif // ACTION_TYPE_H
