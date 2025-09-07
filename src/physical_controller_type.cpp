#include "physical_controller_type.h"

#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<physical_controller_type> get_all_physical_controller_types() noexcept
{
  const auto a{magic_enum::enum_values<physical_controller_type>()};
  std::vector<physical_controller_type> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

/*
physical_controller_type get_physical_controller_type(
  const game_controller& c,
  const side player
) noexcept
{
  return get_physical_controller(c, player).get_type();
}
*/

physical_controller_type get_next(const physical_controller_type item) noexcept
{
  const auto v{get_all_physical_controller_types()};
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

physical_controller_type get_previous(const physical_controller_type item) noexcept
{
  const auto v{get_all_physical_controller_types()};
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

void test_physical_controller_type()
{
#ifndef NDEBUG
  // get_all_physical_controller_types
  {
    auto v{get_all_physical_controller_types()};
    assert(std::unique(std::begin(v), std::end(v)) == std::end(v));
  }
  // to_human_str
  {
    assert(to_human_str(physical_controller_type::mouse) == "Mouse");
    assert(to_human_str(physical_controller_type::keyboard) == "Keyboard");
  }
  // to_str
  {
    assert(to_str(physical_controller_type::mouse) == "mouse");
    assert(to_str(physical_controller_type::keyboard) == "keyboard");
  }
  // get_next
  {
    for (const auto t: get_all_physical_controller_types())
    {
      assert(get_previous(get_next(t)) == t);
    }
  }
  // get_previous
  {
    for (const auto t: get_all_physical_controller_types())
    {
      assert(get_next(get_previous(t)) == t);
    }
  }
  // operator<<
  {
    std::stringstream s;
    s << physical_controller_type::mouse;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_human_str(const physical_controller_type t) noexcept
{
  std::string s{to_str(t)};
  s[0] = std::toupper(s[0]);
  return s;
}

std::string to_str(const physical_controller_type t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}

std::ostream& operator<<(std::ostream& os, const physical_controller_type t) noexcept
{
  os << to_str(t);
  return os;
}
