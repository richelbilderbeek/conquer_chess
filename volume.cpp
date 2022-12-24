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

volume get_previous(const volume& v) noexcept
{
  return volume(
    (v.get_percentage() - 1 + 101) % 101
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
  // get_next increments
  {
    assert(get_previous(get_next(volume(0))) == volume(0));
    assert(get_previous(get_next(volume(100))) == volume(100));
  }
  // operator==
  {
    const volume a(12);
    const volume b(12);
    const volume c(23);
    assert(a == b);
    assert(!(a == c));
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

bool operator==(const volume& lhs, const volume& rhs) noexcept
{
  return lhs.get_percentage() == rhs.get_percentage();
}

std::ostream& operator<<(std::ostream& os, const volume& v) noexcept
{
  os << v.get_percentage();
  return os;
}

