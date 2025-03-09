#include "game_speed.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<game_speed> get_all_game_speeds() noexcept
{
  const auto a{magic_enum::enum_values<game_speed>()};
  std::vector<game_speed> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

game_speed get_next(const game_speed speed) noexcept
{
  const auto v{get_all_game_speeds()};
  auto there{std::find(std::begin(v), std::end(v), speed)};
  assert(there != std::end(v));
  if (there == std::end(v) - 1)
  {
    assert(!v.empty());
    const auto t{v.front()};
    return t;
  }
  return *(++there);
}

game_speed get_previous(const game_speed speed) noexcept
{
  const auto v{get_all_game_speeds()};
  auto there{std::find(std::begin(v), std::end(v), speed)};
  assert(there != std::end(v));
  if (there == std::begin(v))
  {
    assert(!v.empty());
    const auto t{v.back()};
    return t;
  }
  return *(--there);
}

void test_game_speed()
{
#ifndef NDEBUG
  // get_next
  {
    assert(get_next(game_speed::slowest) == game_speed::slower);
    assert(get_next(game_speed::fastest) == game_speed::slowest);
  }
  // get_previous
  {
    assert(get_previous(get_next(game_speed::slowest)) == game_speed::slowest);
    assert(get_previous(get_next(game_speed::slow)) == game_speed::slow);
    assert(get_previous(get_next(game_speed::normal)) == game_speed::normal);
    assert(get_previous(get_next(game_speed::fast)) == game_speed::fast);
    assert(get_previous(get_next(game_speed::fastest)) == game_speed::fastest);
  }
  // to_delta_t
  {
    delta_t last_game_speed{0.0};
    for (const game_speed s: get_all_game_speeds())
    {
      const auto new_speed{to_delta_t(s)};
      assert(new_speed > last_game_speed);
      last_game_speed = new_speed;
    }
  }
  // to_str
  {
    assert(to_str(game_speed::slowest) == "slowest");
    assert(to_str(game_speed::slow) == "slow");
    assert(to_str(game_speed::normal) == "normal");
    assert(to_str(game_speed::fast) == "fast");
    assert(to_str(game_speed::fastest) == "fastest");
  }
  // 42: operator<<
  {
    const auto speed{game_speed::fastest};
    std::stringstream s;
    s << speed;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

delta_t to_delta_t(const game_speed speed) noexcept
{
  switch (speed)
  {
    case game_speed::fastest: return delta_t(8.0);
    case game_speed::faster: return delta_t(4.0);
    case game_speed::fast: return delta_t(2.0);
    case game_speed::normal: return delta_t(1.0);
    case game_speed::slow: return delta_t(0.5);
    case game_speed::slower: return delta_t(0.25);
    default:
    case game_speed::slowest:
      assert(speed == game_speed::slowest);
      return delta_t(0.125);
  }
}

std::string to_str(const game_speed speed) noexcept
{
  return std::string(magic_enum::enum_name(speed));
}


std::ostream& operator<<(std::ostream& os, const game_speed speed) noexcept
{
  os << to_str(speed);
  return os;
}
