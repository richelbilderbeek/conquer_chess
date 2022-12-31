#include "game_controller.h"

#include <cassert>

game_controller::game_controller()
  : m_player_lhs_pos{0.5, 4.5},
    m_player_rhs_pos{7.5, 4.5}
{

}

void game_controller::add_user_input(const user_input& a)
{
  // These will be processed in 'tick'
  m_user_inputs.add(a);
}

const game_coordinat& game_controller::get_player_pos(const side player) const noexcept
{
  if (player == side::lhs) return m_player_lhs_pos;
  assert(player == side::rhs);
  return m_player_rhs_pos;
}

/*
game_coordinat& game_controller::get_player_pos(const side player) noexcept
{
  if (player == side::lhs) return m_player_lhs_pos;
  assert(player == side::rhs);
  return m_player_rhs_pos;
}
*/

void game_controller::set_player_pos(
  const game_coordinat& pos,
  const side player) noexcept
{
  if (player == side::lhs)
  {
    m_player_lhs_pos = pos;
  }
  else
  {
    assert(player == side::rhs);
    m_player_rhs_pos = pos;
  }
}
