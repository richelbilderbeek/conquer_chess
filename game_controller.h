#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "ccfwd.h"
#include "game_coordinat.h"
#include "side.h"
#include "user_inputs.h"

/// The class that acts as a controller for \link{game}.
class game_controller
{
public:
  game_controller();

  /// Add a user input. These will be processed in 'tick'
  void add_user_input(const user_input& a);

  /// Get the player position
  const game_coordinat& get_cursor_pos(const side player) const noexcept;

  /// Get the game users' inputs
  const auto& get_user_inputs() const noexcept { return m_user_inputs; }

  /// Get the game users' inputs
  auto& get_user_inputs() noexcept { return m_user_inputs; }

  /// Set a player's position
  void set_player_pos(const game_coordinat& pos, const side player) noexcept;

private:
  /// The in-game coordinat of the LHS user's cursor
  game_coordinat m_player_lhs_pos;

  /// The in-game coordinat of the RHS user's cursor
  game_coordinat m_player_rhs_pos;

  /// The user inputs that need to be processed
  user_inputs m_user_inputs;
};

/// Test this class and its free functions
void test_game_controller();

/// Test the \link{game_controller} class
/// and its free functions, for keyboard use
void test_game_controller_keyboard_use();

/// Test the \link{game_controller} class
/// and its free functions, for mouse use
void test_game_controller_mouse_use();

#endif // GAME_CONTROLLER_H
