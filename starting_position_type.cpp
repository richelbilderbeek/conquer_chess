#include "starting_position_type.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum.hpp"

std::vector<starting_position_type> get_all_starting_position_types() noexcept
{
  const auto a{magic_enum::enum_values<starting_position_type>()};
  std::vector<starting_position_type> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

starting_position_type get_next(const starting_position_type starting_position) noexcept
{
  const auto v{get_all_starting_position_types()};
  auto there{std::find(std::begin(v), std::end(v), starting_position)};
  assert(there != std::end(v));
  if (there == std::end(v) - 1)
  {
    assert(!v.empty());
    const starting_position_type t{v.front()};
    return t;
  }
  return *(++there);
}

starting_position_type get_previous(const starting_position_type starting_position) noexcept
{
  const auto v{get_all_starting_position_types()};
  auto there{std::find(std::begin(v), std::end(v), starting_position)};
  assert(there != std::end(v));
  if (there == std::begin(v))
  {
    assert(!v.empty());
    const starting_position_type t{v.back()};
    return t;
  }
  return *(--there);
}

void test_starting_position_type()
{
#ifndef NDEBUG
  // to_str, manual
  {
    assert(to_str(starting_position_type::standard) == "standard");
    assert(to_str(starting_position_type::kings_only) == "kings_only");
    assert(to_str(starting_position_type::before_scholars_mate) == "before_scholars_mate");
    assert(to_str(starting_position_type::bishop_and_knight_end_game) == "bishop_and_knight_end_game");
    assert(to_str(starting_position_type::pawn_all_out_assault) == "pawn_all_out_assault");
  }
  // to_str, from collection
  {
    for (const auto spt: get_all_starting_position_types())
    {
      assert(!to_str(spt).empty());
    }
  }
  // get_next
  {
    for (const auto spt: get_all_starting_position_types())
    {
      const auto next{get_next(spt)};
      assert(next != spt);
    }
  }
  // get_previous
  {
    for (const auto spt: get_all_starting_position_types())
    {
      const auto next{get_next(spt)};
      const auto previous{get_previous(next)};
      assert(previous == spt);
    }
  }
  // 43: operator<<
  {
    std::stringstream s;
    const auto t = starting_position_type::before_en_passant;
    s << t;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_str(const starting_position_type t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}

std::ostream& operator<<(std::ostream& os, const starting_position_type t) noexcept
{
  os << to_str(t);
  return os;
}
