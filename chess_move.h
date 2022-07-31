#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include <string>
#include <vector>

#include "piece_type.h"
#include "square.h"

/// A chess move, e.g. 'e4'
// Call it 'chess_move' instead of 'move' to avoid conflicts with 'std::move'
class chess_move
{
public:
  chess_move(std::string s);

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
  bool m_is_capture;

  std::vector<square> m_to;

  std::vector<piece_type> m_type;

  std::vector<chess_color> m_winner;

};

/// Conclude if the move is a capture from a PGN string
/// E.g. 'Nxc3' will result in true
bool get_is_capture(const std::string& pgn_str);

/// Get the square from a string
/// E.g. 'Nc3' will result in 'c3'
square get_square(const std::string& pgn_str);

/// Get a piece type for a string
/// E.g. 'Nc3' will result in a knight, 'e4' will result in a pawn
piece_type get_piece_type(const std::string& pgn_str);

/// Get the winner from a notation.
/// Assumes a win ('1-0' or '0-1') or a draw ('1/2-1/2')
std::vector<chess_color> get_winner(const std::string& pgn_str);

/// Test this class and its free functions
void test_chess_move();

#endif // CHESS_MOVE_H
