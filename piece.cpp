#include "piece.h"

#include "helper.h"
#include "piece_type.h"
#include "square.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>

piece::piece(
  const chess_color color,
  const piece_type type,
  const square& coordinat,
  const side player
)
  : m_color{color},
    m_coordinat{to_coordinat(coordinat)},
    m_current_square{coordinat},
    m_health{::get_max_health(type)},
    m_id{create_new_id()},
    m_is_selected{false},
    m_max_health{::get_max_health(type)},
    m_player{player},
    m_target_square{},
    m_type{type}
{

}

void piece::add_action(const piece_action& action)
{
  const std::vector<piece_action> atomic_actions{
    to_atomic(action)
  };
  std::copy_if(
    std::begin(atomic_actions),
    std::end(atomic_actions),
    std::back_inserter(m_actions),
    [this](const piece_action& a)
    {
      return can_move(this->get_type(), a.get_from(), a.get_to(), this->get_player());
    }
  );
  //assert(is_atomic(action));
  //m_actions.push_back(action);
}

void piece::add_sound_effect(const sound_effect& effect)
{
  m_sound_effects.push_back(effect);
}

bool can_move(
  const piece_type& type,
  const square& from,
  const square& to,
  const side player
)
{
  switch (type)
  {
    case piece_type::king:
      return are_on_same_rank(from, to)
        || are_on_same_file(from, to)
        || are_on_same_diagonal(from, to)
      ;
    case piece_type::pawn:
      return are_on_same_file(from, to)
        && is_forward(from, to, player)
      ;
    case piece_type::rook:
      return are_on_same_rank(from, to)
        || are_on_same_file(from, to)
      ;
    case piece_type::queen:
      return are_on_same_rank(from, to)
        || are_on_same_file(from, to)
        || are_on_same_diagonal(from, to)
      ;
    case piece_type::bishop:
      return are_on_same_diagonal(from, to);
    default:
    case piece_type::knight:
      assert(type == piece_type::knight);
      return are_on_same_half_diagonal(from, to);
  }
}

void clear_actions(piece& p)
{
  p.get_actions().clear();
  assert(count_piece_actions(p) == 0);
}

void piece::clear_sound_effects() noexcept
{
  m_sound_effects.clear();
  assert(m_sound_effects.empty());
}

int count_piece_actions(const piece& p)
{
  return static_cast<int>(p.get_actions().size());
}

std::string describe_actions(const piece& p)
{
  const auto& actions = p.get_actions();
  if (actions.empty()) return "idle";
  std::stringstream s;
  std::transform(
    std::begin(actions),
    std::end(actions),
    std::ostream_iterator<std::string>(s, ", "),
    [](const piece_action& action) { return describe_action(action); }
  );
  std::string t = s.str();
  t.pop_back();
  t.pop_back();
  return t;
}


double get_f_health(const piece& p) noexcept
{

  return p.get_health() / p.get_max_health();
}

square get_occupied_square(const piece& p) noexcept
{
  return p.get_current_square();
}

piece get_test_white_king() noexcept
{
  return piece(
    chess_color::white,
    piece_type::king,
    square("e1"),
    side::lhs
  );
}

piece get_test_white_knight() noexcept
{
  return piece(
    chess_color::white,
    piece_type::knight,
    square("c3"),
    side::lhs
  );
}

bool has_actions(const piece& p) noexcept
{
  return count_piece_actions(p) != 0;
}

bool is_idle(const piece& p) noexcept
{
  return !has_actions(p);
}

void select(piece& p) noexcept
{
  p.set_selected(true);
}


void piece::set_selected(const bool is_selected) noexcept
{
  m_is_selected = is_selected;
}

