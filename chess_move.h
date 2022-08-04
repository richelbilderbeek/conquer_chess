#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <string>
#include <vector>
#include <iosfwd>

#include "castling_type.h"
#include "piece_type.h"
#include "square.h"

/// A chess move, e.g. 'e4'
// Call it 'chess_move' instead of 'move' to avoid conflicts with 'std::move'
class chess_move
{
public:
  /// Need to now the player's color, e.g. for 'O-O-O' or '1-0'
  explicit chess_move(std::string pgn_str, const chess_color color);

  /// Get the castling type.
  /// Will be empty if this move is not a promotion
  const auto& get_castling_type() const noexcept { return m_castling_type; };

  /// Get the color of the player that did this move
  const auto& get_color() const noexcept { return m_color; };

  /// Get the original PGN string back
  const auto& get_pgn_str() const noexcept { return m_pgn_str; }

  /// Get the promotion type.
  /// Will be empty if this move is not a promotion
  const auto& get_promotion_type() const noexcept { return m_promotion_type; };

  /// Get the target square, e.g. 'e4' in 'Pxe4'
  /// Result will be empty when castling (e.g. 'O-O') or winning (e.g. '1-0')
  const auto& get_to() const noexcept { return m_to; }

  /// The type of chess piece
  /// Result will be empty when castling (e.g. 'O-O') or winning (e.g. '1-0')
  const auto& get_type() const noexcept { return m_type; }

  /// Get the winner
  /// If this is empty, the game is still on-going
  /// If this has 1 element, that color is the winner
  /// If this has 2 elements, the game ended in a draw
  const auto& get_winner() const noexcept { return m_winner; }

  /// Is this move a capture?
  /// E.g. 'Qxf7' is a capture
  bool is_capture() const noexcept { return m_is_capture; }

private:

  /// The type of castling. Empty if move is not a castling
  std::vector<castling_type> m_castling_type;

  chess_color m_color;

  bool m_is_capture;

  /// The original PGN string
  std::string m_pgn_str;

  /// The type of piece a pawn promotes into. Empty if move is not a promotion
  std::vector<piece_type> m_promotion_type;

  std::vector<square> m_to;

  std::vector<piece_type> m_type;

  std::vector<chess_color> m_winner;

};

/// Get the square from a string
/// E.g. 'Nc3' will result in 'c3'
square get_square(const std::string& pgn_str);

/// Get a piece type for a string
/// E.g. 'Nc3' will result in a knight, 'e4' will result in a pawn
piece_type get_piece_type(const std::string& pgn_str);

/// Get the type the piece is promoted to.
/// Will be empty if this is no promotion
std::vector<piece_type> get_promotion_type(const std::string& pgn_str);

/// Get the winner from a notation.
/// Assumes a win ('1-0' or '0-1') or a draw ('1/2-1/2')
std::vector<chess_color> get_winner(const std::string& pgn_str);

/// Conclude if the move is a capture from a PGN string
/// E.g. 'Nxc3' will result in true
bool is_capture(const std::string& pgn_str);

/// Get if the move is a capture
bool is_capture(const chess_move& move) noexcept;

/// Get if the move is a castling
bool is_castling(const chess_move& move) noexcept;

/// Get if the move is a win
bool is_draw(const chess_move& move) noexcept;

/// Get if the move is a move,
/// i.e. if it moves one piece, without a capture,
/// no castling, no promotion, no win and no draw
bool is_simple_move(const chess_move& move) noexcept;

/// Get if the move is a promotion
bool is_promotion(const chess_move& move) noexcept;

/// Get if the move is a win
bool is_win(const chess_move& move) noexcept;

/// Test this class and its free functions
void test_chess_move();

bool operator==(const chess_move& lhs, const chess_move& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const chess_move& m) noexcept;

#endif // CHESS_MOVE_H
