#include "log.h"

#include <algorithm>
#include <cassert>
#include <sstream>

log::log(const double display_time_secs)
  : m_display_time_secs{display_time_secs}
{
  assert(m_display_time_secs >= 0.0);
}

void log::add_message(
  const message& m
) noexcept
{
  m_timed_messages.push_back(
    std::make_pair(
      m_clock.getElapsedTime().asSeconds(),
      m
    )
  );
}

std::string log::get_last_messages() const
{
  std::stringstream s;
  std::transform(
    std::begin(m_timed_messages),
    std::end(m_timed_messages),
    std::ostream_iterator<std::string>(s, "\n"),
    [](const auto& p)
    {
      return to_str(p.second);
    }
  );
  return s.str();
}

void log::tick()
{
  const double now_secs{m_clock.getElapsedTime().asSeconds()};
  m_timed_messages.erase(
    std::remove_if(
      std::begin(m_timed_messages),
      std::end(m_timed_messages),
      [this, now_secs](const auto& p)
      {
        return now_secs - p.first > m_display_time_secs;
      }
    ),
    std::end(m_timed_messages)
  );
}
