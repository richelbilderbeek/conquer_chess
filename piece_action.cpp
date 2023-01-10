#include "piece_action.h"

#include "piece.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

piece_action::piece_action(
  const chess_color color,
  const piece_type pt,
  const piece_action_type at,
  const square& from,
  const square& to
) : m_action_type{at},
    m_color{color},
    m_from{from},
    m_piece_type{pt},
    m_to{to}
{
  // m_from can be m_to if a piece needs to move back
}

piece_action::piece_action(
  const chess_color color,
  const piece_type pt,
  const piece_action_type at,
    const std::string& from_str,
    const std::string& to_str
) : piece_action(color, pt, at, square(from_str), square(to_str))
{
  // m_from can be m_to if a piece needs to move back
}


std::string describe_action(const piece_action& p)
{
  return to_str(p);
}

piece_action get_test_piece_action() noexcept
{
  return piece_action(
    chess_color::white,
    piece_type::pawn,
    piece_action_type::move,
    "e2",
    "e4"
  );
}

bool has_action_of_type(
  const std::vector<piece_action>& actions,
  const piece_action_type t
)
{
  return std::find_if(
    std::begin(actions),
    std::end(actions),
    [t](const piece_action& action) { return action.get_action_type() == t; }
  ) != std::end(actions);
}

