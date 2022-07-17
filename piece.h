#ifndef PIECE_H
#define PIECE_H

#include "chess_color.h"
#include "piece_type.h"
#include "piece_action.h"
#include "game_coordinat.h"
#include "sound_effect_type.h"

#include <string>
#include <vector>

/// A chesspiece
class piece
{
public:
  piece(
    const chess_color color,
    const piece_type type,
    const game_coordinat& coordinat
  );

  /// Add an action for the piece to do
  /// @see 'tick' processes the actions
  void add_action(const piece_action& action);

  /// Get all the piece actions
  const auto& get_actions() const noexcept { return m_actions; }

  /// Get all the piece actions
  auto& get_actions() noexcept { return m_actions; }

  /// Get the color of the piece, i.e. white or black
  chess_color get_color() const noexcept { return m_color; }

  /// Get the coordinat at the center of the piece
  game_coordinat get_coordinat() const noexcept { return m_coordinat; }

  /// Get the health of the unit
  double get_health() const noexcept { return m_health; }

  /// Get the maximum health of the unit
  double get_max_health() const noexcept { return m_max_health; }


  /// Get the type of piece, e.g. king, queen, rook, bishop, knight, pawn
  piece_type get_type() const noexcept { return m_type; }

  /// Is the piece selected?
  bool is_selected() const noexcept { return m_is_selected; }

  /// Set the selectedness of the piece
  void set_selected(bool is_selected) noexcept;

  /// Do one frame of movement, resulting in a piece movement of 1 * delta_t
  /// @param delta_t the fraction of a full move that is done, where
  ///   0.01 means that only 1% of the full move is done and
  ///   1.0 denotes doing a full move.
  /// @see use 'add_action' to add an action to be processed
  void tick(const double delta_t);

private:

  /// The actions the piece is doing, or about to do
  std::vector<piece_action> m_actions;

  /// The color of the piece, i.e. white or black
  chess_color m_color;

  /// The in-game coordinat
  game_coordinat m_coordinat;

  /// The health
  double m_health;

  /// Is this piece selected?
  bool m_is_selected;

  /// The maximum health
  double m_max_health;

  /// The type of piece, e.g. king, queen, rook, bishop, knight, pawn
  piece_type m_type;
};

/// Clear all the actions
void clear_actions(piece& p);

/// Count the number of actions a piece has
int count_piece_actions(const piece& p);

/// Describe the actions a piece have, e.g. 'idle', or 'moving to (3, 4)'
std::string describe_actions(const piece& p);

/// Get all the pieces in the starting position
std::vector<piece> get_default_starting_pieces() noexcept;

/// Get the fraction of the health, where 1.0 denotes full health
double get_f_health(const piece& p) noexcept;

/// Get a king-versus-king starting position
std::vector<piece> get_king_versus_king_starting_pieces() noexcept;


/// Does the piece have actions to do?
bool has_actions(const piece& p) noexcept;

/// Is the unit idle?
bool is_idle(const piece& p) noexcept;

/// Select the piece
void select(piece& p) noexcept;

/// Select the piece
void toggle_select(piece& p) noexcept;

/// Unselect the piece
void unselect(piece& p) noexcept;

#endif // PIECE_H
