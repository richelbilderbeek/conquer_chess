#include "piece_action.h"

#include <cassert>
#include <iostream>
#include <sstream>

piece_action::piece_action(
  const piece_action_type type,
  const game_coordinat& coordinat
) : m_type{type}, m_coordinat{coordinat}
{

}

std::string describe_action(const piece_action& p)
{
  return to_str(p);
}

void test_piece_action()
{
#ifndef NDEBUG
  // to_str
  {
    const piece_action a(piece_action_type::move, game_coordinat());
    assert(!to_str(piece_action(a)).empty());
  }
  // describe_action
  {
    const piece_action a(piece_action_type::move, game_coordinat());
    assert(!describe_action(piece_action(a)).empty());
  }
  // operator<<
  {
    const piece_action a(piece_action_type::move, game_coordinat());
    std::stringstream s;
    s << a;
    assert(!s.str().empty());
  }
#endif // DEBUG
}

std::string to_str(const piece_action& a) noexcept
{
  std::stringstream s;
  s << a.get_type() << " to " << a.get_coordinat();
  return s.str();
}

std::ostream& operator<<(std::ostream& os, const piece_action& a) noexcept
{
  os << to_str(a);
  return os;
}