bool is_double_move(const piece_action& a) noexcept
{
  return a.get_piece_type() == piece_type::pawn
    && a.get_action_type() == piece_action_type::move
    && std::abs(a.get_from().get_x() - a.get_to().get_x()) == 2;
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
    const piece_action a(chess_color::white, piece_type::rook, piece_action_type::attack, square("d1"), square("d8"));
    assert(!describe_action(piece_action(a)).empty());
  }
  // has_action_of_type
  {
    const piece_action a(chess_color::white, piece_type::rook, piece_action_type::attack, square("d1"), square("d8"));
    assert(!has_action_of_type( { a }, piece_action_type::move));
    assert(has_action_of_type( { a }, piece_action_type::attack));
  }
  // is_atomic
  {
    const auto d1{game_coordinat(to_coordinat(square("d1")))};
    const auto d2{game_coordinat(to_coordinat(square("d2")))};
    const auto d3{game_coordinat(to_coordinat(square("d3")))};
    assert(is_atomic(piece_action(chess_color::white, piece_type::king, piece_action_type::move, square(d1), square(d2))));
    assert(is_atomic(piece_action(chess_color::white, piece_type::king, piece_action_type::move, square(d2), square(d3))));
    assert(!is_atomic(piece_action(chess_color::white, piece_type::king, piece_action_type::move, square(d1), square(d3))));
  }
  // to_atomic
  {
    // Moving
    assert(to_atomic(piece_action(chess_color::white, piece_type::king, piece_action_type::move, square("e2"), square("e3"))).size() == 1);
    assert(to_atomic(piece_action(chess_color::white, piece_type::king, piece_action_type::move, square("e2"), square("e4"))).size() == 2);
    assert(to_atomic(piece_action(chess_color::white, piece_type::king, piece_action_type::move, square("e2"), square("e5"))).size() == 3);
    // Attacking
    assert(to_atomic(piece_action(chess_color::white, piece_type::king, piece_action_type::attack, square("e7"), square("e6"))).size() == 1);
    assert(to_atomic(piece_action(chess_color::white, piece_type::queen, piece_action_type::attack, square("a1"), square("h8"))).size() == 1);
    assert(to_atomic(piece_action(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("h8"))).size() > 4);
  }
  // to_str
  {
    const piece_action a(chess_color::white, piece_type::king, piece_action_type::attack, square("e2"), square("e3"));
    assert(!to_str(piece_action(a)).empty());
    const piece_action b(chess_color::white, piece_type::king, piece_action_type::move, square("e2"), square("e4"));
    assert(!to_str(piece_action(b)).empty());
  }
  // to_str on std::vector<piece_action>
  {
    std::vector<piece_action> v;
    assert(to_str(v).empty());
    const piece_action a(chess_color::white, piece_type::king, piece_action_type::attack, square("e2"), square("e3"));
    v.push_back(a);
    assert(!to_str(v).empty());
    const piece_action b(chess_color::white, piece_type::king, piece_action_type::move, square("e2"), square("e4"));
    v.push_back(b);
    assert(!to_str(v).empty());
  }
  // operator<<
  {
    const piece_action a(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a8"));
    std::stringstream s;
    s << a;
    assert(!s.str().empty());
  }
  // operator==
  {
    // Different color
    {
      const piece_action a(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action b(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action c(chess_color::black, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      assert(a == b);
      assert(!(a == c));

    }
    // Different piece
    {
      const piece_action a(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action b(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action c(chess_color::white, piece_type::queen, piece_action_type::attack, square("a1"), square("a3"));
      assert(a == b);
      assert(!(a == c));
    }
    // Different action typ
    {
      // Attack
      const piece_action a(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action b(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action c(chess_color::white, piece_type::king, piece_action_type::attack, square("a2"), square("a3"));
      assert(a == b);
      assert(!(a == c));
      // Move
      const piece_action d(chess_color::white, piece_type::king, piece_action_type::move, square("a1"), square("a3"));
      const piece_action e(chess_color::white, piece_type::king, piece_action_type::move, square("a1"), square("a3"));
      const piece_action f(chess_color::white, piece_type::king, piece_action_type::move, square("a2"), square("a3"));
      assert(d == e);
      assert(!(d == f));
    }
    // Different from
    {
      const piece_action a(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action b(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action c(chess_color::white, piece_type::king, piece_action_type::attack, square("a2"), square("a3"));
      assert(a == b);
      assert(!(a == c));
    }
    // Different to
    {
      const piece_action a(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action b(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a3"));
      const piece_action c(chess_color::white, piece_type::king, piece_action_type::attack, square("a1"), square("a4"));
      assert(a == b);
      assert(!(a == c));
    }
    // 63: to_str
    {
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::king,
            piece_action_type::attack,
            square("a1"),
            square("a3")
          )
        ).empty()
      );
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::king,
            piece_action_type::castle_kingside,
            square("e1"),
            square("g1")
          )
        ).empty()
      );
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::king,
            piece_action_type::castle_queenside,
            square("e1"),
            square("c1")
          )
        ).empty()
      );
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::pawn,
            piece_action_type::en_passant,
            square("e5"),
            square("d6")
          )
        ).empty()
      );
      assert(!to_str(piece_action(chess_color::white, piece_type::king, piece_action_type::move, square("e2"), square("e4"))).empty());
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::pawn,
            piece_action_type::promote_to_bishop,
            square("e7"),
            square("d8")
          )
        ).empty()
      );
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::pawn,
            piece_action_type::promote_to_knight,
            square("e7"),
            square("d8")
          )
        ).empty()
      );
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::pawn,
            piece_action_type::promote_to_queen,
            square("e7"),
            square("d8")
          )
        ).empty()
      );
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::pawn,
            piece_action_type::promote_to_rook,
            square("e7"),
            square("d8")
          )
        ).empty()
      );
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::pawn,
            piece_action_type::select,
            square("e2"),
            square("e2")
          )
        ).empty()
      );
      assert(
        !to_str(
          piece_action(
            chess_color::white,
            piece_type::pawn,
            piece_action_type::unselect,
            square("e2"),
            square("e2")
          )
        ).empty()
      );
    }
  }
#endif // DEBUG
}

std::vector<piece_action> to_atomic(const piece_action& a)
{
  assert(a.get_action_type() == piece_action_type::move
    || a.get_action_type() == piece_action_type::attack
  );
  std::vector<piece_action> atomic_actions;
  if (a.get_from() == a.get_to()) return atomic_actions;

  const std::vector<square> squares{
    get_intermediate_squares(
      a.get_from(),
      a.get_to()
    )
  };
  const int n_squares{static_cast<int>(squares.size())};
  assert(n_squares >= 2);
  atomic_actions.reserve(n_squares);


  for (int i{1}; i != n_squares; ++i)
  {
    assert(i - 1 >= 0);
    assert(i < static_cast<int>(squares.size()));
    const auto& from{squares[i - 1]};
    const auto& to{squares[i]};
    if (a.get_action_type() == piece_action_type::attack
      && can_attack(a.get_color(), a.get_piece_type(), from, a.get_to()))
    {
      atomic_actions.push_back(
        piece_action(
          a.get_color(),
          a.get_piece_type(),
          piece_action_type::attack,
          from,
          a.get_to()
        )
      );
      break; // Done!
    }
    else
    {
      atomic_actions.push_back(
        piece_action(
          a.get_color(),
          a.get_piece_type(),
          piece_action_type::move,
          from,
          to
        )
      );
    }
  }
  return atomic_actions;
}

