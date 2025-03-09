#include "piece_action_type.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<piece_action_type> get_all_piece_action_types() noexcept
{
  const auto a{magic_enum::enum_values<piece_action_type>()};
  std::vector<piece_action_type> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

void test_piece_action_type()
{
#ifndef NDEBUG
  // to_human_str
  {
    for (const auto t: get_all_piece_action_types())
    {
      assert(!to_human_str(t).empty());
    }
  }
  // to_str
  {
    assert(to_str(piece_action_type::move) == "move");
    assert(to_str(piece_action_type::attack) == "attack");
    for (const auto t: get_all_piece_action_types())
    {
      assert(!to_str(t).empty());
    }
  }
  // operator<<
  {
    std::stringstream s;
    s << piece_action_type::move;
    assert(s.str() == to_str(piece_action_type::move));
  }
#endif // DEBUG

}

std::string to_human_str(const piece_action_type t) noexcept
{
 switch (t)
 {
    case piece_action_type::attack: return "Attack";
    case piece_action_type::castle_kingside: return "Castle kingside";
    case piece_action_type::castle_queenside: return "Castle queenside";
    case piece_action_type::en_passant: return "Capture en-passant";
    case piece_action_type::move: return "Move";
    case piece_action_type::promote_to_bishop: return "Promote to bishop";
    case piece_action_type::promote_to_knight: return "Promote to knight";
    case piece_action_type::promote_to_queen: return "Promote to queen";
    case piece_action_type::promote_to_rook: return "Promote to rook";
    case piece_action_type::select: return "Select";
    default:
    case piece_action_type::unselect:
      assert(t == piece_action_type::unselect);
    return "Unselect";
  }
}

std::string to_str(const piece_action_type t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}

std::ostream& operator<<(std::ostream& os, const piece_action_type& p) noexcept
{
  os << to_str(p);
  return os;
}