void test_piece()
{
#ifndef NDEBUG
  // can_move, on empty board
  {
    assert(can_move(piece_type::bishop, square("e4"), square("d3"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("d4"), side::lhs));
    assert(can_move(piece_type::bishop, square("e4"), square("d5"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("e3"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("e5"), side::lhs));
    assert(can_move(piece_type::bishop, square("e4"), square("f3"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("f4"), side::lhs));
    assert(can_move(piece_type::bishop, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::bishop, square("e4"), square("f6"), side::lhs));

    assert(can_move(piece_type::king, square("e4"), square("d3"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("d4"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("d5"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("e3"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("e5"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("f3"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("f4"), side::lhs));
    assert(can_move(piece_type::king, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::king, square("e4"), square("f6"), side::lhs));

    assert(!can_move(piece_type::knight, square("e4"), square("d3"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("d4"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("d5"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("e3"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("e5"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("f3"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("f4"), side::lhs));
    assert(!can_move(piece_type::knight, square("e4"), square("f5"), side::lhs));
    assert(can_move(piece_type::knight, square("e4"), square("f6"), side::lhs));

    assert(!can_move(piece_type::pawn, square("e4"), square("d3"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("d4"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("d5"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("e3"), side::lhs));
    assert(can_move(piece_type::pawn, square("e4"), square("e5"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("f3"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("f4"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("f6"), side::lhs));

    assert(can_move(piece_type::pawn, square("e4"), square("e3"), side::rhs));
    assert(!can_move(piece_type::pawn, square("e4"), square("e5"), side::rhs));

    assert(can_move(piece_type::queen, square("e4"), square("d3"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("d4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("d5"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e3"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e5"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("f3"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("f4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::queen, square("e4"), square("f6"), side::lhs));

    assert(!can_move(piece_type::rook, square("e4"), square("d3"), side::lhs));
    assert(can_move(piece_type::rook, square("e4"), square("d4"), side::lhs));
    assert(!can_move(piece_type::rook, square("e4"), square("d5"), side::lhs));
    assert(can_move(piece_type::rook, square("e4"), square("e3"), side::lhs));
    assert(can_move(piece_type::rook, square("e4"), square("e5"), side::lhs));
    assert(!can_move(piece_type::rook, square("e4"), square("f3"), side::lhs));
    assert(can_move(piece_type::rook, square("e4"), square("f4"), side::lhs));
    assert(!can_move(piece_type::rook, square("e4"), square("f5"), side::lhs));
    assert(!can_move(piece_type::rook, square("e4"), square("f6"), side::lhs));
  }
  // count_piece_actions
  {
    const auto p{get_test_white_king()};
    assert(count_piece_actions(p) == 0);
  }
  // describe_actions
  {
    const auto p{get_test_white_king()};
    assert(!describe_actions(p).empty());
  }
  {
    auto p{get_test_white_king()};
    p.add_action(piece_action(piece_action_type::attack, square("a3")));
    assert(!describe_actions(p).empty());
  }
  // piece::get_current_square
  {
    const auto p{get_test_white_king()};
    assert(p.get_current_square() == square("e1"));
  }
  // get_health
  {
    const auto p{get_test_white_king()};
    assert(p.get_health() > 0.0);
  }
  // get_f_health
  {
    const auto p{get_test_white_king()};
    assert(get_f_health(p) == 1.0);
  }
  // get_max_health
  {
    const auto p{get_test_white_king()};
    assert(p.get_max_health() > 0.0);
  }
  // get_occupied_square
  {
    const auto p{get_test_white_king()};
    assert(get_occupied_square(p) == square("e1"));
  }
  // get_test_white_king
  {
    const auto p{get_test_white_king()};
    assert(p.get_type() == piece_type::king);
    assert(p.get_color() == chess_color::white);
  }
  // get_test_white_knight
  {
    const auto p{get_test_white_knight()};
    assert(p.get_type() == piece_type::knight);
    assert(p.get_color() == chess_color::white);
  }
  // has_actions
  {
    const auto p{get_test_white_king()};
    assert(!has_actions(p));
  }
  // is_idle
  {
    const auto p{get_test_white_king()};
    assert(is_idle(p));
  }
  // toggle_select
  {
    auto p{get_test_white_king()};
    assert(!p.is_selected());
    toggle_select(p);
    assert(p.is_selected());
    toggle_select(p);
    assert(!p.is_selected());
  }
  // A pawn for the lhs player can move right
  {
    piece p(
      chess_color::white,
      piece_type::pawn,
      square("e2"),
      side::lhs
    );
    p.add_action(piece_action(piece_action_type::move, square("e2"), square("e4")));
    assert(!p.get_actions().empty());
    p.tick(delta_t(1.0));
    assert(!p.get_actions().empty());
    assert(p.get_coordinat() == square("e3"));
  }
  // A pawn for the rhs player cannot move right
  {
    piece p(
      chess_color::white,
      piece_type::pawn,
      square("e7"),
      side::lhs
    );

    p.add_action(piece_action(piece_action_type::move, square("e7"), square("e5")));
    //assert(!p.get_actions().empty());
    //p.tick(delta_t(1.0));
    //p.tick(delta_t(1.0));
    assert(p.get_actions().empty()); // Actions cleared
    assert(p.get_coordinat() == square("e7")); // Piece stays put
  }
  // operator==
  {
    const auto a{get_test_white_king()};
    const auto b{get_test_white_king()};
    const piece c{chess_color::black, piece_type::pawn, game_coordinat(), side::lhs};
    assert(a == b);
    assert(!(a == c));
  }
  // operator!=
  {
    const auto a{get_test_white_king()};
    const auto b{get_test_white_king()};
    const piece c{chess_color::black, piece_type::pawn, game_coordinat(), side::lhs};
    assert(!(a != b));
    assert(a != c);
  }
  // A pieces moves until it arrives
  {
    piece p{get_test_white_king()}; // A white king
    assert(p.get_type() == piece_type::king);
    assert(square(p.get_coordinat()) == square("e1"));
    p.add_action(piece_action(piece_action_type::move, square("e1"), square("e2")));
    assert(has_actions(p));
    p.tick(delta_t(0.1));
    assert(has_actions(p));
    for (int i{0}; i != 10; ++i)
    {
      p.tick(delta_t(0.1));
    }
    assert(!has_actions(p));
    const auto final_square{square(p.get_coordinat())};
    const auto expected_square{square("e2")};
    assert(final_square == expected_square);
  }
  // A pieces can attack
  {
    piece p{get_test_white_king()}; // A white king
    assert(p.get_type() == piece_type::king);
    assert(square(p.get_coordinat()) == square("e1"));
    p.add_action(piece_action(piece_action_type::attack, square("e2")));
    assert(has_actions(p));
    p.tick(delta_t(1.0));
  }
  // A knight never occupied squares between its source and target square
  {
    piece p{get_test_white_knight()};
    assert(p.get_type() == piece_type::knight);
    assert(square(p.get_coordinat()) == square("c3"));
    p.add_action(piece_action(piece_action_type::move, square("c3"), square("e4")));
    assert(has_actions(p));
    p.tick(delta_t(0.1));
    assert(has_actions(p));
    assert(get_occupied_square(p) == square("c3"));
    for (int i{0}; i != 30; ++i) // It is quite a travel
    {
      p.tick(delta_t(0.1));
      assert(get_occupied_square(p) != square("d3"));
      assert(get_occupied_square(p) != square("d4"));
    }
    assert(!has_actions(p));
    assert(square(p.get_coordinat()) == square("e4"));
    assert(get_occupied_square(p) == square("e4"));
  }
#endif // NDEBUG
}

void piece::tick(const delta_t& dt)
{
  if (m_actions.empty()) return;
  const auto& first_action{m_actions[0]};
  if (first_action.get_type() == piece_action_type::move)
  {
    const auto distance_from_start{
      calc_length(
        to_coordinat(first_action.get_from()) - m_coordinat)
      };
    const auto distance_to_target{
      calc_length(
        to_coordinat(first_action.get_to()) - m_coordinat
      )
    };
    // Occupy different square?
    if (distance_to_target < distance_from_start)
    {
      m_current_square = first_action.get_to();
    }
    if (distance_to_target < dt.get())
    {
      // Arrive at next the square
      m_coordinat = to_coordinat(first_action.get_to());

      // Done moving
      remove_first(m_actions);
    }
    else
    {
      const auto full_length{
        calc_length(
          to_coordinat(first_action.get_to()) - m_coordinat
        )
      };
      const auto delta{
        (to_coordinat(first_action.get_to()) - m_coordinat)
        / (full_length / dt.get())
      };
      m_coordinat += delta;
    }

  }
  else
  {
    assert(first_action.get_type() == piece_action_type::attack);
  }
}

void toggle_select(piece& p) noexcept
{
  p.set_selected(!p.is_selected());
}

void unselect(piece& p) noexcept
{
  p.set_selected(false);
}

bool operator==(const piece& lhs, const piece& rhs) noexcept
{
  return lhs.get_type() == rhs.get_type()
    && lhs.get_color() == rhs.get_color()
    && lhs.get_health() == rhs.get_health()
    && lhs.get_actions() == rhs.get_actions()
    && lhs.is_selected() == rhs.is_selected()
    && lhs.get_coordinat() == rhs.get_coordinat()
    && lhs.get_max_health() == rhs.get_max_health()
    && lhs.get_player() == rhs.get_player()
  ;
}

bool operator!=(const piece& lhs, const piece& rhs) noexcept
{
  return !(lhs == rhs);
}
