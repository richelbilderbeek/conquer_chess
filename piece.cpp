#include "piece.h"

#include "piece_type.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>

piece::piece(
  const chess_color color,
  const piece_type type,
  const game_coordinat& coordinat
)
  : m_color{color},
    m_coordinat{coordinat},
    m_health{::get_max_health(type)},
    m_is_selected{false},
    m_max_health{::get_max_health(type)},
    m_type{type}
{

}

void piece::add_action(const piece_action& action)
{
  m_actions.push_back(action);
}

void clear_actions(piece& p)
{
  p.get_actions().clear();
  assert(count_piece_actions(p) == 0);
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

std::vector<piece> get_standard_starting_pieces() noexcept
{
  return {
    piece(chess_color::white, piece_type::rook,   game_coordinat(0.5, 0.5)),
    piece(chess_color::white, piece_type::knight, game_coordinat(0.5, 1.5)),
    piece(chess_color::white, piece_type::bishop, game_coordinat(0.5, 2.5)),
    piece(chess_color::white, piece_type::king,   game_coordinat(0.5, 3.5)),
    piece(chess_color::white, piece_type::queen,  game_coordinat(0.5, 4.5)),
    piece(chess_color::white, piece_type::bishop, game_coordinat(0.5, 5.5)),
    piece(chess_color::white, piece_type::knight, game_coordinat(0.5, 6.5)),
    piece(chess_color::white, piece_type::rook,   game_coordinat(0.5, 7.5)),
    piece(chess_color::white, piece_type::pawn,   game_coordinat(1.5, 0.5)),
    piece(chess_color::white, piece_type::pawn,   game_coordinat(1.5, 1.5)),
    piece(chess_color::white, piece_type::pawn,   game_coordinat(1.5, 2.5)),
    piece(chess_color::white, piece_type::pawn,   game_coordinat(1.5, 3.5)),
    piece(chess_color::white, piece_type::pawn,   game_coordinat(1.5, 4.5)),
    piece(chess_color::white, piece_type::pawn,   game_coordinat(1.5, 5.5)),
    piece(chess_color::white, piece_type::pawn,   game_coordinat(1.5, 6.5)),
    piece(chess_color::white, piece_type::pawn,   game_coordinat(1.5, 7.5)),
    piece(chess_color::black, piece_type::rook,   game_coordinat(7.5, 0.5)),
    piece(chess_color::black, piece_type::knight, game_coordinat(7.5, 1.5)),
    piece(chess_color::black, piece_type::bishop, game_coordinat(7.5, 2.5)),
    piece(chess_color::black, piece_type::king,   game_coordinat(7.5, 3.5)),
    piece(chess_color::black, piece_type::queen,  game_coordinat(7.5, 4.5)),
    piece(chess_color::black, piece_type::bishop, game_coordinat(7.5, 5.5)),
    piece(chess_color::black, piece_type::knight, game_coordinat(7.5, 6.5)),
    piece(chess_color::black, piece_type::rook,   game_coordinat(7.5, 7.5)),
    piece(chess_color::black, piece_type::pawn,   game_coordinat(6.5, 0.5)),
    piece(chess_color::black, piece_type::pawn,   game_coordinat(6.5, 1.5)),
    piece(chess_color::black, piece_type::pawn,   game_coordinat(6.5, 2.5)),
    piece(chess_color::black, piece_type::pawn,   game_coordinat(6.5, 3.5)),
    piece(chess_color::black, piece_type::pawn,   game_coordinat(6.5, 4.5)),
    piece(chess_color::black, piece_type::pawn,   game_coordinat(6.5, 5.5)),
    piece(chess_color::black, piece_type::pawn,   game_coordinat(6.5, 6.5)),
    piece(chess_color::black, piece_type::pawn,   game_coordinat(6.5, 7.5))
  };
}

double get_f_health(const piece& p) noexcept
{

  return p.get_health() / p.get_max_health();
}

std::vector<piece> get_kings_only_starting_pieces() noexcept
{
  const auto all_pieces{get_standard_starting_pieces()};
  std::vector<piece> pieces;
  pieces.reserve(2);
  std::copy_if(
    std::begin(all_pieces),
    std::end(all_pieces),
    std::back_inserter(pieces),
    [](const auto& piece) { return piece.get_type() == piece_type::king; }
  );
  return pieces;
}

std::vector<piece> get_starting_pieces(const starting_position_type t) noexcept
{
  switch (t)
  {
    case starting_position_type::standard: return get_standard_starting_pieces();
    default:
    case starting_position_type::kings_only: return get_kings_only_starting_pieces();
  }
}

piece get_test_piece() noexcept
{
  return piece(
    chess_color::white,
    piece_type::king,
    game_coordinat(0.5, 3.5)
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
  // count_piece_actions
  {
    const auto p{get_test_piece()};
    assert(count_piece_actions(p) == 0);
  }
  // describe_actions
  {
    const auto p{get_test_piece()};
    assert(!describe_actions(p).empty());
  }
  // get_f_health
  {
    const auto p{get_test_piece()};
    assert(get_f_health(p) == 1.0);
  }
  // has_actions
  {
    const auto p{get_test_piece()};
    assert(!has_actions(p));
  }
  // is_idle
  {
    const auto p{get_test_piece()};
    assert(is_idle(p));
  }
  // toggle_select
  {
    auto p{get_test_piece()};
    assert(!p.is_selected());
    toggle_select(p);
    assert(p.is_selected());
    toggle_select(p);
    assert(!p.is_selected());
  }
#endif // NDEBUG
}

void piece::tick(const delta_t& dt)
{
  if (m_actions.empty()) return;
  const auto& first_action{m_actions[0]};
  if (first_action.get_type() == piece_action_type::move)
  {
    const auto full_delta{first_action.get_coordinat() - m_coordinat};
    const double full_length{calc_length(full_delta)};
    if (full_length < dt.get())
    {
      std::vector<decltype(m_actions)::value_type>(m_actions.begin() + 1, m_actions.end()).swap(m_actions);
    }
    const auto delta{full_delta / (full_length / dt.get())};
    m_coordinat += delta;

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
