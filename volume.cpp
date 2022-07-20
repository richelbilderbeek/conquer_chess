#include "volume.h"

#include <cassert>
#include <iostream>

volume::volume(const int volume_in_percent)
  : m_volume_in_percent{volume_in_percent}
{
  assert(m_volume_in_percent >= 0);
  assert(m_volume_in_percent <= 100);
}

volume get_next(const volume& v) noexcept
{
  return volume(
    (v.get_percentage() + 1) % 100
  );
}

std::ostream& operator<<(std::ostream& os, const volume& v) noexcept
{
  os << v.get_percentage();
  return os;
}
