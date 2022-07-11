#ifndef PIECE_H
#define PIECE_H

#include "chess_color.h"
#include "piece_type.h"

#include <vector>

/// A chesspiece
class piece
{
public:
  piece(
    const chess_color color,
    const piece_type type,
    const double x,
    const double y
  );

  /// Get the color of the piece, i.e. white or black
  chess_color get_color() const noexcept { return m_color; }

  /// Get the fraction of the health, where 1.0 denotes full health
  double get_f_health() const noexcept { return m_f_health; }

  /// Get the type of piece, e.g. king, queen, rook, bishop, knight, pawn
  piece_type get_type() const noexcept { return m_type; }

  /// Get the x coordinat of the piece, where 0 is file A, 1 is file B, etc.
  double get_x() const noexcept { return m_x; }

  /// Get the y coordinat of the piece, where 0 is rank 1, 1 is rank 2, etc.
  double get_y() const noexcept { return m_y; }

private:

  /// The color of the piece, i.e. white or black
  chess_color m_color;

  /// The fraction of the health, where 1.0 denotes full health
  double m_f_health;

  /// The type of piece, e.g. king, queen, rook, bishop, knight, pawn
  piece_type m_type;

  double m_x;
  double m_y;
};

/// Get all the pieces in the starting position
std::vector<piece> get_starting_pieces() noexcept;

#endif // PIECE_H
