#include "delta_t.h"

#include <cassert>

delta_t::delta_t(const double dt)
  : m_delta_t{dt}
{
  assert(m_delta_t > 0.0);
  assert(m_delta_t <= 1.0);
}

double get_default_delta_t()
{
  return 0.0001;
}

delta_t get_next(const delta_t& dt) noexcept
{
  if (dt.get() <= 0.001) return 0.01;
  if (dt.get() <= 0.01) return 0.1;
  if (dt.get() <= 0.1) return 1.0;
  return 0.001;
}

void test_delta_t()
{
#ifndef NDEBUG
  // to_human_str
  {
    assert(to_human_str(delta_t(1.0)) == "fastest");
    assert(to_human_str(delta_t(0.1)) == "fast");
    assert(to_human_str(delta_t(0.01)) == "slow");
    assert(to_human_str(delta_t(0.001)) == "slowest");
    assert(to_human_str(delta_t(0.0001)) == "slowest");
  }
#endif // DEBUG
}

std::string to_human_str(const delta_t& dt) noexcept
{
  if (dt.get() <= 0.001) return "slowest";
  if (dt.get() <= 0.01) return "slow";
  if (dt.get() <= 0.1) return "fast";
  return "fastest";
}
