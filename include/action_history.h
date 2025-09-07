#ifndef ACTION_HISTORY_H
#define ACTION_HISTORY_H

#include "ccfwd.h"
#include "in_game_time.h"
#include "piece_action.h"

#include <iosfwd>
#include <vector>

/// The collected in-game times of actions,
/// in chrononical order.
///
/// With the time and the full piece action, one can perfectly replay
/// a game.
class action_history
{
public:
  action_history(
    const std::vector<std::pair<in_game_time, piece_action>>& timed_actions  = {}
  );

  /// Add an action, when started
  void add_action(const in_game_time& in_game_time, const piece_action& action) noexcept;

  /// Get the collected in-game times of actions,
  /// in chrononical order.
  /// Or: the first action ever is at index 0,
  /// where the most recent action is at the back
  const auto& get() const noexcept { return m_timed_actions; }

private:

  /// The history of actions (i.e when they started), in chrononical order
  std::vector<std::pair<in_game_time, piece_action>> m_timed_actions;

};

/// Collect all the actions that started in the timespan
std::vector<piece_action> collect_actions_in_timespan(
  const action_history& history,
  const in_game_time from,
  const in_game_time to
);

/// Create an action history from a game
action_history create_action_history_from_game(const game& g);

/// Create an action history from a PGN string
action_history create_action_history_from_pgn(const pgn_game_string& s);

#ifdef REALLY_NEED_ISSUE_153
// This is quite a complex beast!

/// Create an xdotool script from a PGN string
/// @param s a PGN string
/// @param r_filename path to the to-be-made R scrip
/// @example
/// const pgn_game_string s("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0");
/// create_r_scrip_from_pgn(s, "play_scholars_mate.R");
void create_r_script_from_pgn(const pgn_game_string& s, const std::string& r_filename);
#endif // REALLY_NEED_ISSUE_153


/// Get the number of moves in the action history
int get_n_piece_actions(const action_history& r) noexcept;

/// Has this piece (i.e. a pawn) just finish a double move forward last time unit?
/// Only pawns can do this, e.g. e2-e4 or e7-e5
/// For example, when at t=0.5, e2-e4 is played,
/// from t=1.5 to t=2.5 this function will return true.
/// Before that, the piece was moving
/// After that, it was too long ago for en-passant
bool has_just_double_moved(
  const action_history& action_history,
  const in_game_time when
);

const piece_action& get_last_action(const action_history& history);

bool has_actions(const action_history& history) noexcept;

/// Can this piece (i.e. a pawn) be captured by en-passant?
/// Only pawns can do this, e.g. e2-e4 or e7-e5
/// For example, when at t=0.5, e2-e4 is played,
/// from t=1.5 to t=2.5 this function will return true.
/// Before that, the piece was moving
/// After that, it was too long ago for en-passant
bool is_enpassantable(
  const action_history& action_history,
  const in_game_time when
);


/// Is this piece selected, according to its action history
bool is_piece_selected(const action_history& h);

/// Combine action histories and sort these
action_history merge_action_histories(const std::vector<action_history> histories);

/// Test this class and its free functions
void test_action_history();

std::string to_str(const action_history& history) noexcept;

bool operator==(const action_history& lhs, const action_history& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const action_history& history) noexcept;

#endif // ACTION_HISTORY_H
