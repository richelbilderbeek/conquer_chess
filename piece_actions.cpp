#include "piece_actions.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

bool is_in(const piece_action& action, const std::vector<piece_action>& actions) noexcept
{
  return std::find(std::begin(actions), std::end(actions), action) != std::end(actions);
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