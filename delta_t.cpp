#include "delta_t.h"

#include <cassert>

delta_t::delta_t(const double dt)
  : m_delta_t{dt}
{
  assert(m_delta_t > 0.0);
  assert(m_delta_t <= 1.0);
}

std::string to_human_str(const delta_t& dt) noexcept
{
  if (dt.get() < 0.001) return "slowest";
  if (dt.get() < 0.01) return "slow";
  if (dt.get() < 0.1) return "fast";
  return "fastest";
}
