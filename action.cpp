#include "action.h"

action::action(
  const control_action_type type,
  const game_coordinat& coordinat
) : m_coordinat{coordinat}, m_type{type}
{

}

action create_mouse_move_action(const game_coordinat& coordinat)
{
  return action(control_action_type::mouse_move, coordinat);
}

action create_press_attack_action()
{
  return action(control_action_type::press_attack, game_coordinat());
}

action create_press_down_action()
{
  return action(control_action_type::press_down, game_coordinat());
}

action create_press_left_action()
{
  return action(control_action_type::press_left, game_coordinat());
}

action create_press_lmb_action(const game_coordinat& coordinat)
{
  return action(control_action_type::lmb_down, coordinat);
}

action create_press_move_action()
{
  return action(control_action_type::press_move, game_coordinat());
}

action create_press_right_action()
{
  return action(control_action_type::press_right, game_coordinat());
}

action create_press_rmb_action(const game_coordinat& coordinat)
{
  return action(control_action_type::rmb_down, coordinat);
}

action create_press_select_action()
{
  return action(control_action_type::press_select, game_coordinat());
}

action create_press_up_action()
{
  return action(control_action_type::press_up, game_coordinat());
}
