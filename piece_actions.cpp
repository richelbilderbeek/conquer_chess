#include "piece_actions.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

std::vector<std::pair<square, chess_color>>
  collect_attacked_squares(const std::vector<piece_action>& actions)
{
  std::vector<std::pair<square, chess_color>> v;
  for (const auto& action: actions)
  {
     if (action.get_action_type() == piece_action_type::move)
     {
       v.push_back(std::make_pair(action.get_to(), action.get_color()));
     }
  }
  return v;
}

bool is_in(const piece_action& action, const std::vector<piece_action>& actions) noexcept
{
  return std::find(std::begin(actions), std::end(actions), action) != std::end(actions);
}

bool is_square_attacked_by(
  const std::vector<std::pair<square, chess_color>> attacked_squares,
  const square& s,
  const chess_color enemy_color
)
{
  const auto p{std::make_pair(s, enemy_color)};
  return std::find(
    std::begin(attacked_squares),
    std::end(attacked_squares),
    p
  ) != std::end(attacked_squares);
}

void test_piece_actions()
{
#ifndef NDEBUG
  // is_in
  {
    const auto action{
      piece_action(chess_color::white, piece_type::king, piece_action_type::move, square("e1"), square("e2"))
    };
    std::vector<piece_action> actions;
    assert(!is_in(action, actions));
    actions.push_back(action);
    assert(is_in(action, actions));
  }
  // operator<<
  {
    const std::vector<piece_action> v{
      piece_action(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a8"))
    };
    std::stringstream s;
    s << v;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}


std::ostream& operator<<(std::ostream& os, const std::vector<piece_action>& p) noexcept
{
  os << to_str(p);
  return os;
}
