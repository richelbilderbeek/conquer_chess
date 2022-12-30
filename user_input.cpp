#include "user_input.h"

#include <cassert>
#include <iostream>

#include "game_coordinat.h"

user_input::user_input(
  const user_input_type type,
  const side player,
  const std::optional<game_coordinat>& coordinat
) : m_user_input_type{type},
    m_coordinat{coordinat},
    m_player{player}
{
  #ifndef NDEBUG
  if (does_input_type_need_coordinat(m_user_input_type))
  {
    assert(m_coordinat);
  }
  else
  {
    assert(!m_coordinat);
  }
  #endif
}

user_input create_mouse_move_action(
  const game_coordinat& coordinat,
  const side player
)
{
  return user_input(user_input_type::mouse_move, player, coordinat);
}

user_input create_press_action_1(
  const side player
)
{
  return user_input(user_input_type::press_action_1, player);
}

user_input create_press_action_2(const side player)
{
  return user_input(user_input_type::press_action_2, player);
}

user_input create_press_action_3(const side player)
{
  return user_input(user_input_type::press_action_3, player);
}

user_input create_press_action_4(const side player)
{
  return user_input(user_input_type::press_action_4, player);
}

user_input create_press_down_action(const side player)
{
  return user_input(user_input_type::press_down, player);
}

user_input create_press_left_action(const side player)
{
  return user_input(user_input_type::press_left, player);
}

user_input create_press_lmb_action(
  const side player
)
{
  return user_input(user_input_type::lmb_down, player);
}

user_input create_press_right_action(const side player)
{
  return user_input(user_input_type::press_right, player);
}

user_input create_press_rmb_action(const side player)
{
  return user_input(user_input_type::rmb_down, player);
}

user_input create_press_up_action(const side player)
{
  return user_input(user_input_type::press_up, player);
}

user_input create_random_user_input(
  std::default_random_engine& rng_engine
)
{
  const user_input_type type{create_random_user_input_type(rng_engine)};
  const side player{create_random_side(rng_engine)};
  std::optional<game_coordinat> maybe_coordinat;
  if (does_input_type_need_coordinat(type))
  {
    maybe_coordinat = create_random_game_coordinat(rng_engine);
  }
  return user_input(
    type,
    player,
    maybe_coordinat
  );
}

void test_user_input()
{
#ifndef NDEBUG
  {
    assert(create_mouse_move_action(game_coordinat(), side::lhs).get_user_input_type() == user_input_type::mouse_move);
    assert(create_press_action_1(side::lhs).get_user_input_type() == user_input_type::press_action_1);
    assert(create_press_action_2(side::lhs).get_user_input_type() == user_input_type::press_action_2);
    assert(create_press_action_3(side::lhs).get_user_input_type() == user_input_type::press_action_3);
    assert(create_press_action_4(side::lhs).get_user_input_type() == user_input_type::press_action_4);
    assert(create_press_down_action(side::lhs).get_user_input_type() == user_input_type::press_down);
    assert(create_press_left_action(side::lhs).get_user_input_type() == user_input_type::press_left);
    assert(create_press_lmb_action(side::lhs).get_user_input_type() == user_input_type::lmb_down);
    assert(create_press_right_action(side::lhs).get_user_input_type() == user_input_type::press_right);
    assert(create_press_rmb_action(side::lhs).get_user_input_type() == user_input_type::rmb_down);
    assert(create_press_up_action(side::lhs).get_user_input_type() == user_input_type::press_up);
  }
  // create_random_control_action
  {
    const int seed{314};
    std::default_random_engine rng_engine(seed);
    create_random_user_input(rng_engine);
  }
#endif // DEBUG
}

bool operator==(const user_input& lhs, const user_input& rhs) noexcept
{
  return lhs.get_coordinat() == rhs.get_coordinat()
    && lhs.get_player() == rhs.get_player()
    && lhs.get_user_input_type() == rhs.get_user_input_type()
  ;
}

std::ostream& operator<<(std::ostream& os, const user_input& a) noexcept
{
  os
    << "coordinat: ";
  if (a.get_coordinat())
  {
    os << a.get_coordinat().value();
  }
  else
  {
    os << "{}";
  }
  os
    << '\n'
    << "player: " << a.get_player() << '\n'
    << "type: " << a.get_user_input_type()
  ;
  return os;
}
