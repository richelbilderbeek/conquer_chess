#include "starting_position_type.h"

#include <algorithm>
#include <cassert>

#include "../magic_enum/include/magic_enum.hpp"

std::vector<starting_position_type> get_all_starting_position_types() noexcept
{
  const auto a{magic_enum::enum_values<starting_position_type>()};
  std::vector<starting_position_type> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
  /*
  return
  {
    starting_position_type::standard,
    starting_position_type::kings_only,
    starting_position_type::before_scholars_mate,
    starting_position_type::queen_end_game,
    starting_position_type::bishop_and_knight_end_game,
    starting_position_type::pawn_all_out_assault
  };
  */
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
#endif // DEBUG
}

std::string to_str(const starting_position_type t) noexcept
{
  return std::string(magic_enum::enum_name(t));
  /*
  switch(t)
  {
    case starting_position_type::standard:
      return "standard";
    case starting_position_type::kings_only:
      return "kings_only";
    case starting_position_type::before_scholars_mate:
      return "before_scholars_mate";
    case starting_position_type::queen_end_game:
      return "queen_end_game";
    case starting_position_type::bishop_and_knight_end_game:
      return "bishop_and_knight_end_game";
    case starting_position_type::pawn_all_out_assault:
    default:
      assert(t == starting_position_type::pawn_all_out_assault);
      return "pawn_all_out_assault";
  }
  */
}
