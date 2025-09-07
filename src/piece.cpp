#include "piece.h"

#include "game.h"
#include "game_options.h"
#include "game_coordinate.h"
#include "helper.h"
#include "piece_type.h"
#include "square.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <sstream>

#ifdef GAME_CONTROLLER_H
#error 'pieces' must know nothing about 'game_controller'
#error Instead, it is the other way around...
#endif

#ifdef LOBBY_OPTIONS_H
#error 'pieces' must know nothing about 'lobby_options'
#error Instead, all player sides (e.g. LHS) need to be converted to colors (e.g. white)
#endif

#ifdef SIDE_H
#error 'pieces' must know nothing about 'side'
#error This is managed by 'lobby_options'.
#endif

#ifdef GAME_OPTION_H
#error 'pieces' must know nothing about 'game_option'
#error This is managed by 'lobby_options'.
#endif

piece::piece(
  const chess_color color,
  const piece_type type,
  const square& coordinat,
  const race r
)
  : m_color{color},
    m_current_action_progress{delta_t(0.0)},
    m_current_square{coordinat},
    m_has_moved{false},
    m_health{::get_max_health(r)},
    m_id{create_new_id()},
    m_in_game_time{in_game_time(0.0)},
    m_kill_count{0},
    m_max_health{::get_max_health(r)},
    m_max_shield{::get_max_shield(r)},
    m_race{r},
    m_shield{::get_max_shield(r)},
    m_type{type}
{

}

piece::piece(
  const chess_color color,
  const piece_type type,
  const std::string& coordinat,
  const race r
) : piece(color, type, square(coordinat), r) {

}


