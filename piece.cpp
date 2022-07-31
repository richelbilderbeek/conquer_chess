#include "piece.h"

#include "helper.h"
#include "piece_type.h"
#include "square.h"
#include "message.h"
#include "game.h"

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
    //m_coordinat{to_coordinat(coordinat)},
    m_current_action_time{delta_t(0.0)},
    m_current_square{coordinat},
    m_health{::get_max_health(type)},
    m_id{create_new_id()},
    m_is_selected{false},
    m_kill_count{0},
    m_max_health{::get_max_health(type)},
    m_player{player},
    m_target_square{},
    m_type{type}
{

}

void piece::add_action(const piece_action& action)
{
  assert(action.get_piece_type() == m_type);
  assert(action.get_player() == m_player);

  if (action.get_action_type() == piece_action_type::move)
  {
    if (
      !can_move(
        this->get_type(),
        square(action.get_from()),
        square(action.get_to()),
        this->get_player()
      )
    )
    {
      this->add_message(message_type::cannot);
      return;
    }
    else
    {
      this->add_message(message_type::start_move);
    }
  }
  else
  {
    assert(action.get_action_type() == piece_action_type::attack);
    if (
      !can_attack(
        this->get_type(),
        square(action.get_from()),
        square(action.get_to()),
        this->get_player()
      )
    )
    {
      this->add_message(message_type::cannot);
      return;
    }
    else
    {
      this->add_message(message_type::start_attack);
    }
  }
  const std::vector<piece_action> atomic_actions{
    to_atomic(action)
  };
  // If the first atomic action is an invalid move,
  // Only keep the actions that are valid
  std::copy_if(
    std::begin(atomic_actions),
    std::end(atomic_actions),
    std::back_inserter(m_actions),
    [this](const piece_action& a)
    {
      if (a.get_action_type() == piece_action_type::move)
      {
        return can_move(this->get_type(), a.get_from(), a.get_to(), this->get_player());
      }
      else
      {
        assert(a.get_action_type() == piece_action_type::attack);
        return can_attack(this->get_type(), a.get_from(), a.get_to(), this->get_player());
      }
    }
  );
}

void piece::add_message(const message_type& message)
{
  m_messages.push_back(message);
}

bool can_attack(
  const piece_type& type,
  const square& from,
  const square& to,
  const side player
)
{
  if (from == to) return false;
  switch (type)
  {
    case piece_type::king:
      return can_move(type, from, to, player)
        && are_adjacent(from, to)
      ;
    case piece_type::pawn:
      return are_on_adjacent_diagonal(from, to)
        && is_forward(from, to, player)
      ;
    case piece_type::rook:
      return can_move(type, from, to, player);
    case piece_type::queen:
      return can_move(type, from, to, player);
    case piece_type::bishop:
      return can_move(type, from, to, player);
    default:
    case piece_type::knight:
      assert(type == piece_type::knight);
      return can_move(type, from, to, player)
        && are_adjacent_for_knight(from, to)
      ;
  }
}

/// Can a piece capture from 'from' to 'to'?
/// This function assumes the board is empty
bool can_capture(
  const piece_type& p,
  const square& from,
  const square& to,
  const side player
)
{
  return can_attack(p, from, to, player);
}

