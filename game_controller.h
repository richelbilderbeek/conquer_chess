#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "ccfwd.h"
#include "action_number.h"

#include "physical_controllers.h"
#include "game_coordinat.h"
#include "side.h"
#include "user_inputs.h"

#include <iosfwd>

/// The class that acts as a controller for \link{game}.
class game_controller
{
public:
  game_controller(
    const physical_controllers& controllers = physical_controllers()
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

  /// Get a player's physical controller
  const physical_controller& get_physical_controller(const side player) const noexcept;

  /// Get the game users' inputs
  auto& get_user_inputs() noexcept { return m_user_inputs; }

  /// Set the selected action for the mouse user.
  /// Assumes there is a mouse user
  void set_mouse_user_selector(const action_number& number);

  /// Set a player's position
  void set_player_pos(const game_coordinat& pos, const side player) noexcept;

private:

  /// The in-game coordinat of the LHS user's cursor
  game_coordinat m_lhs_cursor_pos;

  /// A mouse user selects one of the four actions
  /// (a keyboard user has different keys for that)
  std::optional<action_number> m_mouse_user_selector;

  /// The physical controllers the players use
  physical_controllers m_physical_controllers;

  /// The in-game coordinat of the RHS user's cursor
  game_coordinat m_rhs_cursor_pos;

  /// The user inputs that need to be processed
  user_inputs m_user_inputs;
};

/// Add a user input. These will be processed in 'game::tick'
void add_user_input(game_controller& c, const user_input& input);

/// Add a user_inputs. These will be processed in 'game::tick'
void add_user_inputs(game_controller& c, const user_inputs& input);

/// What side does the player with the mouse control?
side get_mouse_user_player_side(const game_controller& c);

/// Get the physical controllers
const physical_controller& get_physical_controller(
  const game_controller& c,
  const side player_side
) noexcept;

/// Is there a player that uses the mouse?
bool has_mouse_controller(const game_controller& c);

/// Test this class and its free functions
void test_game_controller();

/// Test the \link{game_controller} class
/// and its free functions, for keyboard use
void test_game_controller_keyboard_use();

/// Test the \link{game_controller} class
/// and its free functions, for mouse use
void test_game_controller_mouse_use();

std::ostream& operator<<(std::ostream& os, const game_controller& g) noexcept;

#endif // GAME_CONTROLLER_H
