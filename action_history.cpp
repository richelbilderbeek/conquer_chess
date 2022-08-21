#include "action_history.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

action_history::action_history(
  const std::vector<std::pair<delta_t, piece_action>>& timed_actions
) : m_timed_actions{timed_actions}
{
  assert(
    std::is_sorted(
      std::begin(m_timed_actions),
      std::end(m_timed_actions),
      [](const auto& lhs, const auto& rhs)
      {
        return lhs.first < rhs.first;
      }
    )
  );
}

void action_history::add_action(const delta_t& t, const piece_action& action) noexcept
{
  m_timed_actions.push_back(std::make_pair(t, action));
}

const piece_action& get_last_action(const action_history& history)
{
  assert(has_actions(history));
  return history.get_timed_actions().back().second;
}

bool has_actions(const action_history& history) noexcept
{
  return !history.get_timed_actions().empty();
}

action_history merge_action_histories(const std::vector<action_history> histories)
{
  std::vector<std::pair<delta_t, piece_action>> timed_actions;
  for (const auto& history: histories)
  {
    const auto tas{history.get_timed_actions()};
    std::copy(std::begin(tas), std::end(tas), std::back_inserter(timed_actions));
  }
  std::sort(
    std::begin(timed_actions),
    std::end(timed_actions),
    [](const auto& lhs, const auto& rhs)
    {
      return lhs.first < rhs.first;
    }
  );
  return action_history(timed_actions);
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
