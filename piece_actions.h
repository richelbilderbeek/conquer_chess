#ifndef PIECE_ACTIONS_H
#define PIECE_ACTIONS_H

/// Work on collections of piece_action
///
#include <iosfwd>
#include <vector>

#include "piece_action.h"

bool is_in(const piece_action& action, const std::vector<piece_action>& actions) noexcept;

std::ostream& operator<<(std::ostream& os, const std::vector<piece_action>& p) noexcept;

void test_piece_actions();

#endif // PIECE_ACTIONS_H
