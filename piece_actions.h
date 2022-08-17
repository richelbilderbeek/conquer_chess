#ifndef PIECE_ACTIONS_H
#define PIECE_ACTIONS_H

/// Work on collections of piece_action
///
#include <iosfwd>
#include <vector>

#include "piece_action.h"

/// Collect all the squares that are attacked by each color
std::vector<std::pair<square, chess_color>>
  collect_attacked_squares(const std::vector<piece_action>& actions);

/// Concatenate the vectors
std::vector<piece_action> concatenate(
  const std::vector<piece_action>& lhs_actions,
  const std::vector<piece_action>& rhs_actions
);

bool is_in(const piece_action& action, const std::vector<piece_action>& actions) noexcept;

/// Is the square attacked by a certain (enemy) color?
bool is_square_attacked_by(
  const std::vector<std::pair<square, chess_color>> attacked_squares,
  const square& s,
  const chess_color enemy_color
);

std::ostream& operator<<(std::ostream& os, const std::vector<piece_action>& p) noexcept;

void test_piece_actions();

#endif // PIECE_ACTIONS_H