bool can_move(
  const piece_type& type,
  const square& from,
  const square& to,
  const side player
)
{
  // A piece can always move home
  if (from == to) return true;

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

void piece::clear_messages() noexcept
{
  m_messages.clear();
  assert(m_messages.empty());
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

game_coordinat piece::get_coordinat() const noexcept
{
  if (m_actions.empty()) return to_coordinat(m_current_square);
  const auto first_action{m_actions[0]};
  if (first_action.get_action_type() == piece_action_type::attack)
  {
    return to_coordinat(m_current_square);
  }
  assert(first_action.get_action_type() == piece_action_type::move);
  const auto from{to_coordinat(m_current_square)};
  const auto to{to_coordinat(first_action.get_to())};
  const auto full_delta{to - from};
  const auto delta{full_delta * get_current_action_time().get()};
  return from + delta;


}

double get_f_health(const piece& p) noexcept
{

  return p.get_health() / p.get_max_health();
}

square get_occupied_square(const piece& p) noexcept
{
  return p.get_current_square();
}

piece get_rotated_piece(const piece& p) noexcept
{
  piece q = p;
  q.set_current_square(get_rotated_square(p.get_current_square()));
  //q.set_coordinat(get_rotated_coordinat(p.get_coordinat()));
  return q;
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


bool is_dead(const piece& p) noexcept
{
  return p.get_health() <= 0.0;
}

bool is_idle(const piece& p) noexcept
{
  return !has_actions(p);
}

void piece::receive_damage(const double damage)
{
  assert(damage > 0.0);
  m_health -= damage;
}

void select(piece& p) noexcept
{
  p.set_selected(true);
}

void piece::set_current_action_time(const delta_t& t) noexcept
{
  m_current_action_time = t;
}

void piece::set_selected(const bool is_selected) noexcept
{
  if (!m_is_selected && is_selected)
  {
    add_message(message_type::select);
  }
  m_is_selected = is_selected;
}

void test_piece()
{
#ifndef NDEBUG
  ////////////////////////////////////////////////////////////////////////////
  // Member functions
  ////////////////////////////////////////////////////////////////////////////
  // piece::add_action
  {
    // start_move for a correct move results in a sound
    {
      auto piece{get_test_white_knight()};
      assert(piece.get_current_square() == square("c3"));
      assert(piece.get_messages().empty());
      assert(is_idle(piece));
      piece.add_action(piece_action(side::lhs, piece_type::knight, piece_action_type::move, square("c3"), square("d5")));
      game g{get_kings_only_game()};
      piece.tick(delta_t(0.1), g);
      assert(!piece.get_actions().empty()); // Yep, let's start moving
      assert(!piece.get_messages().empty());
      assert(piece.get_messages().at(0) == message_type::start_move);
    }
    // move for an invalid move results in a sound
    {
      auto piece{get_test_white_knight()};
      assert(piece.get_current_square() == square("c3"));
      assert(piece.get_messages().empty());
      assert(is_idle(piece));
      piece.add_action(piece_action(side::lhs, piece_type::knight, piece_action_type::move, square("c3"), square("h8")));
      game g{get_kings_only_game()};
      piece.tick(delta_t(0.1), g);
      assert(piece.get_actions().empty()); // Nope, cannot do that
      assert(!piece.get_messages().empty());
      assert(piece.get_messages().at(0) == message_type::cannot);
    }
    // attack for an invalid attack results in a sound
    {
      auto piece{get_test_white_knight()};
      assert(piece.get_current_square() == square("c3"));
      assert(piece.get_messages().empty());
      assert(is_idle(piece));
      piece.add_action(piece_action(side::lhs, piece_type::knight, piece_action_type::attack, square("c3"), square("d4")));
      game g{get_kings_only_game()};
      piece.tick(delta_t(0.1), g);
      assert(piece.get_actions().empty()); // Nope, cannot do that
      assert(!piece.get_messages().empty());
      assert(piece.get_messages().at(0) == message_type::cannot);
    }
  }
  // piece::get_kill_count
  {
    const auto piece{get_test_white_knight()};
    assert(piece.get_kill_count() == 0);
  }
  // piece::get_messages
  {
    auto piece{get_test_white_knight()};
    assert(piece.get_messages().empty());
  }
  // piece::receive_damage
  {
    auto piece{get_test_white_knight()};
    const auto health_before{piece.get_health()};
    piece.receive_damage(0.1);
    const auto health_after{piece.get_health()};
    assert(health_after < health_before);
  }
  ////////////////////////////////////////////////////////////////////////////
  // Free functions
  ////////////////////////////////////////////////////////////////////////////
  // can_attack, on empty board
  {
    assert(can_attack(piece_type::bishop, square("e4"), square("d3"), side::lhs));
    assert(!can_attack(piece_type::bishop, square("e4"), square("d4"), side::lhs));
    assert(can_attack(piece_type::bishop, square("e4"), square("d5"), side::lhs));
    assert(!can_attack(piece_type::bishop, square("e4"), square("e3"), side::lhs));
    assert(!can_attack(piece_type::bishop, square("e4"), square("e5"), side::lhs));
    assert(can_attack(piece_type::bishop, square("e4"), square("f3"), side::lhs));
    assert(!can_attack(piece_type::bishop, square("e4"), square("f4"), side::lhs));
    assert(can_attack(piece_type::bishop, square("e4"), square("f5"), side::lhs));
    assert(!can_attack(piece_type::bishop, square("e4"), square("f6"), side::lhs));

    assert(can_attack(piece_type::king, square("e4"), square("d3"), side::lhs));
    assert(can_attack(piece_type::king, square("e4"), square("d4"), side::lhs));
    assert(can_attack(piece_type::king, square("e4"), square("d5"), side::lhs));
    assert(can_attack(piece_type::king, square("e4"), square("e3"), side::lhs));
    assert(can_attack(piece_type::king, square("e4"), square("e5"), side::lhs));
    assert(can_attack(piece_type::king, square("e4"), square("f3"), side::lhs));
    assert(can_attack(piece_type::king, square("e4"), square("f4"), side::lhs));
    assert(can_attack(piece_type::king, square("e4"), square("f5"), side::lhs));
    assert(!can_attack(piece_type::king, square("e4"), square("f6"), side::lhs));

    assert(!can_attack(piece_type::knight, square("e4"), square("d3"), side::lhs));
    assert(!can_attack(piece_type::knight, square("e4"), square("d4"), side::lhs));
    assert(!can_attack(piece_type::knight, square("e4"), square("d5"), side::lhs));
    assert(!can_attack(piece_type::knight, square("e4"), square("e3"), side::lhs));
    assert(!can_attack(piece_type::knight, square("e4"), square("e5"), side::lhs));
    assert(!can_attack(piece_type::knight, square("e4"), square("f3"), side::lhs));
    assert(!can_attack(piece_type::knight, square("e4"), square("f4"), side::lhs));
    assert(!can_attack(piece_type::knight, square("e4"), square("f5"), side::lhs));
    assert(can_attack(piece_type::knight, square("e4"), square("f6"), side::lhs));

    // Knights cannot attack over distance
    assert(!can_attack(piece_type::knight, square("e4"), square("g8"), side::lhs));

    assert(!can_attack(piece_type::pawn, square("e4"), square("d3"), side::lhs));
    assert(!can_attack(piece_type::pawn, square("e4"), square("d4"), side::lhs));
    assert(can_attack(piece_type::pawn, square("e4"), square("d5"), side::lhs));
    assert(!can_attack(piece_type::pawn, square("e4"), square("e3"), side::lhs));
    assert(!can_attack(piece_type::pawn, square("e4"), square("e5"), side::lhs));
    assert(!can_attack(piece_type::pawn, square("e4"), square("f3"), side::lhs));
    assert(!can_attack(piece_type::pawn, square("e4"), square("f4"), side::lhs));
    assert(can_attack(piece_type::pawn, square("e4"), square("f5"), side::lhs));
    assert(!can_attack(piece_type::pawn, square("e4"), square("f6"), side::lhs));

    assert(!can_attack(piece_type::pawn, square("e4"), square("e5"), side::rhs));
    assert(can_attack(piece_type::pawn, square("e4"), square("d3"), side::rhs));
    assert(can_attack(piece_type::pawn, square("e4"), square("f3"), side::rhs));
    assert(!can_attack(piece_type::pawn, square("e4"), square("f4"), side::rhs));
    assert(!can_attack(piece_type::pawn, square("e4"), square("f5"), side::rhs));

    assert(can_attack(piece_type::queen, square("e4"), square("d3"), side::lhs));
    assert(can_attack(piece_type::queen, square("e4"), square("d4"), side::lhs));
    assert(can_attack(piece_type::queen, square("e4"), square("d5"), side::lhs));
    assert(can_attack(piece_type::queen, square("e4"), square("e3"), side::lhs));
    assert(can_attack(piece_type::queen, square("e4"), square("e5"), side::lhs));
    assert(can_attack(piece_type::queen, square("e4"), square("f3"), side::lhs));
    assert(can_attack(piece_type::queen, square("e4"), square("f4"), side::lhs));
    assert(can_attack(piece_type::queen, square("e4"), square("f5"), side::lhs));
    assert(!can_attack(piece_type::queen, square("e4"), square("f6"), side::lhs));

    assert(!can_attack(piece_type::rook, square("e4"), square("d3"), side::lhs));
    assert(can_attack(piece_type::rook, square("e4"), square("d4"), side::lhs));
    assert(!can_attack(piece_type::rook, square("e4"), square("d5"), side::lhs));
    assert(can_attack(piece_type::rook, square("e4"), square("e3"), side::lhs));
    assert(can_attack(piece_type::rook, square("e4"), square("e5"), side::lhs));
    assert(!can_attack(piece_type::rook, square("e4"), square("f3"), side::lhs));
    assert(can_attack(piece_type::rook, square("e4"), square("f4"), side::lhs));
    assert(!can_attack(piece_type::rook, square("e4"), square("f5"), side::lhs));
    assert(!can_attack(piece_type::rook, square("e4"), square("f6"), side::lhs));
  }
  // can_capture
  {
    // The same as can_attack
    assert(can_capture(piece_type::king, square("e4"), square("d3"), side::lhs));
  }
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

    assert(can_move(piece_type::queen, square("e4"), square("e1"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e2"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e3"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e4"), side::lhs)); // Can move home
    assert(can_move(piece_type::queen, square("e4"), square("e5"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e6"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e7"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e8"), side::lhs));

    assert(can_move(piece_type::queen, square("e4"), square("a4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("b4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("c4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("d4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("e4"), side::lhs)); // Can move home
    assert(can_move(piece_type::queen, square("e4"), square("f4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("g4"), side::lhs));
    assert(can_move(piece_type::queen, square("e4"), square("h4"), side::lhs));

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
    p.add_action(piece_action(side::lhs, piece_type::king, piece_action_type::attack, square("a3"), square("a4")));
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
  // get_rotated_piece
  {
    const auto p{get_test_white_king()};
    assert(get_occupied_square(p) == square("e1"));
    const auto q{get_rotated_piece(p)};
    assert(get_occupied_square(q) == square("d8"));
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
    p.add_action(piece_action(side::lhs, piece_type::pawn, piece_action_type::move, square("e2"), square("e4")));
    assert(!p.get_actions().empty());
    int n_ticks{0};
    game g;
    while (p.get_current_square() == square("e2"))
    {
      p.tick(delta_t(0.1), g);
      ++n_ticks;
      assert(n_ticks < 1000);
    }
    while (p.get_current_square() == square("e3"))
    {
      p.tick(delta_t(0.1), g);
      ++n_ticks;
      assert(n_ticks < 1000);
    }
    assert(p.get_current_square() == square("e4"));
  }
  // A pawn for the rhs player cannot move right
  {
    piece p(
      chess_color::white,
      piece_type::pawn,
      square("e7"),
      side::lhs
    );

    p.add_action(piece_action(side::lhs, piece_type::pawn, piece_action_type::move, square("e7"), square("e5")));
    assert(p.get_actions().empty()); // Actions cleared
    assert(p.get_current_square() == square("e7")); // Piece stays put
  }
  // A queen for the lhs player can move up
  {
    piece p(
      chess_color::white,
      piece_type::queen,
      square("h5"),
      side::lhs
    );
    p.add_action(piece_action(side::lhs, piece_type::queen, piece_action_type::move, square("h5"), square("g5")));
    assert(!p.get_actions().empty());
    int n_ticks{0};
    game g = get_game_with_starting_position(starting_position_type::before_scholars_mate);
    assert(count_piece_actions(p) == 1);
    p.tick(delta_t(0.1), g);
    assert(count_piece_actions(p) == 1);
    while (p.get_current_square() == square("h5"))
    {
      p.tick(delta_t(0.1), g);
      assert(count_piece_actions(p) == 1);
      ++n_ticks;
      assert(n_ticks < 100);
    }
    assert(p.get_current_square() == square("g5"));
  }
  // operator==
  {
    const auto a{get_test_white_king()};
    const auto b{get_test_white_king()};
    const piece c{get_test_white_knight()};
    assert(a == b);
    assert(!(a == c));
  }
  // operator!=
  {
    const auto a{get_test_white_king()};
    const auto b{get_test_white_king()};
    const piece c{get_test_white_knight()};
    assert(!(a != b));
    assert(a != c);
  }
  // A pieces moves until it arrives
  {
    piece p{get_test_white_king()}; // A white king
    assert(p.get_type() == piece_type::king);
    assert(square(p.get_coordinat()) == square("e1"));
    p.add_action(piece_action(side::lhs, piece_type::king, piece_action_type::move, square("e1"), square("e2")));
    assert(has_actions(p));
    game g{get_kings_only_game()};
    p.tick(delta_t(0.1), g);
    assert(has_actions(p));
    for (int i{0}; i != 10; ++i)
    {
      p.tick(delta_t(0.1), g);
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
    p.add_action(piece_action(side::lhs, piece_type::king, piece_action_type::attack, square("e2"), square("e3")));
    assert(has_actions(p));
    game g{get_kings_only_game()};
    p.tick(delta_t(1.0), g);
  }
  // A piece stops attacking when the attacked piece moves away
  {
    game g{get_game_with_starting_position(starting_position_type::queen_end_game)};
    piece& white_queen{get_piece_at(g, square("d1"))};
    piece& black_queen{get_piece_at(g, square("d8"))};
    white_queen.add_action(piece_action(side::lhs, piece_type::queen, piece_action_type::attack, square("d1"), square("d8")));
    assert(has_actions(white_queen));
    black_queen.add_action(piece_action(side::rhs, piece_type::queen, piece_action_type::move, square("d8"), square("a8")));
    assert(has_actions(white_queen));
    for (int i{0}; i != 10; ++i)
    {
      white_queen.tick(delta_t(0.1), g);
      black_queen.tick(delta_t(0.1), g);
    }
    // Black queen is shot, but survives
    assert(get_f_health(black_queen) < 1.0);
  }
  // A knight never occupied squares between its source and target square
  {
    piece p{get_test_white_knight()};
    assert(p.get_type() == piece_type::knight);
    assert(square(p.get_coordinat()) == square("c3"));
    p.add_action(piece_action(side::lhs, piece_type::knight, piece_action_type::move, square("c3"), square("e4")));
    assert(has_actions(p));
    game g{get_kings_only_game()};
    p.tick(delta_t(0.1), g);
    assert(has_actions(p));
    assert(get_occupied_square(p) == square("c3"));
    for (int i{0}; i != 30; ++i) // It is quite a travel
    {
      p.tick(delta_t(0.1), g);
      assert(get_occupied_square(p) != square("d3"));
      assert(get_occupied_square(p) != square("d4"));
    }
    assert(!has_actions(p));
    assert(square(p.get_coordinat()) == square("e4"));
    assert(get_occupied_square(p) == square("e4"));
  }
#endif // NDEBUG
}

void piece::tick(
  const delta_t& dt,
  game& g
)
{
  if (m_actions.empty()) return;
  const auto action_type{m_actions[0].get_action_type()};
  std::clog << get_color() << " " << get_type() << " going to " << action_type << '\n';

  switch(action_type)
  {
    case piece_action_type::move:
      return tick_move(*this, dt, g);
    case piece_action_type::attack:
    default:
      assert(m_actions[0].get_action_type() == piece_action_type::attack);
      return tick_attack(*this, dt, g);
  }
}

void tick_attack(
  piece& p,
  const delta_t& dt,
  game& g
)
{
  assert(!p.get_actions().empty());
  const auto& first_action{p.get_actions()[0]};
  assert(first_action.get_action_type() == piece_action_type::attack);
  // Done if piece moved away
  if (!is_piece_at(g, first_action.get_to()))
  {
    p.add_message(message_type::cannot);
    remove_first(p.get_actions());
    return;
  }
  assert(is_piece_at(g, first_action.get_to()));
  piece& target{get_piece_at(g, first_action.get_to())};

  // Done if target is of own color
  if (p.get_color() == target.get_color())
  {
    p.add_message(message_type::cannot);
    remove_first(p.get_actions());
    return;
  }
  assert(p.get_color() != target.get_color());
  const auto damage{g.get_options().get_damage_per_chess_move() * dt.get()};
  target.receive_damage(damage);
  // Capture the piece if destroyed
  if (is_dead(target))
  {
    p.increase_kill_count();
    p.set_current_square(first_action.get_to()); // Capture
    remove_first(p.get_actions());
  }
}

void tick_move(
  piece& p,
  const delta_t& dt,
  game& g
)
{
  assert(!p.get_actions().empty());
  const auto& first_action{p.get_actions()[0]};
  assert(first_action.get_action_type() == piece_action_type::move);

  // Increase the progress of the action
  p.set_current_action_time(p.get_current_action_time() + dt);
  const double f_too_much{p.get_current_action_time().get()};
  assert(f_too_much >= 0.0);

  // Are we done with the action?
  if (f_too_much >= 1.0)
  {
    // The whole goal of the operation
    assert(p.get_current_square() == first_action.get_to());
    p.set_current_action_time(delta_t(0.0));
    remove_first(p.get_actions());
    if (p.get_actions().empty())
    {
      p.add_message(message_type::done);
    }
    return;
  }

  const double f{std::min(1.0, f_too_much)}; // The fraction of the action done
  assert(f >= 0.0);
  assert(f <= 1.0);

  const auto occupied_squares{get_occupied_squares(g)};
  const bool is_target_occupied{is_occupied(first_action.get_to(), occupied_squares)};
  const bool is_focal_piece_at_target{p.get_current_square() == first_action.get_to()};

  if (is_target_occupied)
  {
    if (is_focal_piece_at_target)
    {
      // Moving the last half
      std::clog << "Piece not halfway (" << f << "), still occupies " << p.get_current_square() << '\n';
    }
    else
    {
      // Too bad, need to go back
      p.get_actions().clear();
      p.add_action(
        piece_action(
          get_piece_at(g, first_action.get_from()).get_player(),
          get_piece_at(g, first_action.get_from()).get_type(),
          piece_action_type::move,
          first_action.get_to(), // Reverse
          first_action.get_from()
        )
      );
      p.set_current_action_time(delta_t(1.0) - p.get_current_action_time()); // Keep progress
      p.add_message(message_type::cannot);
      std::clog << "I cannot" << '\n';
      return;
    }
  }
  else
  {
    assert(!is_target_occupied);
    if (f >= 0.5)
    {
      // If over halfway, occupy target
      assert(!is_occupied(first_action.get_to(), get_occupied_squares(g)));
      p.set_current_square(first_action.get_to());
      std::clog << "Piece over halfway (" << f << "), now occupies " << p.get_current_square() << '\n';
      // Maybe cannot check, as p is not fully updated in game?
      // assert(is_occupied(first_action.get_to(), get_occupied_squares(g)));
    }
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

std::ostream& operator<<(std::ostream& os, const piece& p) noexcept
{
  os
    << to_str(p.get_actions())
    << p.get_color()
    << p.get_coordinat()
    << p.get_current_square()
    << p.get_health()
    << p.get_id()
    << p.is_selected()
    << p.get_kill_count()
    << p.get_max_health()
    << p.get_player()
    << to_str(p.get_messages())
    << p.get_type()
  ;
  std::vector<square> m_target_square;
  return os;
}
