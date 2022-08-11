#ifndef ACTION_HISTORY_H
#define ACTION_HISTORY_H

#include "delta_t.h"
#include "piece_action.h"

#include <iosfwd>
#include <vector>

class action_history
{
public:
  action_history();

  void add_action(const delta_t& t, const piece_action& action) noexcept;

  const auto& get_timed_actions() const noexcept { return m_timed_actions; }

private:

  /// The history of actions, in chrononical order
  std::vector<std::pair<delta_t, piece_action>> m_timed_actions;

};


/// Test this class and its free functions
void test_action_history();

std::string to_str(const action_history& history) noexcept;

std::ostream& operator<<(std::ostream& os, const action_history& history) noexcept;

#endif // ACTION_HISTORY_H