std::string to_str(const piece_action& a) noexcept
{
  std::stringstream s;
  switch (a.get_action_type())
  {
    case piece_action_type::attack:
      s << a.get_color() << " " << a.get_piece_type() << " " << a.get_action_type() << " from " << a.get_from() << " to " << a.get_to();
      break;
    case piece_action_type::castle_kingside:
      assert(a.get_piece_type() == piece_type::king);
      s << a.get_color() << " " << a.get_piece_type() << " castles kingside";
      break;
    case piece_action_type::castle_queenside:
      assert(a.get_piece_type() == piece_type::king);
      s << a.get_color() << " " << a.get_piece_type() << " castles queenside";
      break;
    case piece_action_type::en_passant:
      assert(a.get_piece_type() == piece_type::pawn);
      s << a.get_color() << " " << a.get_piece_type() << " " << a.get_action_type() << " en-passant from " << a.get_from() << " to " << a.get_to();
      break;
    case piece_action_type::move:
      s << a.get_color() << " " << a.get_piece_type() << " " << a.get_action_type() << " from " << a.get_from() << " to " << a.get_to();
     break;
    case piece_action_type::promote_to_bishop:
      assert(a.get_piece_type() == piece_type::pawn || a.get_piece_type() == piece_type::bishop);
      s << a.get_color() << " pawn moves from " << a.get_from() << " to " << a.get_to() << " to become a " << a.get_piece_type();
     break;
    case piece_action_type::promote_to_knight:
      assert(a.get_piece_type() == piece_type::pawn || a.get_piece_type() == piece_type::knight);
      s << a.get_color() << " pawn moves from " << a.get_from() << " to " << a.get_to() << " to become a " << a.get_piece_type();
     break;
    case piece_action_type::promote_to_queen:
      assert(a.get_piece_type() == piece_type::pawn || a.get_piece_type() == piece_type::queen);
      s << a.get_color() << " pawn moves from " << a.get_from() << " to " << a.get_to() << " to become a " << a.get_piece_type();
     break;
    case piece_action_type::promote_to_rook:
      assert(a.get_piece_type() == piece_type::pawn || a.get_piece_type() == piece_type::rook);
      s << a.get_color() << " pawn moves from " << a.get_from() << " to " << a.get_to() << " to become a " << a.get_piece_type();
     break;
    case piece_action_type::select:
      assert(a.get_from() == a.get_to());
      s << a.get_color() << " " << a.get_piece_type() << " is " << a.get_action_type() << " at " << a.get_from();
     break;
    case piece_action_type::unselect:
      assert(a.get_from() == a.get_to());
      s << a.get_color() << " " << a.get_piece_type() << " is " << a.get_action_type() << " at " << a.get_from();
     break;
  }
  return s.str();
}

std::string to_str(const std::vector<piece_action>& actions) noexcept
{
  std::stringstream s;
  for (const auto action: actions)
  {
    s << action << '\n';
  }
  std::string t{s.str()};
  if (t.empty()) return t;
  t.pop_back();
  return t;
}

bool operator==(const piece_action& lhs, const piece_action& rhs) noexcept
{
  return
       lhs.get_color() == rhs.get_color()
    && lhs.get_piece_type() == rhs.get_piece_type()
    && lhs.get_action_type() == rhs.get_action_type()
    && lhs.get_from() == rhs.get_from()
    && lhs.get_to() == rhs.get_to()
  ;
}

bool operator!=(const piece_action& lhs, const piece_action& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const piece_action& a) noexcept
{
  os << to_str(a);
  return os;
}
