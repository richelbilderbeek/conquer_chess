#ifndef PIECE_ACTION_TYPE_H
#define PIECE_ACTION_TYPE_H

#include <iosfwd>

/// The type of actions a piece can do
enum class piece_action_type
{
  move,
  attack,
  promote,
  castle_kingside,
  castle_queenside
};

/// Convert to string
std::string to_str(const piece_action_type t) noexcept;

/// Test the piece_action_type functions
void test_piece_action_type();

std::ostream& operator<<(std::ostream& os, const piece_action_type& p) noexcept;


#endif // PIECE_ACTION_TYPE_H
