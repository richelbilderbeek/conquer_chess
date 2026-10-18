#ifndef REPLAY_H
#define REPLAY_H

#include "ccfwd.h"
#include "delta_t.h"
#include "in_game_time.h"
#include "game_controller.h"
#include "action_history.h"
#include "game_statistics_in_time.h"

#include <iosfwd>

/// A replay.
///
/// A replay plays a game again by playing the action history of a game.
///
/// Use 'do_move' to go forward in time.
///
/// @param h the \link{action_history},
///   as can be created by, e.g. \link{create_action_history_from_pgn}
///   or \link{create_action_history_from_game}
class replay
{
public:
  explicit replay(
    const action_history& h = action_history(),
    const game_controller& c = game_controller()
  );

  /// Do a move or do nothing, for 1 time unit
  void do_move(const delta_t& dt = delta_t(1.0));

  /// Get the history of actions, from a replay
  const auto& get_action_history() const noexcept { return m_action_history; }

  /// Get the current state of the game
  const auto& get_game() const noexcept { return m_game_controller.get_game(); }

  /// Get the current state of the game
  const auto& get_game_controller() const noexcept { return m_game_controller; }

  /// The index of the next move to do in the action history
  const auto& get_index() const noexcept { return m_index; };

  /// Get the current state of the game
  const auto& get_initial_game_controller() const noexcept { return m_initial_game_controller.get_value(); }

  bool is_done() const noexcept;

  /// Rewind back to the start
  void reset() noexcept;

private:

  /// The history of actions, from a replay
  read_only<action_history> m_action_history;

  /// The current state of the game
  game_controller m_game_controller;

  /// The index of the next move to do in m_action_history
  int m_index;

  /// The initial state of the game controller
  read_only<game_controller> m_initial_game_controller;
};

/// Extract the game statistics over time
///
/// @param r a replayer
/// @param dt the interval at which the statistics are extracted
game_statistics_in_time extract_game_statistics_in_time(
  const replay& r,
  const delta_t& dt
);

const in_game_time& get_in_game_time(
  const replay& r
) noexcept;

/// Get the number of moves in the replay
int get_n_moves(const replay& r) noexcept;

replay get_played_scholars_mate();

replay get_played_kasparov_vs_topalov();


/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const replay& r,
  const square& coordinat
);

/// Test this class and its free functions
void test_replay();


bool operator==(const replay& lhs, const replay& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const replay& r) noexcept;

#endif // REPLAY_H
