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
  game_controller(
    const std::vector<physical_controller>& physical_controllers
  );

  /// Add a user input. These will be processed in 'game::tick'
  void add_user_input(const user_input& a);

  /// Get the a player's cursor position
  const game_coordinat& get_cursor_pos(const side player) const noexcept;

  /// A mouse user selects one of the four actions by using RMB
  /// and scrollwheel. The mouse user selector stores
  /// which of the four actions is selected.
  /// (a keyboard user has different keys for that)
  const auto& get_mouse_user_selector() const noexcept { return m_mouse_user_selector; }

  /// Get the game users' inputs
  const auto& get_user_inputs() const noexcept { return m_user_inputs; }

  const auto& get_physical_controllers() const noexcept { return m_physical_controllers; }

  /// Get the game users' inputs
  auto& get_user_inputs() noexcept { return m_user_inputs; }

  /// Set the selected action for the mouse user.
  /// Assumes there is a mouse user
  void set_mouse_user_selector(const int index);

  /// Set a player's position
  void set_player_pos(const game_coordinat& pos, const side player) noexcept;

private:

  /// The in-game coordinat of the LHS user's cursor
  game_coordinat m_lhs_cursor_pos;

  /// A mouse user selects one of the four actions
  /// (a keyboard user has different keys for that)
  std::optional<int> m_mouse_user_selector;

  /// The physical controllers the players use
  std::vector<physical_controller> m_physical_controllers;

  /// The in-game coordinat of the RHS user's cursor
  game_coordinat m_rhs_cursor_pos;

  /// The user inputs that need to be processed
  user_inputs m_user_inputs;
};

/// Get the physical controllers
const std::vector<physical_controller>& get_physical_controllers(const game_controller& c) noexcept;

/// Test this class and its free functions
void test_game_controller();

/// Test the \link{game_controller} class
/// and its free functions, for keyboard use
void test_game_controller_keyboard_use();

/// Test the \link{game_controller} class
/// and its free functions, for mouse use
void test_game_controller_mouse_use();

#endif // GAME_CONTROLLER_H
