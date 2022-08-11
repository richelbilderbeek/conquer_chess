#include "action_history.h"

#include <cassert>
#include <iostream>
#include <sstream>

action_history::action_history()
{

}

void action_history::add_action(const delta_t& t, const piece_action& action) noexcept
{
  m_timed_actions.push_back(std::make_pair(t, action));
}

void test_action_history()
{
#ifndef NDEBUG
  // upon construction there are timed actions
  {
    const action_history h;
    assert(h.get_timed_actions().empty());
  }
  // operator<<
  {
    std::stringstream s;
    action_history h;
    s << h; // Still empty
    assert(s.str().empty());
    h.add_action(
      delta_t(1.0),
      get_test_piece_action()
    );
    s << h;
    assert(!s.str().empty());
  }
#endif
}

std::string to_str(const action_history& history) noexcept
{
  std::stringstream s;
  for (const auto& timed_action: history.get_timed_actions())
  {
    s << timed_action.first << ": " << timed_action.second << '\n';
  }
  std::string t{s.str()};
  if (!t.empty()) t.pop_back();
  return t;
}

std::ostream& operator<<(std::ostream& os, const action_history& history) noexcept
{
  os << to_str(history);
  return os;
}
