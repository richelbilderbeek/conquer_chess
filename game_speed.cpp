#include "game_speed.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum.hpp" // https://github.com/Neargye/magic_enum

game_speed get_next(const game_speed speed) noexcept
{
  switch (speed)
  {
    case game_speed::fastest: return game_speed::slowest;
    case game_speed::fast: return game_speed::fastest;
    case game_speed::normal: return game_speed::fast;
    case game_speed::slow: return game_speed::normal;
    default:
    case game_speed::slowest:
      assert(speed == game_speed::slowest);
      return game_speed::slow;
  }
}

game_speed get_previous(const game_speed speed) noexcept
{
  switch (speed)
  {
    case game_speed::fastest: return game_speed::fast;
    case game_speed::fast: return game_speed::normal;
    case game_speed::normal: return game_speed::slow;
    case game_speed::slow: return game_speed::slowest;
    default:
    case game_speed::slowest:
      assert(speed == game_speed::slowest);
      return game_speed::fastest;
  }
}

void test_game_speed()
{
#ifndef NDEBUG
  // get_next
  {
    assert(get_next(game_speed::slowest) == game_speed::slow);
    assert(get_next(game_speed::slow) == game_speed::normal);
    assert(get_next(game_speed::normal) == game_speed::fast);
    assert(get_next(game_speed::fast) == game_speed::fastest);
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
    const auto dt_fastest{to_delta_t(game_speed::fastest)};
    const auto dt_fast{to_delta_t(game_speed::fast)};
    const auto dt_normal{to_delta_t(game_speed::normal)};
    const auto dt_slow{to_delta_t(game_speed::slow)};
    const auto dt_slowest{to_delta_t(game_speed::slowest)};
    assert(dt_fastest > dt_fast);
    assert(dt_fast > dt_normal);
    assert(dt_normal > dt_slow);
    assert(dt_slow > dt_slowest);
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
    case game_speed::fastest: return delta_t(5.0);
    case game_speed::fast: return delta_t(2.0);
    case game_speed::normal: return delta_t(1.0);
    case game_speed::slow: return delta_t(0.5);
    default:
    case game_speed::slowest:
      assert(speed == game_speed::slowest);
      return delta_t(0.25);
  }
}

std::string to_str(const game_speed speed) noexcept
{
  return std::string(magic_enum::enum_name(speed));
  /*
  switch (speed)
  {
    case game_speed::fastest: return "fastest";
    case game_speed::fast: return "fast";
    case game_speed::normal: return "normal";
    case game_speed::slow: return "slow";
    default:
    case game_speed::slowest:
      assert(speed == game_speed::slowest);
      return "slowest";
  }
  */
}


std::ostream& operator<<(std::ostream& os, const game_speed speed) noexcept
{
  os << to_str(speed);
  return os;
}
