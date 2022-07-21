#include "volume.h"

#include <cassert>
#include <iostream>
#include <sstream>

volume::volume(const int volume_in_percent)
  : m_volume_in_percent{volume_in_percent}
{
  assert(m_volume_in_percent >= 0);
  assert(m_volume_in_percent <= 100);
}

volume get_next(const volume& v) noexcept
{
  return volume(
    (v.get_percentage() + 1) % 101
  );
}

void test_volume()
{
#ifndef NDEBUG
  // volume::get_percentage
  {
    const int volume_in_percent{50};
    const volume v(volume_in_percent);
    assert(v.get_percentage() == volume_in_percent);
  }
  // get_next increments
  {
    const int volume_in_percent{50};
    const volume v(volume_in_percent);
    const auto next_volume{get_next(v)};
    assert(next_volume.get_percentage() == volume_in_percent + 1);
  }
  // get_next loops
  {
    const volume v(100);
    const auto next_volume{get_next(v)};
    assert(next_volume.get_percentage() == 0);
  }
  // operator<<
  {
    std::stringstream s;
    const volume v(100);
    s << v;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

std::ostream& operator<<(std::ostream& os, const volume& v) noexcept
{
  os << v.get_percentage();
  return os;
}

