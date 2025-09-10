#ifndef PIECE_H
#define PIECE_H


#include "action_history.h"
#include "delta_t.h"
#include "chess_color.h"
#include "piece_type.h"
#include "piece_action.h"
#include "piece_id.h"
#include "message_type.h"
#include "race.h"
#include "read_only.h"

#include <iosfwd>
#include <string>
#include <vector>

/// A chess piece.
///
/// @seealso for drawing a piece and/or its health bar properly
///   use \link{piece_layout}
class piece
{
public:
  explicit piece(
    const chess_color color,
    const piece_type type,
    const square& coordinat,
    const race r = race::classic
  );
  explicit piece(
    const chess_color color,
    const piece_type type,
    const std::string& coordinat,
    const race r = race::classic
  );

  /// Add an action for the piece to do
  /// This function will split up the action in smaller atomic actions
  /// @see 'tick' processes the actions
  void add_action(const piece_action& action);

  /// Add a message, i.e. a thing to say for this piece
  void add_message(const message_type& message);

  /// Clear all the sound effects, i.e. remove all
  void clear_messages() noexcept;

  /// Get all the piece actions
  const auto& get_actions() const noexcept { return m_actions; }

  /// Get all the piece actions
  auto& get_actions() noexcept { return m_actions; }

  /// Get the actions history
  const auto& get_action_history() const noexcept { return m_action_history; }

  /// Get the color of the piece, i.e. white or black
  const auto& get_color() const noexcept { return m_color.get_value(); }

  /// The progress of the current action,
  /// which is a value from zero to (and including) one
  delta_t get_current_action_progress() const;

  const auto& get_current_square() const noexcept { return m_current_square; }

  /// Get the health of the unit
  double get_health() const noexcept { return m_health; }

  /// The unique piece ID
  const piece_id& get_id() const noexcept { return m_id; }

  /// Get the in-game time according to this piece
  const auto& get_in_game_time() const noexcept { return m_in_game_time; }

  /// Get the number of pieces this piece has killd
  int get_kill_count() const noexcept { return m_kill_count; }

  /// Get the maximum health of the unit
  double get_max_health() const noexcept { return m_max_health; }

  /// Get the maximum shield value of the unit
  ///
  /// Assume this unit is a Rooxx unit
  double get_max_shield() const;

  /// The things this piece wants to say
  const auto& get_messages() const noexcept { return m_messages; }

  /// Get the race of a piece
  const auto& get_race() const noexcept { return m_race.get_value(); }

  /// Get the shield value of the unit.
  ///
  /// Assumes this unit is a Rooxx unit
  double get_shield() const;

  /// Get the type of piece, e.g. king, queen, rook, bishop, knight, pawn
  const auto& get_type() const noexcept { return m_type.get_value(); }

  /// Has this piece (attempted to) moved?
  bool has_moved() const noexcept { return m_has_moved; }

  /// Increase the kill count by one
  void increase_kill_count() noexcept { ++m_kill_count; }

  /// Can the unit be captured by en-passant?
  /// Will fail if the piece is not a pawn
  bool is_enpassantable(const in_game_time& when) const;


  /// Receive damage
  /// @param damage a positive value
  void receive_damage(const double damage);

  /// Set the current progres an action has passed
  void set_current_action_progress(const delta_t& t) noexcept;

  /// Set the current/occupied square
  void set_current_square(const square& s) noexcept { m_current_square = s; }


  /// Do one frame of movement, resulting in a piece movement of 1 * delta_t
  /// @param delta_t the fraction of a full move that is done, where
  ///   0.01 means that only 1% of the full move is done and
  ///   1.0 denotes doing a full move.
  /// @param occupied_squares the squares that are occupied
  /// @see use 'add_action' to add an action to be processed
  void tick(
    const delta_t& dt,
    game& g
  );

private:

  /// The actions the piece is doing, or about to do
  std::vector<piece_action> m_actions;

  /// The history of actions, in chrononical order
  action_history m_action_history;

  /// The color of the piece, i.e. white or black
  read_only<chess_color> m_color;

  /// Time that the current action is taking
  /// Is always between 0.0 (start) and 1.0 (done).
  /// When it passes 1.0, the current action is done.
  delta_t m_current_action_progress;

  /// The square the piece occupies now
  square m_current_square;

  /// Has this piece (attempted to) move?
  bool m_has_moved;

  /// The health
  double m_health;

  /// The piece its ID
  ///
  /// A piece needs an ID, so that the `game_controller` can keep
  /// track of a piece that is moving.
  piece_id m_id;

