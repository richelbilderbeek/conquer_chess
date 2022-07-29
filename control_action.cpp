#include "control_action.h"

#include <cassert>

#include "game_coordinat.h"

control_action::control_action(
  const control_action_type type,
  const game_coordinat& coordinat
) : m_coordinat{coordinat}, m_type{type}
{

}

control_action create_mouse_move_action(const game_coordinat& coordinat)
{
  return control_action(control_action_type::mouse_move, coordinat);
}

control_action create_press_attack_action()
{
  return control_action(control_action_type::press_attack, game_coordinat());
}

control_action create_press_down_action()
{
  return control_action(control_action_type::press_down, game_coordinat());
}

control_action create_press_left_action()
{
  return control_action(control_action_type::press_left, game_coordinat());
}

control_action create_press_lmb_action(const game_coordinat& coordinat)
{
  return control_action(control_action_type::lmb_down, coordinat);
}

control_action create_press_move_action()
{
  return control_action(control_action_type::press_move, game_coordinat());
}

control_action create_press_right_action()
{
  return control_action(control_action_type::press_right, game_coordinat());
}

control_action create_press_rmb_action(const game_coordinat& coordinat)
{
  return control_action(control_action_type::rmb_down, coordinat);
}

control_action create_press_select_action()
{
  return control_action(control_action_type::press_select, game_coordinat());
}

control_action create_press_up_action()
{
  return control_action(control_action_type::press_up, game_coordinat());
}

void test_control_action()
{
#ifndef NDEBUG
  {
    assert(create_mouse_move_action(game_coordinat()).get_type() == control_action_type::mouse_move);
    assert(create_press_attack_action().get_type() == control_action_type::press_attack);
    assert(create_press_down_action().get_type() == control_action_type::press_down);
    assert(create_press_left_action().get_type() == control_action_type::press_left);
    assert(create_press_lmb_action(game_coordinat()).get_type() == control_action_type::lmb_down);
    assert(create_press_move_action().get_type() == control_action_type::press_move);
    assert(create_press_right_action().get_type() == control_action_type::press_right);
    assert(create_press_rmb_action(game_coordinat()).get_type() == control_action_type::rmb_down);
    assert(create_press_select_action().get_type() == control_action_type::press_select);
    assert(create_press_up_action().get_type() == control_action_type::press_up);
  }
#endif // DEBUG
}
