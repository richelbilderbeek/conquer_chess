#ifndef CONTROL_ACTION_H
#define CONTROL_ACTION_H

#include "ccfwd.h"

#include "control_action_type.h"
#include "game_coordinat.h"

#include <SFML/Window/Event.hpp>

/// An action
class control_action
{
public:
  explicit control_action(
    const control_action_type type,
    const side player,
    const game_coordinat& coordinat
  );
  auto& get_coordinat() const noexcept { return m_coordinat; }
  auto get_player() const noexcept { return m_player; }
  auto get_type() const noexcept { return m_type; }

private:
  /// Must be a game_coordinat (not a square),
  /// as the mouse has actions
  game_coordinat m_coordinat;

  side m_player;

  control_action_type m_type;
};

control_action create_mouse_move_action(
  const game_coordinat& coordinat,
  const side player
);

control_action create_press_action_1(const side player);
control_action create_press_action_2(const side player);
control_action create_press_action_3(const side player);
control_action create_press_action_4(const side player);
control_action create_press_down_action(const side player);
control_action create_press_left_action(const side player);
control_action create_press_lmb_action(
  const game_coordinat& coordinat,
  const side player
);

control_action create_press_right_action(const side player);

control_action create_press_rmb_action(
  const game_coordinat& coordinat,
  const side player
);

control_action create_press_up_action(const side player);

/// Test the 'control_action' class and its free functions
void test_control_action();

#endif // CONTROL_ACTION_H

