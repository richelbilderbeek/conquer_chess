#ifndef PIECE_ACTION_H
#define PIECE_ACTION_H

#include <iosfwd>
#include <string>
#include <vector>

#include "piece_action_type.h"
#include "game_coordinat.h"
#include "square.h"
#include "piece_type.h"

/// An action to be done by a piece, for example:
/// * Move from d1 to d7
/// * Attack e5
class piece_action
{
public:

  /// Move of move from a square to another
  explicit piece_action(
    const side player,
    const piece_type pt,
    const piece_action_type at,
    const square& from,
    const square& to
  );

  auto get_action_type() const noexcept { return m_action_type; }
  const auto& get_from() const noexcept { return m_from; }
  auto get_piece_type() const noexcept { return m_piece_type; }
  auto get_player() const noexcept { return m_player; }
  const auto& get_to() const noexcept { return m_to; }

private:

  piece_action_type m_action_type;
  square m_from;
  piece_type m_piece_type;
  side m_player;
  square m_to;
};

/// Describe the 'piece_action' in words, e.g. 'move to (3, 4)'
std::string describe_action(const piece_action& p);

/// Is the action atomic, i.e. it cannot be split up further.
/// For example, moving a queen d1 to d3 is not atomic,
/// as it is split up in d1 -> d2 -> d3
bool is_atomic(const piece_action& a) noexcept;

/// Test the 'piece_action' class and its free functions
void test_piece_action();

/// Convert an action to one or more atomic actions
std::vector<piece_action> to_atomic(const piece_action& a);

/// Convert to string
std::string to_str(const piece_action& a) noexcept;

/// Convert to string, one line per action, no newline at the end
std::string to_str(const std::vector<piece_action>& a) noexcept;

bool operator==(const piece_action& lhs, const piece_action& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const piece_action& p) noexcept;


#endif // PIECE_ACTION_H
