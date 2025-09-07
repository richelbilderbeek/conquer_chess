#include "game_statistic_type.h"

#include <cassert>
#include <iterator>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<game_statistic_type> get_all_game_statistic_types() noexcept
{
  const auto a{magic_enum::enum_values<game_statistic_type>()};
  std::vector<game_statistic_type> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

game_statistic_type get_next(const game_statistic_type item) noexcept
{
  const auto v{get_all_game_statistic_types()};
  auto there{std::find(std::begin(v), std::end(v), item)};
  assert(there != std::end(v));
  if (there == std::end(v) - 1)
  {
    assert(!v.empty());
    const auto t{v.front()};
    return t;
  }
  return *(++there);
}

game_statistic_type get_previous(const game_statistic_type item) noexcept
{
  const auto v{get_all_game_statistic_types()};
  auto there{std::find(std::begin(v), std::end(v), item)};
  assert(there != std::end(v));
  if (there == std::begin(v))
  {
    assert(!v.empty());
    const auto t{v.back()};
    return t;
  }
  return *(--there);
}

void test_game_statistic_type()
{
#ifndef NDEBUG
  // get_all_game_info_statistics
  {
    auto v{get_all_game_statistic_types()};
    assert(std::unique(std::begin(v), std::end(v)) == std::end(v));
  }
  // to_human_str
  {
    assert(to_human_str(game_statistic_type::activity) == "Activity");
  }
  // to_str
  {
    assert(to_str(game_statistic_type::activity) == "activity");
  }
  // get_next
  {
    for (const auto t: get_all_game_statistic_types())
    {
      assert(get_previous(get_next(t)) == t);
    }
  }
  // get_previous
  {
    for (const auto t: get_all_game_statistic_types())
    {
      assert(get_next(get_previous(t)) == t);
    }
  }
  // operator<<
  {
    std::stringstream s;
    s << game_statistic_type::activity;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_human_str(const game_statistic_type t) noexcept
{
  std::string s{to_str(t)};
  s[0] = std::toupper(s[0]);
  return s;
}

std::string to_str(const game_statistic_type t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}

std::ostream& operator<<(std::ostream& os, const game_statistic_type t) noexcept
{
  os << to_str(t);
  return os;
}