void piece::add_action(const piece_action& action)
{
  assert(action.get_piece_type() == get_type() || get_type() == piece_type::pawn);
  assert(action.get_color() == m_color.get_value());
  if (action.get_action_type() == piece_action_type::select)
  {
    this->add_message(message_type::select);
  }
  else if (action.get_action_type() == piece_action_type::unselect)
  {
    this->add_message(message_type::unselect);
  }
  else if (action.get_action_type() == piece_action_type::move)
  {
    if (
      !can_move_on_empty_board(
        get_color(),
        get_type(),
        square(action.get_from()),
        square(action.get_to())
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
  else if (action.get_action_type() == piece_action_type::attack)
  {
    if (
      !can_attack_on_empty_board(
        get_color(),
        get_type(),
        square(action.get_from()),
        square(action.get_to())
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
  else if (action.get_action_type() == piece_action_type::attack_en_passant)
  {
    this->add_message(message_type::start_en_passant_attack);
  }
  else if (action.get_action_type() == piece_action_type::castle_kingside)
  {
    this->add_message(message_type::start_castling_kingside);
  }
  else if (action.get_action_type() == piece_action_type::castle_queenside)
  {
    this->add_message(message_type::start_castling_queenside);
  }
  else
  {
    assert(
      action.get_action_type() == piece_action_type::promote_to_knight
      || action.get_action_type() == piece_action_type::promote_to_bishop
      || action.get_action_type() == piece_action_type::promote_to_rook
      || action.get_action_type() == piece_action_type::promote_to_queen
    );
    assert(can_promote(get_color(), get_type(), m_current_square));
    m_actions.push_back(action);
    return;
  }
  m_actions.push_back(action);
}

void piece::add_message(const message_type& message)
{
  m_messages.push_back(message);
}

bool can_attack_on_empty_board(
  const chess_color color,
  const piece_type& type,
  const square& from,
  const square& to
)
{
  if (from == to) return false;
  switch (type)
  {
    case piece_type::king:
      return can_move_on_empty_board(color, type, from, to)
        && are_adjacent(from, to)
      ;
    case piece_type::pawn:
      return are_on_adjacent_diagonal(from, to)
        && is_forward(color, from, to)
      ;
    case piece_type::rook:
      return can_move_on_empty_board(color, type, from, to);
    case piece_type::queen:
      return can_move_on_empty_board(color, type, from, to);
    case piece_type::bishop:
      return can_move_on_empty_board(color, type, from, to);
    default:
    case piece_type::knight:
      assert(type == piece_type::knight);
      return can_move_on_empty_board(color, type, from, to)
        && are_adjacent_for_knight(from, to)
      ;
  }
}

/// Can a piece capture from 'from' to 'to'?
/// This function assumes the board is empty
bool can_capture_on_empty_board(
  const chess_color color,
  const piece_type& p,
  const square& from,
  const square& to
)
{
  return can_attack_on_empty_board(color, p, from, to);
}

bool can_move_on_empty_board(
  const chess_color color,
  const piece_type& type,
  const square& from,
  const square& to
)
{
  // A piece can always move home
  if (from == to) return true;

  switch (type)
  {
    case piece_type::king:
      return are_adjacent(from, to);
    case piece_type::pawn:
    {
      if (!are_on_same_file(from, to)) return false;
      if (!is_forward(color, from, to)) return false;

      const int n_squares{std::abs(from.get_x() - to.get_x())};
      assert(n_squares > 0);
      assert(n_squares < 8);
      if (is_on_starting_rank(color, from))
      {
        return n_squares <= 2;
      }
      else
      {
        return n_squares == 1;
      }
    }
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
      return are_at_knights_jump_distance(from, to);
      return are_on_same_half_diagonal(from, to);
  }
}

bool can_promote(
  const chess_color color,
  const piece_type& type,
  const square& s
) noexcept
{
  if (type != piece_type::pawn) return false;
  if (color == chess_color::white) return s.get_x() == 7;
  assert(color == chess_color::black);
  return s.get_x() == 0;
}

bool can_promote(const piece& p) noexcept
{
  return can_promote(
    p.get_color(),
    p.get_type(),
    p.get_current_square()
  );
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

delta_t piece::get_current_action_progress() const
{
  assert(m_current_action_progress.get() >= 0.0);
  assert(m_current_action_progress.get() <= 1.0);
  return m_current_action_progress;
}

double get_f_health(const piece& p) noexcept
{
  return p.get_health() / p.get_max_health();
}

double get_f_shield(const piece& p) noexcept
{
  assert(p.get_race() == race::rooxx);
  return p.get_shield() / p.get_max_shield();
}

double piece::get_max_shield() const
{
  assert(m_race.get_value() == race::rooxx);
  return m_max_shield;
}

square get_occupied_square(const piece& p) noexcept
{
  return p.get_current_square();
}

double piece::get_shield() const
{
  assert(m_race.get_value() == race::rooxx);
  return m_shield;
}

piece get_test_white_king() noexcept
{
  return piece(
    chess_color::white,
    piece_type::king,
    square("e1")
  );
}

piece get_test_white_knight(const race r) noexcept
{
  return piece(
    chess_color::white,
    piece_type::knight,
    square("c3"),
    r
  );
}

piece get_test_white_pawn() noexcept
{
  return piece(
    chess_color::white,
    piece_type::pawn,
    square("e2")
  );
}

bool has_actions(const piece& p) noexcept
{
  return count_piece_actions(p) != 0;
}

bool has_just_double_moved(
  const piece& p,
  const in_game_time& when
) noexcept
{
  return has_just_double_moved(p.get_action_history(), when);
}

bool has_moved(const piece& p) noexcept
{
  return p.has_moved();
}

bool is_dead(const piece& p) noexcept
{
  return p.get_health() <= 0.0;
}

bool piece::is_enpassantable(
  const in_game_time& when
) const
{
  return has_just_double_moved(*this, when);
}


bool is_enpassantable(
  const piece& p,
  const in_game_time& when
)
{
  return p.is_enpassantable(when);
}

bool is_idle(const piece& p) noexcept
{
  return !has_actions(p);
}

bool is_on_starting_rank(const chess_color c, const square& q) noexcept
{
  const int starting_rank{c == chess_color::white ? 1 : 6};
  return q.get_x() == starting_rank;
}

bool is_pawn(const piece& p) noexcept
{
  return p.get_type() == piece_type::pawn;
}

bool is_selected(const piece& p)
{
  return is_piece_selected(p.get_action_history());
}

void piece::receive_damage(const double damage)
{
  assert(damage >= 0.0);
  m_shield -= damage;

  if (m_shield >= 0.0) return;

  const double physical_damage{-m_shield};
  m_shield = 0.0;
  m_health -= physical_damage;
}

void piece::set_current_action_progress(const delta_t& t) noexcept
{
  m_current_action_progress = t;
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
      piece.add_action(piece_action(chess_color::white, piece_type::knight, piece_action_type::move, square("c3"), square("d5")));
      game g(get_pieces_kings_only());
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
      piece.add_action(piece_action(chess_color::white, piece_type::knight, piece_action_type::move, square("c3"), square("h8")));
      game g(get_pieces_kings_only());
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
      piece.add_action(piece_action(chess_color::white, piece_type::knight, piece_action_type::attack, square("c3"), square("d4")));
      game g(get_pieces_kings_only());
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
  // piece::has_moved
  {
    // a piece has not moved at the start
    {
      const auto piece{get_test_white_knight()};
      assert(!piece.has_moved());
    }
    // a piece has moved after a movement
    {
      piece p(
        chess_color::white,
        piece_type::pawn,
        square("e2")
      );
      assert(!p.has_moved());
      p.add_action(piece_action(chess_color::white, piece_type::pawn, piece_action_type::move, square("e2"), square("e4")));
      game g;
      p.tick(delta_t(0.01), g);
      assert(p.has_moved());
    }
  }
  // piece::get_action_history, const
  {
    const auto piece{get_test_white_knight()};
    assert(piece.get_action_history().get().empty());
  }
  // piece::get_action_history, non-const
  {
    auto piece{get_test_white_knight()};
    auto& history{piece.get_action_history()};
    assert(history.get().empty());
  }
  // piece::get_messages
  {
    auto piece{get_test_white_knight()};
    assert(piece.get_messages().empty());
  }
  // classic piece reduces health
  {
    auto piece{get_test_white_knight()};
    const auto health_before{piece.get_health()};
    piece.receive_damage(0.1);
    const auto health_after{piece.get_health()};
    assert(health_after < health_before);
  }
  // rooxx piece first reduce shields
  {
    auto piece{get_test_white_knight(race::rooxx)};
    const auto health_before{piece.get_health()};
    const auto shield_before{piece.get_shield()};
    piece.receive_damage(0.1);
    const auto health_after{piece.get_health()};
    const auto shield_after{piece.get_shield()};
    assert(health_after == health_before);
    assert(shield_after < shield_before);
  }
  // rooxx piece first reduce shields, then health
  {
    auto piece{get_test_white_knight(race::rooxx)};
    while (piece.get_shield() > 0.0)
    {
      assert(piece.get_health() == piece.get_max_health());
      piece.receive_damage(0.1);
    }
    assert(piece.get_shield() == 0.0);

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
    assert(can_attack_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("d3")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("d4")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("d5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("e3")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("e5")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("f3")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("f4")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("f5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("f6")));

    assert(can_attack_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("d3")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("d4")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("d5")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("e3")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("e5")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("f3")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("f4")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("f5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("f6")));

    assert(!can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("d3")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("d4")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("d5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("e3")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("e5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("f3")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("f4")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("f5")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("f6")));

    // Knights cannot attack over distance
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("g8")));

    assert(!can_attack_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("d3")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("d4")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("d5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("e3")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("e5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("f3")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("f4")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("f5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("f6")));

    assert(!can_attack_on_empty_board(chess_color::black, piece_type::pawn, square("e4"), square("e5")));
    assert(can_attack_on_empty_board(chess_color::black, piece_type::pawn, square("e4"), square("d3")));
    assert(can_attack_on_empty_board(chess_color::black, piece_type::pawn, square("e4"), square("f3")));
    assert(!can_attack_on_empty_board(chess_color::black, piece_type::pawn, square("e4"), square("f4")));
    assert(!can_attack_on_empty_board(chess_color::black, piece_type::pawn, square("e4"), square("f5")));

    assert(can_attack_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("d3")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("d4")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("d5")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e3")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e5")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("f3")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("f4")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("f5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("f6")));

    assert(!can_attack_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("d3")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("d4")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("d5")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("e3")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("e5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("f3")));
    assert(can_attack_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("f4")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("f5")));
    assert(!can_attack_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("f6")));
  }
  // can_capture
  {
    // The same as can_attack
    assert(can_capture_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("d3")));
  }
  // can_move, on empty board
  {
    // For bishop movement, see test_rule_3_2

    // A king can move 1 square
    assert(can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("d3")));
    assert(can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("d4")));
    assert(can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("d5")));
    assert(can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("e3")));
    assert(can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("e5")));
    assert(can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("f3")));
    assert(can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("f4")));
    assert(can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("f5")));
    assert(!can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("f6")));
    assert(!can_move_on_empty_board(chess_color::white, piece_type::king, square("e4"), square("e6")));

    // For movement of the knight, see `test_rule_3_6`
    assert(!can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("d3")));
    assert(!can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("d4")));
    assert(!can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("d5")));
    assert(!can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("e3")));

    // Not in one time unit
    assert(!can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("e6")));
    assert(!can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("f3")));
    assert(!can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("f4")));
    assert(!can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("f5")));
    assert(!can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("f6")));

    assert(can_move_on_empty_board(chess_color::black, piece_type::pawn, square("e4"), square("e3")));
    assert(!can_move_on_empty_board(chess_color::black, piece_type::pawn, square("e4"), square("e5")));

    // For queen movement, see `test_rule_3_4`



    // For rook movement, see `test_rule_3_3`

  }
  // can_promote
  {
    assert(can_promote(chess_color::white, piece_type::pawn, square("e8")));
    assert(!can_promote(chess_color::white, piece_type::pawn, square("e1"))); // Impossible in practice
    assert(can_promote(chess_color::black, piece_type::pawn, square("e1")));
    assert(!can_promote(chess_color::black, piece_type::pawn, square("e8"))); // Impossible in practice
    assert(!can_promote(chess_color::white, piece_type::queen, square("e8")));
    assert(!can_promote(chess_color::black, piece_type::queen, square("e1")));
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
    p.add_action(piece_action(chess_color::white, piece_type::king, piece_action_type::attack, square("a3"), square("a4")));
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
  // get_f_shield, rooxx
  {
    const auto p{get_test_white_knight(race::rooxx)};
    assert(get_f_shield(p) > 0.1);
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
  // get_race
  {
    const auto rooxx_piece{get_test_white_knight(race::rooxx)};
    assert(rooxx_piece.get_race() == race::rooxx);
    const auto spawn_piece{get_test_white_knight(race::spawn)};
    assert(spawn_piece.get_race() == race::spawn);
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
  // get_test_white_pawn
  {
    const auto p{get_test_white_pawn()};
    assert(p.get_type() == piece_type::pawn);
    assert(p.get_color() == chess_color::white);
  }
  // has_actions
  {
    const auto p{get_test_white_king()};
    assert(!has_actions(p));
  }
  // has_just_double_moved, white
  {
    action_history h;
    assert(!has_just_double_moved(h, in_game_time(2.0)));
    h.add_action(
      in_game_time(0.5),
      piece_action(
        chess_color::white,
        piece_type::pawn,
        piece_action_type::move,
        "e2",
        "e4"
      )
    );
    assert(!has_just_double_moved(h, in_game_time(0.0)));
    assert(!has_just_double_moved(h, in_game_time(1.0)));
    assert(has_just_double_moved(h, in_game_time(2.0)));
    assert(!has_just_double_moved(h, in_game_time(3.0)));
  }
  // has_just_double_moved, black
  {
    action_history h;
    assert(!has_just_double_moved(h, in_game_time(2.0)));
    h.add_action(
      in_game_time(0.5),
      piece_action(
        chess_color::white,
        piece_type::pawn,
        piece_action_type::move,
        "e7",
        "e5"
      )
    );
    assert(!has_just_double_moved(h, in_game_time(0.0)));
    assert(!has_just_double_moved(h, in_game_time(1.0)));
    assert(has_just_double_moved(h, in_game_time(2.0)));
    assert(!has_just_double_moved(h, in_game_time(3.0)));
  }
  // is_enpassantable
  {
      const auto p{get_test_white_pawn()};
      assert(is_pawn(p));
      assert(!is_enpassantable(p, in_game_time(0.0)));
  }
  // is_enpassantable
  {
    action_history h;
    assert(!has_just_double_moved(h, in_game_time(2.0)));
    h.add_action(
      in_game_time(0.5),
      piece_action(
        chess_color::white,
        piece_type::pawn,
        piece_action_type::move,
        "e2",
        "e4"
      )
    );
    assert(!has_just_double_moved(h, in_game_time(0.0)));
    assert(!has_just_double_moved(h, in_game_time(1.0)));
    assert(has_just_double_moved(h, in_game_time(2.0)));
    assert(!has_just_double_moved(h, in_game_time(3.0)));

    assert(!is_enpassantable(h, in_game_time(0.0)));
    assert(!is_enpassantable(h, in_game_time(1.0)));
    assert(is_enpassantable(h, in_game_time(2.0)));
    assert(!is_enpassantable(h, in_game_time(3.0)));

  }
  // is_idle
  {
    const auto p{get_test_white_king()};
    assert(is_idle(p));
  }
  // is_on_starting_rank
  {
    assert(is_on_starting_rank(chess_color::white, square("a2")));
    assert(is_on_starting_rank(chess_color::black, square("a7")));
    assert(!is_on_starting_rank(chess_color::white, square("a7")));
    assert(!is_on_starting_rank(chess_color::black, square("a2")));
  }
  // to_char
  {
    assert(to_fen_char(piece(chess_color::white, piece_type::bishop, "e4")) == 'B');
    assert(to_fen_char(piece(chess_color::white, piece_type::king, "e4")) == 'K');
    assert(to_fen_char(piece(chess_color::white, piece_type::knight, "e4")) == 'N');
    assert(to_fen_char(piece(chess_color::white, piece_type::pawn, "e4")) == 'P');
    assert(to_fen_char(piece(chess_color::white, piece_type::queen, "e4")) == 'Q');
    assert(to_fen_char(piece(chess_color::white, piece_type::rook, "e4")) == 'R');

    assert(to_fen_char(piece(chess_color::black, piece_type::bishop, "e4")) == 'b');
    assert(to_fen_char(piece(chess_color::black, piece_type::king, "e4")) == 'k');
    assert(to_fen_char(piece(chess_color::black, piece_type::knight, "e4")) == 'n');
    assert(to_fen_char(piece(chess_color::black, piece_type::pawn, "e4")) == 'p');
    assert(to_fen_char(piece(chess_color::black, piece_type::queen, "e4")) == 'q');
    assert(to_fen_char(piece(chess_color::black, piece_type::rook, "e4")) == 'r');
  }
  // A pawn for the lhs player can move right
  {
    piece p(
      chess_color::white,
      piece_type::pawn,
      square("e2")
    );
    p.add_action(piece_action(chess_color::white, piece_type::pawn, piece_action_type::move, square("e2"), square("e4")));
    assert(!p.get_actions().empty());
    int n_ticks{0};
    game g;
    while (p.get_current_square() == square("e2"))
    {
      p.tick(delta_t(0.1), g);
      ++n_ticks;
      assert(n_ticks < 1000);
    }
    assert(p.get_current_square() != square("e3"));
    assert(p.get_current_square() == square("e4"));
  }
  // A pawn for the rhs player cannot move right
  {
    piece p(
      chess_color::white,
      piece_type::pawn,
      square("e7")
    );

    p.add_action(piece_action(chess_color::white, piece_type::pawn, piece_action_type::move, square("e7"), square("e5")));
    assert(p.get_actions().empty()); // Actions cleared
    assert(p.get_current_square() == square("e7")); // Piece stays put
  }
  // A pawn for the rhs player can move left
  {
    piece p(
      chess_color::black,
      piece_type::pawn,
      square("e7")
    );
    p.add_action(
      piece_action(
        chess_color::black,
        piece_type::pawn,
        piece_action_type::move,
        square("e7"), square("e5")
      )
    );
    assert(!p.get_actions().empty());
    int n_ticks{0};
    game g;
    while (p.get_current_square() == square("e7"))
    {
      p.tick(delta_t(0.1), g);
      ++n_ticks;
      assert(n_ticks < 1000);
    }
    assert(p.get_current_square() == square("e5"));
  }
  // A queen for the lhs player can move up
  {
    piece p(
      chess_color::white,
      piece_type::queen,
      square("h5")
    );
    p.add_action(piece_action(chess_color::white, piece_type::queen, piece_action_type::move, square("h5"), square("g5")));
    assert(!p.get_actions().empty());
    int n_ticks{0};
    game g = create_game_with_starting_position(starting_position_type::before_scholars_mate);
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
  // a pawn can promote
  {
    piece p(
      chess_color::white,
      piece_type::pawn,
      square("a8")
    );
    p.add_action(
      piece_action(
        chess_color::white,
        piece_type::queen,
        piece_action_type::promote_to_queen,
        square("a8"),
        square("a8")
      )
    );
    assert(!p.get_actions().empty());
    game g;
    assert(count_piece_actions(p) == 1);
    p.tick(delta_t(0.1), g);
    assert(p.get_type() == piece_type::queen);
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
    assert(p.get_current_square() == square("e1"));
    p.add_action(
      piece_action(
        chess_color::white,
        piece_type::king,
        piece_action_type::move,
        square("e1"),
        square("e2")
      )
    );
    assert(has_actions(p));
    game g(get_pieces_kings_only());
    p.tick(delta_t(0.1), g);
    assert(has_actions(p));
    for (int i{0}; i != 10; ++i)
    {
      p.tick(delta_t(0.1), g);
    }
    assert(!has_actions(p));
    const auto final_square{p.get_current_square()};
    const auto expected_square{square("e2")};
    assert(final_square == expected_square);
  }
  // A pieces can attack
  {
    piece p{get_test_white_king()}; // A white king
    assert(p.get_type() == piece_type::king);
    assert(p.get_current_square() == square("e1"));
    p.add_action(piece_action(chess_color::white, piece_type::king, piece_action_type::attack, square("e2"), square("e3")));
    assert(has_actions(p));
    game g(get_pieces_kings_only());
    p.tick(delta_t(1.0), g);
  }
  // A piece stops attacking when the attacked piece moves away
  {
    game g(get_pieces_queen_endgame());
    piece& white_queen{get_piece_at(g, square("d1"))};
    piece& black_queen{get_piece_at(g, square("d8"))};
    white_queen.add_action(piece_action(chess_color::white, piece_type::queen, piece_action_type::attack, square("d1"), square("d8")));
    assert(has_actions(white_queen));
    black_queen.add_action(piece_action(chess_color::black, piece_type::queen, piece_action_type::move, square("d8"), square("a8")));
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
    assert(p.get_current_square() == square("c3"));
    p.add_action(piece_action(chess_color::white, piece_type::knight, piece_action_type::move, square("c3"), square("e4")));
    assert(has_actions(p));
    game g(get_pieces_kings_only());
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
    assert(p.get_current_square() == square("e4"));
    assert(get_occupied_square(p) == square("e4"));
  }
  // operator<<
  {
    std::stringstream s;
    s << get_test_white_king();
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

void piece::tick(
  const delta_t& dt,
  game& g
)
{
  if (m_actions.empty())
  {
    m_in_game_time += dt;
    return;
  }
  const auto action_type{m_actions[0].get_action_type()};
  if (!has_actions(m_action_history)
    || get_last_action(m_action_history) != m_actions[0]
  )
  {
    m_action_history.add_action(m_in_game_time, m_actions[0]);
  }
  assert(get_last_action(m_action_history) == m_actions[0]);

  switch(action_type)
  {
    case piece_action_type::move:
      m_has_moved = true; // Whatever happens, this piece has tried to move
      tick_move(*this, dt, g);
      break;
    case piece_action_type::attack:
      tick_attack(*this, dt, g);
      break;
    case piece_action_type::attack_en_passant:
      tick_attack_en_passant(*this, dt, g);
      break;
    case piece_action_type::unselect:
      remove_first(m_actions);
      break;
    case piece_action_type::select:
      remove_first(m_actions);
      break;
    case piece_action_type::castle_kingside:
      tick_castle_kingside(*this, dt, g);
      break;
    case piece_action_type::castle_queenside:
      tick_castle_queenside(*this, dt, g);
      break;
    case piece_action_type::promote_to_knight:
    case piece_action_type::promote_to_bishop:
    case piece_action_type::promote_to_rook:
    case piece_action_type::promote_to_queen:
    default:
    {
      const auto first_action{m_actions[0]};
      assert(
           first_action.get_action_type() == piece_action_type::promote_to_knight
        || first_action.get_action_type() == piece_action_type::promote_to_bishop
        || first_action.get_action_type() == piece_action_type::promote_to_rook
        || first_action.get_action_type() == piece_action_type::promote_to_queen
      );
      assert(this->get_type() == piece_type::pawn);
      add_message(message_type::done);
      m_type = first_action.get_piece_type();
      remove_first(m_actions);
    }
  }

  m_in_game_time += dt;
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

  if (!can_do_attack(g, p, first_action.get_to(), p.get_color()))
  {
    p.add_message(message_type::cannot);
    remove_first(p.get_actions());
    return;
  }
  assert(is_piece_at(g, first_action.get_to()));
  piece& target{get_piece_at(g, first_action.get_to())};

  assert(p.get_color() != target.get_color());
  const double damage_per_chess_move{1.0};
  const auto damage{damage_per_chess_move * dt.get()};
  target.receive_damage(damage);
  #ifdef DO_NOT_CAPTURE_HERE_ANYMORE_157
  // Capture the piece if destroyed
  if (is_dead(target) && is_first_attacker(p, first_action.get_to(), g))
  {
    p.increase_kill_count();
    p.set_current_square(first_action.get_to()); // Capture
    remove_first(p.get_actions());
  }
  #endif // DO_NOT_CAPTURE_HERE_ANYMORE_157
}

void tick_attack_en_passant(
  piece& p,
  const delta_t& dt,
  game& g
)
{
  assert(!p.get_actions().empty());
  const auto& first_action{p.get_actions()[0]};
  assert(first_action.get_action_type() == piece_action_type::attack_en_passant);
  if (!can_do_en_passant(g, p, first_action.get_to(), p.get_color()))
  {
    p.add_message(message_type::cannot);
    remove_first(p.get_actions());
    return;
  }
  const auto attack_square{
    get_en_passant_capture_square(first_action.get_from(), first_action.get_to())
  };
  assert(is_piece_at(g, attack_square));
  piece& target{get_piece_at(g, attack_square)};

  assert(p.get_color() != target.get_color());
  const double damage_per_chess_move{1.0};
  const auto damage{damage_per_chess_move * dt.get()};
  target.receive_damage(damage);
  #ifdef DO_NOT_CAPTURE_HERE_ANYMORE_157
  // Capture the piece if destroyed
  if (is_dead(target))
  {
    p.increase_kill_count();
    p.set_current_square(first_action.get_to()); // Capture
    remove_first(p.get_actions());
  }
  #endif // DO_NOT_CAPTURE_HERE_ANYMORE_157
}
void tick_castle_kingside(
  piece& p,
  const delta_t& dt,
  game& g
)
{
  assert(!p.get_actions().empty());
  const auto& first_action{p.get_actions()[0]};
  assert(first_action.get_action_type() == piece_action_type::castle_kingside);

  // Increase the progress of the action
  const double new_progress{
    p.get_current_action_progress().get() + dt.get()
  };
  assert(new_progress >= 0.0);

  // Are we done with the action?
  if (new_progress >= 1.0)
  {
    // Can this piece now be captured by en passant?
    // Or should it be found in a piece's history ..?

    // The whole goal of the operation
    assert(p.get_current_square() == first_action.get_to());
    p.set_current_action_progress(delta_t(0.0));
    remove_first(p.get_actions());
    if (p.get_actions().empty())
    {
      p.add_message(message_type::done);
    }
    return;
  }

  p.set_current_action_progress(p.get_current_action_progress() + dt);
  const double f{std::min(1.0, new_progress)}; // The fraction of the action done
  assert(f >= 0.0);
  assert(f <= 1.0);

  // Allow for duplicate squares
  const auto occupied_squares{get_occupied_squares(g)};
  const bool is_target_occupied{is_occupied(first_action.get_to(), occupied_squares)};
  const bool is_focal_piece_at_target{p.get_current_square() == first_action.get_to()};

  if (is_target_occupied)
  {
    if (is_focal_piece_at_target)
    {
      // Moving the last half
      assert(f > 0.5);
    }
    else
    {
      // Too bad, need to go back
      p.get_actions().clear();
      p.add_action(
        piece_action(
          get_piece_at(g, first_action.get_from()).get_color(),
          get_piece_at(g, first_action.get_from()).get_type(),
          piece_action_type::move,
          first_action.get_to(), // Reverse
          first_action.get_from()
        )
      );
      p.set_current_action_progress(delta_t(1.0) - p.get_current_action_progress()); // Keep progress
      p.add_message(message_type::cannot);
      return;
    }
  }
  else
  {
    assert(!is_target_occupied);
    if (f >= 0.5)
    {
      // If over halfway, occupy target
      assert(!is_occupied(first_action.get_to(), get_unique_occupied_squares(g)));
      p.set_current_square(first_action.get_to());
      // Maybe cannot check, as p is not fully updated in game?
      // assert(is_occupied(first_action.get_to(), get_occupied_squares(g)));
    }
  }
}

void tick_castle_queenside(
  piece& p,
  const delta_t& dt,
  game& g
)
{
  assert(!p.get_actions().empty());
  const auto& first_action{p.get_actions()[0]};
  assert(first_action.get_action_type() == piece_action_type::castle_queenside);

  // Increase the progress of the action
  const double new_progress{
    p.get_current_action_progress().get() + dt.get()
  };
  assert(new_progress >= 0.0);

  // Are we done with the action?
  if (new_progress >= 1.0)
  {
    // Can this piece now be captured by en passant?
    // Or should it be found in a piece's history ..?

    // The whole goal of the operation
    assert(p.get_current_square() == first_action.get_to());
    p.set_current_action_progress(delta_t(0.0));
    remove_first(p.get_actions());
    if (p.get_actions().empty())
    {
      p.add_message(message_type::done);
    }
    return;
  }

  p.set_current_action_progress(p.get_current_action_progress() + dt);
  const double f{std::min(1.0, new_progress)}; // The fraction of the action done
  assert(f >= 0.0);
  assert(f <= 1.0);

  // Allow for duplicate squares
  const auto occupied_squares{get_occupied_squares(g)};
  const bool is_target_occupied{is_occupied(first_action.get_to(), occupied_squares)};
  const bool is_focal_piece_at_target{p.get_current_square() == first_action.get_to()};

  if (is_target_occupied)
  {
    if (is_focal_piece_at_target)
    {
      // Moving the last half
      assert(f > 0.5);
    }
    else
    {
      // Too bad, need to go back
      p.get_actions().clear();
      p.add_action(
        piece_action(
          get_piece_at(g, first_action.get_from()).get_color(),
          get_piece_at(g, first_action.get_from()).get_type(),
          piece_action_type::move,
          first_action.get_to(), // Reverse
          first_action.get_from()
        )
      );
      p.set_current_action_progress(delta_t(1.0) - p.get_current_action_progress()); // Keep progress
      p.add_message(message_type::cannot);
      return;
    }
  }
  else
  {
    assert(!is_target_occupied);
    if (f >= 0.5)
    {
      // If over halfway, occupy target
      assert(!is_occupied(first_action.get_to(), get_unique_occupied_squares(g)));
      p.set_current_square(first_action.get_to());
      // Maybe cannot check, as p is not fully updated in game?
      // assert(is_occupied(first_action.get_to(), get_occupied_squares(g)));
    }
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
  const double new_progress{
    p.get_current_action_progress().get() + dt.get()
  };
  assert(new_progress >= 0.0);

  // Are we done with the action?
  if (new_progress >= 1.0)
  {

    // The whole goal of the operation
    assert(p.get_current_square() == first_action.get_to());
    p.set_current_action_progress(delta_t(0.0));
    remove_first(p.get_actions());
    if (p.get_actions().empty())
    {
      p.add_message(message_type::done);
    }
    return;
  }

  p.set_current_action_progress(p.get_current_action_progress() + dt);
  const double f{std::min(1.0, new_progress)}; // The fraction of the action done
  assert(f >= 0.0);
  assert(f <= 1.0);

  // Allow for duplicate squares
  const auto occupied_squares{get_occupied_squares(g)};
  const bool is_target_occupied{is_occupied(first_action.get_to(), occupied_squares)};
  const bool is_focal_piece_at_target{p.get_current_square() == first_action.get_to()};

  if (is_target_occupied)
  {
    if (is_focal_piece_at_target)
    {
      // Moving the last half
      assert(f >= 0.5);
    }
    else
    {
      // Too bad, need to go back
      p.get_actions().clear();
      p.add_action(
        piece_action(
          get_piece_at(g, first_action.get_from()).get_color(),
          get_piece_at(g, first_action.get_from()).get_type(),
          piece_action_type::move,
          first_action.get_to(), // Reverse
          first_action.get_from()
        )
      );
      p.set_current_action_progress(delta_t(1.0) - p.get_current_action_progress()); // Keep progress
      p.add_message(message_type::cannot);
      return;
    }
  }
  else
  {
    assert(!is_target_occupied);
    if (f >= 0.5)
    {
      // If over halfway, occupy target
      assert(!is_occupied(first_action.get_to(), get_unique_occupied_squares(g)));
      p.set_current_square(first_action.get_to());
      // Maybe cannot check, as p is not fully updated in game?
      // assert(is_occupied(first_action.get_to(), get_occupied_squares(g)));
    }
  }
}

char to_fen_char(const piece& p) noexcept
{
  char c = '\0';
  switch (p.get_type())
  {
    case piece_type::bishop: c = 'B'; break;
    case piece_type::knight: c = 'N'; break;
    case piece_type::king: c = 'K'; break;
    case piece_type::pawn: c = 'P'; break;
    case piece_type::queen: c = 'Q'; break;
    case piece_type::rook:
    default:
      assert(p.get_type() == piece_type::rook);
      c = 'R';
      break;
  }
  if (p.get_color() == chess_color::black)
  {
    c = std::tolower(c);
  }
  return c;
}

/*
void toggle_select(piece& p) noexcept
{
  p.set_selected(!p.is_selected());
}

void unselect(piece& p) noexcept
{
  p.set_selected(false);
}
 */

bool operator==(const piece& lhs, const piece& rhs) noexcept
{
  return lhs.get_type() == rhs.get_type()
    && lhs.get_color() == rhs.get_color()
    && lhs.get_health() == rhs.get_health()
    && lhs.get_actions() == rhs.get_actions()
    //&& lhs.is_selected() == rhs.is_selected()
    && lhs.get_current_square() == rhs.get_current_square()
    && lhs.get_max_health() == rhs.get_max_health()
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
    << p.get_current_square()
    << p.get_health()
    //<< p.is_selected()
    << p.get_kill_count()
    << p.get_max_health()
    << to_str(p.get_messages())
    << p.get_type()
  ;
  std::vector<square> m_target_square;
  return os;
}
