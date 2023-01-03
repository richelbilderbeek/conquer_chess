#include "race.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<race> get_all_races() noexcept
{
  const auto a{magic_enum::enum_values<race>()};
  std::vector<race> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}


race get_next(const race r) noexcept
{
  const auto v{get_all_races()};
  auto there{std::find(std::begin(v), std::end(v), r)};
  assert(there != std::end(v));
  if (there == std::end(v) - 1)
  {
    assert(!v.empty());
    const auto t{v.front()};
    return t;
  }
  return *(++there);
}

race get_previous(const race r) noexcept
{
  const auto v{get_all_races()};
  auto there{std::find(std::begin(v), std::end(v), r)};
  assert(there != std::end(v));
  if (there == std::begin(v))
  {
    assert(!v.empty());
    const auto t{v.back()};
    return t;
  }
  return *(--there);
}

void test_race()
{
#ifndef NDEBUG
  // to_str, manual
  {
    assert(to_str(race::zerg) == "zerg");
    assert(to_str(race::protoss) == "protoss");
    assert(to_str(race::terran) == "terran");
  }
  // to_str, all
  {
    for (const auto c: get_all_races())
    {
      assert(!to_str(c).empty());
    }
  }
  // operator<<
  {
    std::stringstream s;
    s << race::zerg;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_str(const race c) noexcept
{
  return std::string(magic_enum::enum_name(c));
}

std::ostream& operator<<(std::ostream& os, const race c) noexcept
{
  os << to_str(c);
  return os;
}
