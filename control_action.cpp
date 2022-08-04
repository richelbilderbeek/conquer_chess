#include "control_action.h"

#include <cassert>

#include "game_coordinat.h"

control_action::control_action(
  const control_action_type type,
  const side player,
  const game_coordinat& coordinat
) : m_coordinat{coordinat},
    m_player{player},
    m_type{type}
{

}

control_action create_mouse_move_action(
  const game_coordinat& coordinat,
  const side player
)
{
  return control_action(control_action_type::mouse_move, player, coordinat);
}

control_action create_press_action_1(const side player)
{
  return control_action(control_action_type::press_action_1, player, game_coordinat());
}

control_action create_press_action_2(const side player)
{
  return control_action(control_action_type::press_action_2, player, game_coordinat());
}

control_action create_press_action_3(const side player)
{
  return control_action(control_action_type::press_action_3, player, game_coordinat());
}

control_action create_press_action_4(const side player)
{
  return control_action(control_action_type::press_action_4, player, game_coordinat());
}

/*
control_action create_press_attack_action(const side player)
{
  return control_action(control_action_type::press_attack, player, game_coordinat());
}
*/

control_action create_press_down_action(const side player)
{
  return control_action(control_action_type::press_down, player, game_coordinat());
}

control_action create_press_left_action(const side player)
{
  return control_action(control_action_type::press_left, player, game_coordinat());
}

control_action create_press_lmb_action(
  const game_coordinat& coordinat,
  const side player
)
{
  return control_action(control_action_type::lmb_down, player, coordinat);
}

/*
control_action create_press_move_action(const side player)
{
  return control_action(control_action_type::press_move, player, game_coordinat());
}
*/

control_action create_press_right_action(const side player)
{
  return control_action(control_action_type::press_right, player, game_coordinat());
}

control_action create_press_rmb_action(const game_coordinat& coordinat, const side player)
{
  return control_action(control_action_type::rmb_down, player, coordinat);
}

/*
control_action create_press_select_action(const side player)
{
  return control_action(control_action_type::press_select, player, game_coordinat());
}
*/

control_action create_press_up_action(const side player)
{
  return control_action(control_action_type::press_up, player, game_coordinat());
}

void test_control_action()
{
#ifndef NDEBUG
  {
    assert(create_mouse_move_action(game_coordinat(), side::lhs).get_type() == control_action_type::mouse_move);
    //assert(create_press_attack_action(side::lhs).get_type() == control_action_type::press_attack);
    assert(create_press_action_1(side::lhs).get_type() == control_action_type::press_action_1);
    assert(create_press_action_2(side::lhs).get_type() == control_action_type::press_action_2);
    assert(create_press_action_3(side::lhs).get_type() == control_action_type::press_action_3);
    assert(create_press_action_4(side::lhs).get_type() == control_action_type::press_action_4);
    assert(create_press_down_action(side::lhs).get_type() == control_action_type::press_down);
    assert(create_press_left_action(side::lhs).get_type() == control_action_type::press_left);
    assert(create_press_lmb_action(game_coordinat(), side::lhs).get_type() == control_action_type::lmb_down);
    //assert(create_press_move_action(side::lhs).get_type() == control_action_type::press_move);
    assert(create_press_right_action(side::lhs).get_type() == control_action_type::press_right);
    assert(create_press_rmb_action(game_coordinat(), side::lhs).get_type() == control_action_type::rmb_down);
    //assert(create_press_select_action(side::lhs).get_type() == control_action_type::press_select);
    assert(create_press_up_action(side::lhs).get_type() == control_action_type::press_up);
  }
#endif // DEBUG
}
