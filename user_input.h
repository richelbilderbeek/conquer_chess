#ifndef USER_INPUT_H
#define USER_INPUT_H

#include "ccfwd.h"

#include "user_input_type.h"
#include "game_coordinat.h"
#include "side.h"

#include <iosfwd>
#include <random>
#include <SFML/Window/Event.hpp>

/// An action
class user_input
{
public:
  explicit user_input(
    const user_input_type type,
    const side player,
    const game_coordinat& coordinat
  );
  auto get_control_action_type() const noexcept { return m_control_action_type; }
  auto& get_coordinat() const noexcept { return m_coordinat; }
  auto get_player() const noexcept { return m_player; }

private:

  user_input_type m_control_action_type;

  /// Must be a game_coordinat (not a square),
  /// as the mouse has actions
  game_coordinat m_coordinat;

  side m_player;

};

user_input create_mouse_move_action(
  const game_coordinat& coordinat,
  const side player
);

user_input create_press_action_1(
  const game_coordinat& coordinat,
  const side player
);

user_input create_press_action_2(const side player);
user_input create_press_action_3(const side player);
user_input create_press_action_4(const side player);
user_input create_press_down_action(const side player);
user_input create_press_left_action(const side player);
user_input create_press_lmb_action(
  const game_coordinat& coordinat,
  const side player
);

user_input create_press_right_action(const side player);

user_input create_press_rmb_action(
  const game_coordinat& coordinat,
  const side player
);

user_input create_press_up_action(const side player);

user_input create_random_control_action(
  std::default_random_engine& rng_engine
);

/// Test the 'control_action' class and its free functions
void test_control_action();

bool operator==(const user_input& lhs, const user_input& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const user_input& a) noexcept;

#endif // USER_INPUT_H

