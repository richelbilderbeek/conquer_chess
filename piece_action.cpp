#include "piece_action.h"

#include <cassert>
#include <iostream>
#include <sstream>

piece_action::piece_action(
  const piece_action_type type,
  const square& to
) : m_from{to}, m_type{type}, m_to{to}
{

}

piece_action::piece_action(
  const piece_action_type type,
  const square& from,
  const square& to
) : m_from{from}, m_type{type}, m_to{to}
{

}

std::string describe_action(const piece_action& p)
{
  return to_str(p);
}

bool is_atomic(const piece_action& a) noexcept
{
  if (a.get_from() == a.get_to()) return true;
  const int dx{std::abs(a.get_to().get_x() - a.get_from().get_x())};
  const int dy{std::abs(a.get_to().get_y() - a.get_from().get_y())};
  if (dx == 0 && dy == 1) return true; // Vertical
  if (dx == 1 && dy == 0) return true; // Horizontal
  if (dx == 1 && dy == 1) return true; // Diagonal
  if (dx == 2 && dy == 1) return true; // Knight
  if (dx == 1 && dy == 2) return true; // Knight
  return false;
}

void test_piece_action()
{
#ifndef NDEBUG
  // describe_action
  {
    const piece_action a(piece_action_type::move, game_coordinat());
    assert(!describe_action(piece_action(a)).empty());
  }
  // is_atomic
  {
    const auto d1{game_coordinat(to_coordinat(square("d1")))};
    const auto d2{game_coordinat(to_coordinat(square("d2")))};
    const auto d3{game_coordinat(to_coordinat(square("d3")))};
    assert(is_atomic(piece_action(piece_action_type::move, d1, d2)));
    assert(is_atomic(piece_action(piece_action_type::move, d2, d3)));
    assert(!is_atomic(piece_action(piece_action_type::move, d1, d3)));
  }
  // to_str
  {
    const piece_action a(piece_action_type::move, game_coordinat());
    assert(!to_str(piece_action(a)).empty());
  }
  // operator<<
  {
    const piece_action a(piece_action_type::move, game_coordinat());
    std::stringstream s;
    s << a;
    assert(!s.str().empty());
  }
  // operator==
  {
    const piece_action a(piece_action_type::move, game_coordinat());
    const piece_action b(piece_action_type::move, game_coordinat());
    const piece_action c(piece_action_type::attack, game_coordinat());
    assert(a == b);
    assert(!(a == c));
  }
#endif // DEBUG
}

std::string to_str(const piece_action& a) noexcept
{
  std::stringstream s;
  if (a.get_type() == piece_action_type::move)
  {
    s << a.get_type() << " from " << a.get_from() << " to " << a.get_to();
  }
  else
  {
    assert(a.get_type() == piece_action_type::attack);
    s << a.get_type() << " " << a.get_to();
  }
  return s.str();
}

bool operator==(const piece_action& lhs, const piece_action& rhs) noexcept
{
  return lhs.get_type() == rhs.get_type()
    && lhs.get_from() == rhs.get_from()
    && lhs.get_to() == rhs.get_to()
  ;
}

std::ostream& operator<<(std::ostream& os, const piece_action& a) noexcept
{
  os << to_str(a);
  return os;
}
