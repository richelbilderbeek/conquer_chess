#ifndef PIECE_ACTION_TYPE_H
#define PIECE_ACTION_TYPE_H

#include <iosfwd>
#include <vector>

/// The type of actions a piece can do
enum class piece_action_type
{
  attack, // a regular attack, not en-passant
  castle_kingside,
  castle_queenside,
  en_passant, // a type of attack
  move,
  promote_to_bishop,
  promote_to_knight,
  promote_to_queen,
  promote_to_rook,
  select,   // select and unselect are valid piece action,
  unselect  // as the GUI can suggest these as the default (piece) action
};

/// Get all the \link{piece_action_type}s
std::vector<piece_action_type> get_all_piece_action_types() noexcept;

/// Test the piece_action_type functions
void test_piece_action_type();

/// Convert to short, human-readable, string
std::string to_human_str(const piece_action_type t) noexcept;

/// Convert to string
std::string to_str(const piece_action_type t) noexcept;

std::ostream& operator<<(std::ostream& os, const piece_action_type& p) noexcept;


#endif // PIECE_ACTION_TYPE_H
