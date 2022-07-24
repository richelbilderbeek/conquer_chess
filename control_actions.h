#ifndef ACTIONS_H
#define ACTIONS_H

#include "ccfwd.h"

#include "chess_color.h"
#include "control_action.h"
#include "piece_action.h"
#include "sound_effect.h"

#include <vector>

/// The actions in a game, with two types:
///  * The control_actions are what the user wants.
///    These actions are immediate, processed each tick, and
///    possibly converted to piece_actions
///  * The piece_actions are ongoing piece_actions
///    e.g. move from e2 to e3, then from e3 to e4
class control_actions
{
public:
  control_actions();

  /// Add a new user actions
  void add(const control_action& action);

  /// Clear the sound effects to be processed,
  /// i.e. resize to zero
  void clear_sound_effects() noexcept;

  const auto& get_actions() const noexcept { return m_control_actions; }

  /// Get all the sound effects to be processed
  const auto& get_sound_effects() const noexcept { return m_sound_effects; };

  /// Process all actions and apply these on the game
  void process(game& g, const delta_t& dt);

private:

  std::vector<control_action> m_control_actions;
  std::vector<sound_effect> m_sound_effects;

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

  void process_control_actions(game& g);
  void process_piece_actions(game& g, const delta_t& dt);

  /// Process an M or right-mouse-button down
  void start_move_unit(
    game& g,
    const game_coordinat& coordinat,
    const chess_color player_color
  );
};

/// Count the total number of control actions to be done by the game,
/// which should be zero after each tick
int count_control_actions(const control_actions& a);

/// Count the total number of piece actions to be done by the game
int count_piece_actions(const control_actions& a);

/// Get all the sound effects to be processed
const std::vector<sound_effect>& get_sound_effects(const control_actions& a) noexcept;

#endif // ACTIONS_H
