#include "action.h"

action::action(
  const action_type type,
  const screen_coordinat& coordinat
) : m_coordinat{coordinat}, m_type{type}
{

}

action create_mouse_move_action(const screen_coordinat& coordinat)
{
  return action(action_type::mouse_move, coordinat);
}

action create_press_lmb_action(const screen_coordinat& coordinat)
{
  return action(action_type::lmb_down, coordinat);
}
