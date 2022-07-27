#include "log.h"

log::log()
{

}

void log::add_message(
  const delta_t& t,
  const message& m
) noexcept
{
  m_timed_messages.push_back(
    std::make_pair(t, m)
  );
}
