#include "piece_action.h"

#include <cassert>
#include <iostream>
#include <sstream>

piece_action::piece_action(
  const piece_action_type type,
  const square& to
) : m_from{to}, m_type{type}, m_to{to}
{
  assert(type == piece_action_type::attack);
}

piece_action::piece_action(
  const piece_action_type type,
  const square& from,
  const square& to
) : m_from{from}, m_type{type}, m_to{to}
{
  assert(type == piece_action_type::move);
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
    const piece_action a(piece_action_type::attack, square("a1"));
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
  // to_atomic
  {
    assert(to_atomic(piece_action(piece_action_type::move, square("e2"), square("e3"))).size() == 1);
    assert(to_atomic(piece_action(piece_action_type::move, square("e2"), square("e4"))).size() == 2);
    assert(to_atomic(piece_action(piece_action_type::move, square("e2"), square("e5"))).size() == 3);
    assert(to_atomic(piece_action(piece_action_type::attack, square("e7"))).size() == 1);
  }
  // to_str
  {
    const piece_action a(piece_action_type::attack, square("e3"));
    assert(!to_str(piece_action(a)).empty());
    const piece_action b(piece_action_type::move, square("e2"), square("e4"));
    assert(!to_str(piece_action(b)).empty());
  }
  // operator<<
  {
    const piece_action a(piece_action_type::attack, square("a1"));
    std::stringstream s;
    s << a;
    assert(!s.str().empty());
  }
  // operator==
  {
    // Attack
    const piece_action a(piece_action_type::attack, square("a1"));
    const piece_action b(piece_action_type::attack, square("a1"));
    const piece_action c(piece_action_type::attack, square("a2"));
    assert(a == b);
    assert(!(a == c));
    // Move
    const piece_action d(piece_action_type::move, square("a1"), square("a3"));
    const piece_action e(piece_action_type::move, square("a1"), square("a3"));
    const piece_action f(piece_action_type::move, square("a2"), square("a3"));
    assert(d == e);
    assert(!(d == f));
  }
#endif // DEBUG
}

std::vector<piece_action> to_atomic(const piece_action& a)
{
  std::vector<piece_action> atomic_actions;
  if (a.get_type() == piece_action_type::attack)
  {
    atomic_actions.push_back(a);
  }
  else
  {
    assert(a.get_type() == piece_action_type::move);
    const std::vector<square> squares{
      get_intermediate_squares(
        a.get_from(),
        a.get_to()
      )
    };
    const int n_squares{static_cast<int>(squares.size())};
    assert(n_squares >= 2);
    atomic_actions.reserve(n_squares - 1);
    for (int i{1}; i != n_squares; ++i)
    {
      assert(i - 1 >= 0);
      assert(i < static_cast<int>(squares.size()));
      atomic_actions.push_back(
        piece_action(
          piece_action_type::move,
          squares[i - 1],
          squares[i]
        )
      );
    }
  }
  return atomic_actions;
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
