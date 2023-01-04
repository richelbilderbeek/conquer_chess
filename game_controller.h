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
/// A \link{game_controller} receives \link{user_inputs}.
/// It will use those \link{user_inputs}
/// to move the cursors and to do chess moves on the \link{game}.
class game_controller
{
public:
  game_controller(
    const physical_controllers& controllers = physical_controllers()
  );

  /// Add a user input. These will be processed in 'game::tick'
  void add_user_input(const user_input& a);

  /// Process all actions and apply these on the game
  void apply_user_inputs_to_game(game& g);

  /// Get the a player's cursor position
  const game_coordinat& get_cursor_pos(const side player_side) const noexcept;

  /// A mouse user selects one of the four actions by using RMB
  /// and scrollwheel. The mouse user selector stores
  /// which of the four actions is selected.
  /// (a keyboard user has different keys for that)
  const auto& get_mouse_user_selector() const noexcept { return m_mouse_user_selector; }

  /// Get the game users' inputs
  const auto& get_user_inputs() const noexcept { return m_user_inputs; }

  /// Get a player's physical controller
  const physical_controller& get_physical_controller(const side player_side) const noexcept;

  /// Get the game users' inputs
  //auto& get_user_inputs() noexcept { return m_user_inputs; }

  /// Set the selected action for the mouse user.
  /// Assumes there is a mouse user
  void set_mouse_user_selector(const action_number& number);

  /// Set a player's cursor's position
  void set_cursor_pos(const game_coordinat& pos, const side player_side) noexcept;

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

/// Count the total number of \link{user_input}s
/// to be done by the \link{game_controller}.
int count_user_inputs(const game_controller& c) noexcept;

/// Get the a player's cursor position
const game_coordinat& get_cursor_pos(
  const game_controller& c,
  const side player_side
) noexcept;

/// Get the a player's cursor position, as a \link{square}
square get_cursor_square(
  const game_controller& c,
  const side player_side
);

/// Get the default, primary, most likely piece action
/// Returns an empty optional if the current setup
/// cannot result in an action
std::optional<piece_action_type> get_default_piece_action(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept;

/// Get the side of the controller that uses the keyboard.
/// Assumes there is one controller that uses the keyboard
side get_keyboard_user_player_side(const game_controller& c);

/// What side does the player with the mouse control?
side get_mouse_user_player_side(const game_controller& c);

/// Get the physical controllers
const physical_controller& get_physical_controller(
  const game_controller& c,
  const side player_side
) noexcept;

/// Get the physical controller type
physical_controller_type get_physical_controller_type(
  const game_controller& c,
  const side player_side
) noexcept;

/// Create the user inputs to do action_1 at the square at the cursor
user_input get_user_input_to_do_action_1(
  const game_controller& c,
  const side player_side
);

/// Get the game users' inputs
const user_inputs& get_user_inputs(const game_controller& c) noexcept;

/// Create the user inputs to move the cursor to a target square
/// knowing it will be at the 'from' square.
/// This is useful for creating future 'user_input's,
/// e.g. for white doing e4, the cursor must be moved to e2
/// to select a pawn, then to e4 to select the target.
user_inputs get_user_inputs_to_move_cursor_from_to(
  const game_controller& c,
  const square& from,
  const square& to,
  const side player_side
);

/// Create the user inputs to move the cursor to a target square
user_inputs get_user_inputs_to_move_cursor_to(
  const game_controller& c,
  const square& to,
  const side player_side
);

/// Create the user inputs to select the square at the cursor
user_input get_user_input_to_select(
  const game_controller& c,
  const side player_side
);

/// Is there a player that uses the keyboard?
bool has_keyboard_controller(const game_controller& c);

/// Is there a player that uses the mouse?
bool has_mouse_controller(const game_controller& c);

/// The the player at that side a mouse user?
bool is_mouse_user(const game_controller& c, const side player_side) noexcept;


/// Put the cursor (i.e. the selector, not the mouse pointer)
/// at the desired square
void move_cursor_to(
  game_controller& c,
  const std::string& square_str,
  const side player_side
);

/// Put the cursor (i.e. the selector, not the mouse pointer)
/// at the desired square.
/// Does not select the square where the cursor is.
void move_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
);

/// Put the cursor (i.e. the selector)
/// at the desired square
/// @see \link{move_cursor_to} is more general
void move_keyboard_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
);

/// Put the cursor (i.e. the selector, not the mouse pointer)
/// at the desired square
/// @see \link{move_cursor_to} is more general
void move_mouse_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
);

/// Set the cursor's position to the target position
void set_cursor_pos(
  game_controller& c,
  const game_coordinat& pos,
  const side player_side
) noexcept;

/// Set the cursor's position to the target square
void set_cursor_pos(
  game_controller& c,
  const square& s,
  const side player_side
) noexcept;

/// The the cursor of the keyboard player to the desired square
void set_keyboard_player_pos(
  game_controller& c,
  const square& s
);

/// The the cursor of the mouse player to the desired square
void set_mouse_player_pos(
  game_controller& c,
  const square& s
);

/// Test this class and its free functions
void test_game_controller();

std::ostream& operator<<(std::ostream& os, const game_controller& g) noexcept;

#endif // GAME_CONTROLLER_H