  /// The in-game time (in chess moves)
  in_game_time m_in_game_time;

  /// The number of pieces killed by this one
  int m_kill_count;

  /// The maximum health
  double m_max_health;

  /// The maximum shield, only for Rooxx
  double m_max_shield;

  /// The things this piece wants to say
  std::vector<message_type> m_messages;

  /// The race of this piece
  read_only<race> m_race;

  /// The shield value, only for Rooxx
  double m_shield;

  /// The type of piece, e.g. king, queen, rook, bishop, knight, pawn
  read_only<piece_type> m_type;
};

/// Can a piece attack from 'from' to 'to'?
/// This function assumes the board is empty
bool can_attack_on_empty_board(
  const chess_color color,
  const piece_type& p,
  const square& from,
  const square& to
);

/// Can a piece capture from 'from' to 'to'?
/// This function assumes the board is empty
bool can_capture(
  const piece_type& p,
  const square& from,
  const square& to,
  const chess_color color
);

/// Can a piece move from 'from' to 'to'?
/// This function assumes the board is empty
bool can_move_on_empty_board(
  const chess_color color,
  const piece_type& p,
  const square& from,
  const square& to
);

/// Can this piece promote?
bool can_promote(
  const chess_color color,
  const piece_type& p,
  const square& s
) noexcept;

/// Can this piece promote?
bool can_promote(const piece& p) noexcept;

/// Clear all the actions
void clear_actions(piece& p);

/// Count the number of actions a piece has
int count_piece_actions(const piece& p);

/// Describe the actions a piece have, e.g. 'idle', or 'moving to (3, 4)'
std::string describe_actions(const piece& p);

/// Get the fraction of the health, where 1.0 denotes full health
double get_f_health(const piece& p) noexcept;

/// Get the fraction of the shield, where 1.0 denotes full shields
///
/// Assumes the piece is a Rooxx
double get_f_shield(const piece& p) noexcept;

/// Get the square that this piece occupies now
square get_occupied_square(const piece& p) noexcept;

/// Create a piece to be used in testing: a white king on e1
piece get_test_white_king() noexcept;

/// Create a piece to be used in testing: a white knight on c3
piece get_test_white_knight(const race r = race::classic) noexcept;

/// Does the piece have actions to do?
bool has_actions(const piece& p) noexcept;

/// Has this piece (i.e. a pawn) did a double move forward last time unit?
/// Only pawns can do this, e.g. e2-e4 or e7-e5
bool has_just_double_moved(
  const piece& p,
  const in_game_time& when
) noexcept;

/// Has this piece (attempted to) move?
bool has_moved(const piece& p) noexcept;


/// Is the unit dead?
bool is_dead(const piece& p) noexcept;

/// Can the unit be captured by en-passant?
bool is_enpassantable(
  const piece& p,
  const in_game_time& when
);

/// Is the unit idle?
bool is_idle(const piece& p) noexcept;

/// Is this piece (a pawn) on the starting rank?
bool is_on_starting_rank(const chess_color c, const square& q) noexcept;

/// Is the piece a pawn?
bool is_pawn(const piece& p) noexcept;

/// Determine from the action history if the piece is selected
bool is_selected(const piece& p);

/// Test this class and its free functions
void test_piece();

/// Process a tick, when the current action is an attack
///
/// An attack only does damage, the actual capture is processed
/// by game::do_captures
void tick_attack(
  piece& p,
  const delta_t& dt,
  game& g
);

/// Process a tick, when the current action is an en-passant attack
///
/// An attack only does damage, the actual capture is processed
/// by game::do_captures
void tick_attack_en_passant(
  piece& p,
  const delta_t& dt,
  game& g
);

/// Process a tick, when the current action is a castling to kingside
void tick_castle_kingside(
  piece& p,
  const delta_t& dt,
  game& g
);

/// Process a tick, when the current action is a castling to queenside
void tick_castle_queenside(
  piece& p,
  const delta_t& dt,
  game& g
);

/// Process a tick, when the current action is a move
void tick_move(
  piece& p,
  const delta_t& dt,
  game& g
);

/// Convert the piece to a FEN character
///
/// Type  |Character when white|Character when black
/// ------|--------------------|--------------------
/// bishop| b                  | B
/// king  | k                  | K
/// knight| n                  | N
/// pawn  | p                  | P
/// queen | q                  | Q
/// rook  | r                  | R
char to_fen_char(const piece& p) noexcept;

bool operator==(const piece& lhs, const piece& rhs) noexcept;
bool operator!=(const piece& lhs, const piece& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const piece& p) noexcept;

#endif // PIECE_H
