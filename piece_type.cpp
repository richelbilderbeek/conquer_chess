#include "piece_type.h"

#include <cassert>
#include <iostream>
#include <sstream>

double get_max_health(const piece_type type)
{
  switch (type)
  {
    case piece_type::bishop: return 100.0;
    case piece_type::king: return 100.0;
    case piece_type::knight: return 100.0;
    case piece_type::pawn: return 100.0;
    case piece_type::queen: return 100.0;
    default:
    case piece_type::rook: return 100.0;
  }
}

void test_piece_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(piece_type::king) == "king");
    assert(to_str(piece_type::pawn) == "pawn");
    assert(to_str(piece_type::rook) == "rook");
    assert(to_str(piece_type::queen) == "queen");
    assert(to_str(piece_type::bishop) == "bishop");
    assert(to_str(piece_type::knight) == "knight");
  }
  // get_max_health
  {
    assert(get_max_health(piece_type::king) > 0.0);
    assert(get_max_health(piece_type::pawn) > 0.0);
    assert(get_max_health(piece_type::rook) > 0.0);
    assert(get_max_health(piece_type::queen) > 0.0);
    assert(get_max_health(piece_type::bishop) > 0.0);
    assert(get_max_health(piece_type::knight) > 0.0);
  }
  // operator<<
  {
    std::stringstream s;
    s << piece_type::king;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_str(const piece_type type) noexcept
{
  switch (type)
  {
    case piece_type::bishop: return "bishop";
    case piece_type::king: return "king";
    case piece_type::knight: return "knight";
    case piece_type::queen: return "queen";
    case piece_type::pawn: return "pawn";
    default:
    case piece_type::rook: return "rook";
  }
}

std::ostream& operator<<(std::ostream& os, const piece_type type) noexcept
{
  os << to_str(type);
  return os;
}
