#ifndef ACTIONS_H
#define ACTIONS_H

#include "ccfwd.h"

#include "chess_color.h"
#include "user_input.h"
#include "piece_action.h"
#include "message.h"

#include <iostream>
#include <vector>

/// The actions in a game, with two types:
///  * The control_actions are what the user wants.
///    These actions are immediate, processed each tick, and
///    possibly converted to piece_actions
///  * The piece_actions are ongoing piece_actions
///    e.g. move from e2 to e3, then from e3 to e4
/// The control_actions processes user actions
/// and passes the into the pieces it concerns, as 'piece_actions'.
/// There, the pieces take over.
class user_inputs
{
public:
  user_inputs();

  /// Add a new user actions
  void add(const user_input& action);

  const auto& get_actions() const noexcept { return m_control_actions; }

  /// Process all actions and apply these on the game
  void process(game& g);

private:

  std::vector<user_input> m_control_actions;
};

/// Count the total number of control actions to be done by the game,
/// which should be zero after each tick
int count_user_inputs(const user_inputs& a);

/// Count the total number of piece actions to be done by the game
int count_piece_actions(const user_inputs& a);

/// Create the control actions to do a PGN move from a certain
/// game. The game is needed to determine how the cursor
/// is moved (e.g. by mouse/keyboard, if keyboard:
/// how many squares)
user_inputs create_control_actions(
  const std::string& pgn_str,
  const chess_color color,
  const game& g
);

/// Create the control actions to do a PGN move from a certain
/// game. The game is needed to determine how the cursor
/// is moved (e.g. by mouse/keyboard, if keyboard:
/// how many squares)
user_inputs create_control_actions(
  const chess_move& m,
  const game& g
);

/// Process a left-mouse-button, hence a game_coordinat as a coordinat
void do_select(
  game& g,
  const game_coordinat& coordinat,
  const chess_color player_color
);

/// Process a space, hence a square as a coordinat
void do_select(
  game& g,
  const square& coordinat,
  const chess_color player_color
);

/// Select a piece
void do_select(
  game& g,
  const std::string& square_str,
  const chess_color player_color
);

/// Select a piece
void do_select(
  game& g,
  const std::string& square_str,
  const side player_side
);

/// Select a piece and move it to a target square
void do_select_and_move_piece(
  game& g,
  const std::string& from_square_str,
  const std::string& to_square_str,
  const side player_side
);

/// Respond to action 1
void process_press_action_1(game& g, const user_input& action);

/// Respond to action 1 or LMB down
void process_press_action_1_or_lmb_down(game& g, const user_input& action);

/// Respond to action 2
void process_press_action_2(game& g, const user_input& action);

/// Respond to action 3
void process_press_action_3(game& g, const user_input& action);

/// Respond to action 4
void process_press_action_4(game& g, const user_input& action);

/// Process an A or right-mouse-button down
void start_attack(
  game& g,
  const game_coordinat& coordinat,
  const chess_color player_color
);

/// Process an M or right-mouse-button down
void start_move_unit(
  game& g,
  const game_coordinat& coordinat,
  const chess_color player_color
);

/// Test this class and its free functions
void test_control_actions();

/// Convert a 'piece_action' to a 'control_actions'
user_inputs to_user_inputs(const piece_action& pa, const game& g);

std::ostream& operator<<(std::ostream& os, const user_inputs& actions) noexcept;
bool operator==(const user_inputs& lhs, const user_inputs& rhs) noexcept;

#endif // ACTIONS_H