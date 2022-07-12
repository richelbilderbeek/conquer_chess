#ifndef PIECE_H
#define PIECE_H

#include "chess_color.h"
#include "piece_type.h"
#include "piece_action.h"
#include "game_coordinat.h"

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

  /// Do a left-mouse button down, to un-/select this piece
  void do_lmb_down();

  const auto& get_actions() const noexcept { return m_actions; }

  /// Get the color of the piece, i.e. white or black
  chess_color get_color() const noexcept { return m_color; }

  game_coordinat get_coordinat() const noexcept { return m_coordinat; }

  /// Get the health of the unit
  double get_health() const noexcept { return m_health; }

  /// Get the fraction of the health, where 1.0 denotes full health
  double get_f_health() const noexcept { return m_health / m_max_health; }

  /// Get the maximum health of the unit
  double get_max_health() const noexcept { return m_max_health; }

  /// Get the type of piece, e.g. king, queen, rook, bishop, knight, pawn
  piece_type get_type() const noexcept { return m_type; }

  /// Is the piece selected?
  bool is_selected() const noexcept { return m_is_selected; }

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

/// Get all the pieces in the starting position
std::vector<piece> get_starting_pieces() noexcept;

/// Does the piece have actions to do?
bool has_actions(const piece& p) noexcept;

/// Is the unit idle?
bool is_idle(const piece& p) noexcept;




#endif // PIECE_H
