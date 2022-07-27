#ifndef LOG_H
#define LOG_H

#include <vector>

#include "delta_t.h"
#include "message.h"

/// The text log in the game
class log
{
public:
  log();

  void add_message(
    const delta_t& t,
    const message& m
  ) noexcept;

private:
  std::vector<std::pair<delta_t, message>> m_timed_messages;

};

#endif // LOG_H
