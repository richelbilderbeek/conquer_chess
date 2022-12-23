#ifndef PIECE_ACTION_TYPE_H
#define PIECE_ACTION_TYPE_H

#include <iosfwd>

/// The type of actions a piece can do
enum class piece_action_type
{
  select,
  unselect,
  move,
  attack, // will capture the attacked piece
  en_passant, // a type of attack
  promote_to_queen,
  promote_to_rook,
  promote_to_bishop,
  promote_to_knight,
  castle_kingside,
  castle_queenside
};

/// Convert to string
std::string to_str(const piece_action_type t) noexcept;

/// Test the piece_action_type functions
void test_piece_action_type();

std::ostream& operator<<(std::ostream& os, const piece_action_type& p) noexcept;


#endif // PIECE_ACTION_TYPE_H
