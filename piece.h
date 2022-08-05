#ifndef PIECE_H
#define PIECE_H

#include <iosfwd>

#include "delta_t.h"
#include "chess_color.h"
#include "id.h"
#include "piece_type.h"
#include "piece_action.h"
#include "game_coordinat.h"
#include "message.h"
#include "message_type.h"
#include "starting_position_type.h"
#include "side.h"

#include <string>
#include <vector>

/// A chess piece
class piece
{
public:
  explicit piece(
    const chess_color color,
    const piece_type type,
    const square& coordinat
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

  /// Get the color of the piece, i.e. white or black
  chess_color get_color() const noexcept { return m_color; }

  const auto& get_current_action_time() const noexcept { return m_current_action_time; }

  const auto& get_current_square() const noexcept { return m_current_square; }

  /// Get the health of the unit
  double get_health() const noexcept { return m_health; }

  /// Get the ID of the piece
  const auto& get_id() const noexcept { return m_id; }

  /// Get the number of pieces this piece has killd
  int get_kill_count() const noexcept { return m_kill_count; }

  /// Get the maximum health of the unit
  double get_max_health() const noexcept { return m_max_health; }

  /// The things this piece wants to say
  const auto& get_messages() const noexcept { return m_messages; }

  /// Get the type of piece, e.g. king, queen, rook, bishop, knight, pawn
  piece_type get_type() const noexcept { return m_type; }

  /// Increase the kill count by one
  void increase_kill_count() noexcept { ++m_kill_count; }

  /// Is the piece selected?
  bool is_selected() const noexcept { return m_is_selected; }

  /// Receive damage
  /// @param damage a positive value
  void receive_damage(const double damage);

  /// Set the current time an action has passed
  void set_current_action_time(const delta_t& t) noexcept;

  /// Set the current/occupied square
  void set_current_square(const square& s) noexcept { m_current_square = s; }

  /// Set the selectedness of the piece
  void set_selected(bool is_selected) noexcept;

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

  /// The color of the piece, i.e. white or black
  chess_color m_color;

  /// Time that the current action is taking
  delta_t m_current_action_time;

  /// The square the piece occupies now
  square m_current_square;

  /// The health
  double m_health;

  /// The unique ID of this piece
  id m_id;

  /// Is this piece selected?
  bool m_is_selected;

  /// The number of pieces killed by this one
  int m_kill_count;

  /// The maximum health
  double m_max_health;

  /// The things this piece wants to say
  std::vector<message_type> m_messages;

  /// The square the piece is moving to, if any
  std::vector<square> m_target_square;

  /// The type of piece, e.g. king, queen, rook, bishop, knight, pawn
  piece_type m_type;
};

/// Can a piece attack from 'from' to 'to'?
/// This function assumes the board is empty
bool can_attack(
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
bool can_move(
  const chess_color color,
  const piece_type& p,
  const square& from,
  const square& to
);

/// Clear all the actions
void clear_actions(piece& p);

/// Count the number of actions a piece has
int count_piece_actions(const piece& p);

/// Describe the actions a piece have, e.g. 'idle', or 'moving to (3, 4)'
std::string describe_actions(const piece& p);

/// Get the fraction of the health, where 1.0 denotes full health
double get_f_health(const piece& p) noexcept;

/// Get the square that this piece occupies now
square get_occupied_square(const piece& p) noexcept;

/*
/// Rotate the coordinator of the piece,
/// i.e. turn the board 180 degrees
piece get_rotated_piece(const piece& piece) noexcept;
*/

/// Create a piece to be used in testing: a white king on e1
piece get_test_white_king() noexcept;

/// Create a piece to be used in testing: a white knight on c3
piece get_test_white_knight() noexcept;

/// Does the piece have actions to do?
bool has_actions(const piece& p) noexcept;

/// Is the unit dead?
bool is_dead(const piece& p) noexcept;

/// Is the unit idle?
bool is_idle(const piece& p) noexcept;

/// Select the piece
void select(piece& p) noexcept;

/// Test this class and its free functions
void test_piece();

/// Process a tick, when the current action is an attack
void tick_attack(
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

/// Select the piece
void toggle_select(piece& p) noexcept;

/// Unselect the piece
void unselect(piece& p) noexcept;

bool operator==(const piece& lhs, const piece& rhs) noexcept;
bool operator!=(const piece& lhs, const piece& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const piece& p) noexcept;

#endif // PIECE_H
