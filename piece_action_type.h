#ifndef PIECE_ACTION_TYPE_H
#define PIECE_ACTION_TYPE_H

#include <iosfwd>

/// The type of actions a piece can do
enum class piece_action_type
{
  move,
  attack
};

std::ostream& operator<<(std::ostream& os, const piece_action_type& p);


#endif // PIECE_ACTION_TYPE_H
