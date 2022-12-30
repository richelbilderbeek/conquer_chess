#include "piece_action_type.h"

#include <cassert>
#include <iostream>
#include <sstream>

#include "../magic_enum/include/magic_enum.hpp"

void test_piece_action_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(piece_action_type::move) == "move");
    assert(to_str(piece_action_type::attack) == "attack");
  }
  // operator<<
  {
    std::stringstream s;
    s << piece_action_type::move;
    assert(s.str() == to_str(piece_action_type::move));
  }
#endif // DEBUG

}

std::string to_three_char_str(const piece_action_type t) noexcept
{
 switch (t)
 {
    case piece_action_type::attack: return "ATK";
    case piece_action_type::castle_kingside: return "CKS";
    case piece_action_type::castle_queenside: return "CQS";
    case piece_action_type::en_passant: return "EPC";
    case piece_action_type::move: return "MOV";
    case piece_action_type::promote_to_bishop: return "P2B";
    case piece_action_type::promote_to_knight: return "P2N";
    case piece_action_type::promote_to_queen: return "P2Q";
    case piece_action_type::promote_to_rook: return "P2R";
    case piece_action_type::select: return "SEL";
    default:
    case piece_action_type::unselect:
      assert(t == piece_action_type::unselect);
    return "UNS";
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
