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

void test_delta_t()
{
#ifndef NDEBUG
  // get
  {
    const double dt{0.123};
    const delta_t t{dt};
    assert(t.get() == dt);
  }
  // operator>
  {
    const delta_t low{0.0001};
    const delta_t high{0.11};
    assert(high > low);
  }
#endif // DEBUG
}

bool operator>(const delta_t& lhs, const delta_t& rhs) noexcept
{
  return lhs.get() > rhs.get();
}
