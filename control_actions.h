#ifndef ACTIONS_H
#define ACTIONS_H

#include "ccfwd.h"

#include "chess_color.h"
#include "control_action.h"
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
class control_actions
{
public:
  control_actions();

  /// Add a new user actions
  void add(const control_action& action);

  const auto& get_actions() const noexcept { return m_control_actions; }

  /// Process all actions and apply these on the game
  void process(game& g);

private:

  std::vector<control_action> m_control_actions;
};

/// Count the total number of control actions to be done by the game,
/// which should be zero after each tick
int count_control_actions(const control_actions& a);

/// Count the total number of piece actions to be done by the game
int count_piece_actions(const control_actions& a);

/// Create the control actions to do a PGN move from a certain
/// game. The game is needed to determine how the cursor
/// is moved (e.g. by mouse/keyboard, if keyboard:
/// how many squares)
control_actions create_control_actions(
  const std::string& pgn_str,
  const chess_color color,
  const game& g
);

/// Create the control actions to do a PGN move from a certain
/// game. The game is needed to determine how the cursor
/// is moved (e.g. by mouse/keyboard, if keyboard:
/// how many squares)
control_actions create_control_actions(
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

/// Respond to action 1
void process_press_action_1(game& g, const control_action& action);

/// Respond to action 2
void process_press_action_2(game& g, const control_action& action);

/// Respond to action 3
void process_press_action_3(game& g, const control_action& action);

/// Respond to action 4
void process_press_action_4(game& g, const control_action& action);

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
control_actions to_control_actions(const piece_action& pa, const game& g);

std::ostream& operator<<(std::ostream& os, const control_actions& actions) noexcept;
bool operator==(const control_actions& lhs, const control_actions& rhs) noexcept;

#endif // ACTIONS_H
