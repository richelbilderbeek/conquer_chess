#include "game_speed.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
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
  // get_speed_multiplier
  {
    for (const auto s: get_all_game_speeds())
    {
      assert(get_speed_multiplier(s) > 0.0);
    }
  }
  // to_delta_t
  {
    double last_game_speed{0.0};
    for (const game_speed s: get_all_game_speeds())
    {
      const auto new_speed{get_speed_multiplier(s)};
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
  // to_human_str
  {
    assert(to_human_str(game_speed::slowest) == "Slowest");
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

double get_speed_multiplier(const game_speed speed) noexcept
{
  switch (speed)
  {
    case game_speed::fastest: return 8.0;
    case game_speed::faster: return 4.0;
    case game_speed::fast: return 2.0;
    case game_speed::normal: return 1.0;
    case game_speed::slow: return 0.5;
    case game_speed::slower: return 0.25;
    default:
    case game_speed::slowest:
      assert(speed == game_speed::slowest);
      return 0.125;

  }
}

std::string to_human_str(const game_speed speed) noexcept
{
  std::string s{to_str(speed)};
  assert(s.size() >= 1);
  s[0] = std::toupper(s[0]);
  return s;
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
