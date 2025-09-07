#include "game.h"

//#include "game_options.h"
#include "piece_actions.h"
#include "square.h"
#include "pieces.h"
#include "chess_color.h"
#include <cassert>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <iostream>
//#include <random>

#ifdef GAME_CONTROLLER_H
#error 'game' must know nothing about 'game_controller'
#error Instead, it is the other way around...
#endif

#ifdef LOBBY_OPTIONS_H
#error 'game' must know nothing about 'lobby_options'
#error Instead, all player sides (e.g. LHS) need to be converted to colors (e.g. white)
#endif

#ifdef SIDE_H
#error 'game' must know nothing about 'side'
#error This is managed by 'lobby_options'.
#endif


game::game(const std::vector<piece>& pieces)
  : m_pieces{pieces},
    m_in_game_time{0.0}
{
  check_if_there_is_a_winner();
}

bool can_castle_kingside(const piece& p, const game& g) noexcept
{
  if (p.get_type() != piece_type::king) return false;
  const chess_color player_color{p.get_color()};
  const chess_color enemy_color{get_other_color(player_color)};
  assert(p.get_type() == piece_type::king);
  const auto king_square{p.get_current_square()};
  if (king_square != get_initial_king_square(player_color)) return false;
  if (has_moved(p)) return false;
  assert(to_str(king_square) == "e1" || to_str(king_square) == "e8");
  const int number_index{king_square.get_x()};
  assert(number_index == 0 || number_index == 7); // For 1 or 8
  const int letter_index{king_square.get_y()};
  assert(letter_index == 4); // For e
  if (is_square_attacked(g.get_pieces(), king_square, enemy_color)) return false;
  const auto rook_square(square(king_square.get_x(), 7));
  assert(to_str(rook_square) == "h1" || to_str(rook_square) == "h8");
  if (!is_piece_at(g, rook_square)) return false;
  const auto rook{get_piece_at(g, rook_square)};
  if (rook.get_type() != piece_type::rook) return false;
  if (has_moved(rook)) return false;

  const auto f_pawn_square(square(king_square.get_x(), 5));
  assert(to_str(f_pawn_square) == "f1" || to_str(f_pawn_square) == "f8");
  if (!is_empty(g, f_pawn_square)) return false;
  if (is_square_attacked(g.get_pieces(), f_pawn_square, enemy_color)) return false;
  const auto g_pawn_square(square(king_square.get_x(), 6));
  assert(to_str(g_pawn_square) == "g1" || to_str(g_pawn_square) == "g8");
  if (!is_empty(g, g_pawn_square)) return false;
  if (is_square_attacked(g.get_pieces(), g_pawn_square, enemy_color)) return false;

  // Do not check for moving through check or into check,
  // this would give recursions
  return true;
}

bool can_castle_queenside(const piece& p, const game& g) noexcept
{
  if (p.get_type() != piece_type::king) return false;
  assert(p.get_type() == piece_type::king);
  const chess_color player_color{p.get_color()};
  const chess_color enemy_color{get_other_color(player_color)};
  const auto king_square{p.get_current_square()};
  if (king_square != get_initial_king_square(player_color)) return false;
  assert(to_str(king_square) == "e1" || to_str(king_square) == "e8");
  if (has_moved(p)) return false;
  if (is_square_attacked(g.get_pieces(), king_square, enemy_color)) return false;
  const auto rook_square(square(king_square.get_x(), 0));
  assert(to_str(rook_square) == "a1" || to_str(rook_square) == "a8");
  if (!is_piece_at(g, rook_square)) return false;
  const auto rook{get_piece_at(g, rook_square)};
  if (rook.get_type() != piece_type::rook) return false;
  if (has_moved(rook)) return false;

  const auto b_pawn_square(square(king_square.get_x(), 1));
  assert(to_str(b_pawn_square) == "b1" || to_str(b_pawn_square) == "b8");
  if (!is_empty(g, b_pawn_square)) return false;
  if (is_square_attacked(g.get_pieces(), b_pawn_square, enemy_color)) return false;
  const auto c_pawn_square(square(king_square.get_x(), 2));
  assert(to_str(c_pawn_square) == "c1" || to_str(c_pawn_square) == "c8");
  if (!is_empty(g, c_pawn_square)) return false;
  if (is_square_attacked(g.get_pieces(), c_pawn_square, enemy_color)) return false;
  const auto d_pawn_square(square(king_square.get_x(), 3));
  assert(to_str(d_pawn_square) == "d1" || to_str(d_pawn_square) == "d8");
  if (!is_empty(g, d_pawn_square)) return false;
  if (is_square_attacked(g.get_pieces(), d_pawn_square, enemy_color)) return false;

  // Do not check for moving through check or into check,
  // this would give recursions
  return true;
}

bool can_do_attack(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const chess_color player_color
)
{
  assert(player_color == selected_piece.get_color());
  // Is it theoretically possible, e.g. on an empty board?
  if (
    !can_attack_on_empty_board(
      player_color,
      selected_piece.get_type(),
      selected_piece.get_current_square(),
      cursor_square
    )
  )
  {
    return false;
  }
  // Is it possible in this situation?
  assert(is_piece_at(g, selected_piece.get_current_square()));
  assert(get_piece_at(g, selected_piece.get_current_square()).get_color()
    == player_color
  );
  if (!is_piece_at(g, cursor_square)) return false;
  if(get_piece_at(g, cursor_square).get_color() == player_color)
  {
    return false;
  }
  return is_empty_between(
    g,
    selected_piece.get_current_square(),
    cursor_square
  );

}

bool can_do_castle_kingside(
  const game& g,
  const piece& selected_piece,
  const chess_color player_color
)
{
  assert(player_color == selected_piece.get_color());
  const square king_square{get_initial_king_square(player_color)};
  if (selected_piece.get_current_square() != king_square) return false;
  return can_castle_kingside(get_piece_at(g, king_square), g);
}

bool can_do_castle_queenside(
  const game& g,
  const piece& selected_piece,
  const chess_color player_color
)
{
  assert(player_color == selected_piece.get_color());
  const square king_square{get_initial_king_square(player_color)};
  if (selected_piece.get_current_square() != king_square) return false;
  return can_castle_queenside(get_piece_at(g, king_square), g);
}

bool can_do_en_passant(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const chess_color player_color
)
{
  assert(player_color == selected_piece.get_color());
  const auto player_square{selected_piece.get_current_square()};
  assert(is_piece_at(g, player_square));
  assert(get_piece_at(g, player_square).get_color() == player_color);
  if (is_piece_at(g, cursor_square)) return false;

  if (!are_en_passant_capture_squares(
      player_square,
      cursor_square,
      player_color
    )
  )
  {
    return false;
  }

  const auto en_passant_capture_square{
    get_en_passant_capture_square(
      player_square,
      cursor_square
    )
  };
  if (!is_piece_at(g, en_passant_capture_square)) return false;

  if (get_piece_at(g, en_passant_capture_square).get_color() == player_color) return false;

  return true;
}

bool can_do_move(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const chess_color player_color
)
{
  assert(player_color == selected_piece.get_color());
  // Is it theoretically possible, e.g. on an empty board?
  if (
    !can_move_on_empty_board(
      player_color,
      selected_piece.get_type(),
      selected_piece.get_current_square(),
      cursor_square
    )
  ) return false;
  // Is it possible in this situation?
  assert(is_piece_at(g, selected_piece.get_current_square()));
  assert(get_piece_at(g, selected_piece.get_current_square()).get_color()
    == player_color
  );
  if (is_piece_at(g, cursor_square)) return false;
  return is_empty_between(
    g,
    selected_piece.get_current_square(),
    cursor_square
  );

}

bool can_do_promote(
  const piece& selected_piece,
  const chess_color player_color
)
{
  assert(player_color == selected_piece.get_color());
  return can_promote(selected_piece);
}

void game::check_all_occupied_squares_are_unique() const
{
  const auto squares{get_occupied_squares(*this)};
  assert(are_all_unique(squares));
}


void game::check_game_and_pieces_agree_on_the_time() const
{
  for (const piece& p: m_pieces)
  {
    assert(p.get_in_game_time() == m_in_game_time);
  }
}

void game::check_if_there_is_a_winner()
{
  if (m_winner.has_value())
  {
    // We already have a winner
    return;
  }
  // Do the non-chess things first
  if (!has_king(m_pieces, chess_color::white))
  {
    m_winner = chess_color::black;
  }
  else if (!has_king(m_pieces, chess_color::black))
  {
    m_winner = chess_color::white;
  }
  else if (is_checkmate(m_pieces, chess_color::white))
  {
    m_winner = chess_color::black;
  }
  else if (is_checkmate(m_pieces, chess_color::black))
  {
    m_winner = chess_color::white;
  }
}

void clear_piece_messages(game& g) noexcept
{
  for (auto& p: g.get_pieces()) p.clear_messages();
}

action_history collect_action_history(const game& g)
{
  return collect_action_history(g.get_pieces());
}

std::vector<piece_action> collect_all_piece_actions(const game& g)
{
  // 1. Collect all the simple actions,
  //    such as movement and attacks
  std::vector<piece_action> actions;
  for (const auto& p: g.get_pieces())
  {
    const auto piece_actions{
      collect_all_piece_actions(g, p)
    };
    std::copy(
      std::begin(piece_actions),
      std::end(piece_actions),
      std::back_inserter(actions)
    );
  }
  // 2. collect all attacked squares
  std::vector<std::pair<square, chess_color>> attacked_squares{
    collect_attacked_squares(actions)
  };

  // 3. Prevent king moving into or through (by castling) check
  const auto new_end{
    std::remove_if(
      std::begin(actions),
      std::end(actions),
      [attacked_squares](const piece_action& action)
      {
        if (action.get_action_type() == piece_action_type::move)
        {
          if (action.get_piece_type() == piece_type::king)
          {
            // King cannot move into check
            const chess_color enemy_color{get_other_color(action.get_color())};
            return is_square_attacked_by(attacked_squares, action.get_to(), enemy_color);
          }
        }
        else if (action.get_action_type() == piece_action_type::castle_kingside)
        {
          const square king_square{action.get_from()};
          const chess_color enemy_color{get_other_color(action.get_color())};
          const square f_pawn_square{square(king_square.get_x(), 5)};
          const square g_pawn_square{square(king_square.get_x(), 6)};
          return is_square_attacked_by(attacked_squares, f_pawn_square, enemy_color)
            || is_square_attacked_by(attacked_squares, g_pawn_square, enemy_color)
          ;
        }
        else if (action.get_action_type() == piece_action_type::castle_queenside)
        {
          const square king_square{action.get_from()};
          const chess_color enemy_color{get_other_color(action.get_color())};
          const square b_pawn_square{square(king_square.get_x(), 1)};
          const square c_pawn_square{square(king_square.get_x(), 2)};
          const square d_pawn_square{square(king_square.get_x(), 3)};
          return is_square_attacked_by(attacked_squares, b_pawn_square, enemy_color)
            || is_square_attacked_by(attacked_squares, c_pawn_square, enemy_color)
            || is_square_attacked_by(attacked_squares, d_pawn_square, enemy_color)
          ;
        }
        return false;
      }
    )
  };
  actions.erase(new_end, std::end(actions));

  // Prevent opening up a pin

  // If king is attacked, only moves that break check are possible

  return actions;
}

std::vector<piece_action> collect_all_piece_actions(
  const game& g,
  const chess_color player_color)
{
  std::vector<piece_action> actions;
  for (const auto& p: g.get_pieces())
  {
    if (p.get_color() != player_color) continue;
    const auto piece_actions{
      collect_all_piece_actions(g, p)
    };
    std::copy(
      std::begin(piece_actions),
      std::end(piece_actions),
      std::back_inserter(actions)
    );
  }
  return actions;
}

std::vector<piece_action> collect_all_piece_actions(
  const game& g,
  const piece& p
)
{
  switch (p.get_type())
  {
    case piece_type::bishop:
      return collect_all_bishop_actions(g, p);
    case piece_type::king:
      return collect_all_king_actions(g, p);
    case piece_type::knight:
      return collect_all_knight_actions(g, p);
    case piece_type::pawn:
      return collect_all_pawn_actions(g, p);
    case piece_type::queen:
      return collect_all_queen_actions(g, p);
    case piece_type::rook:
    default:
      assert(p.get_type() == piece_type::rook);
      return collect_all_rook_actions(g, p);
  }
}

std::vector<piece_action> collect_all_bishop_actions(
  const game& g,
  const piece& p
)
{
  std::vector<piece_action> actions;
  const auto type{p.get_type()};
  assert(type == piece_type::bishop);
  const auto color{p.get_color()};
  const auto& from{p.get_current_square()};
  const auto enemy_color{get_other_color(color)};
  const std::vector<std::vector<square>> targetses{
    collect_all_bishop_target_squares(from)
  };
  for (const std::vector<square>& targets: targetses)
  {
    for (const square& to: targets)
    {
      if (is_empty(g, to))
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::move, from, to
          )
        );
      }
      else if (get_piece_at(g, to).get_color() == enemy_color)
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::attack, from, to
          )
        );
        // Nothing to be done behind an enemy piece
        break;
      }
      else
      {
        // Cannot capture own piece
        break;
      }
    }
  }
  return actions;
}

std::vector<piece_action> collect_all_king_actions(
  const game& g,
  const piece& p
)
{
  std::vector<piece_action> actions;
  const auto type{p.get_type()};
  assert(type == piece_type::king);
  const auto color{p.get_color()};
  const auto& from{p.get_current_square()};
  const std::vector<square> targets{
    collect_all_king_target_squares(from)
  };
  for (const square& to: targets)
  {
    if (is_empty(g, to))
    {
      actions.push_back(
        piece_action(
          color, type, piece_action_type::move, from, to
        )
      );
    }
    else
    {
      const auto enemy_color{get_other_color(color)};
      assert(is_piece_at(g, to));
      if (get_piece_at(g, to).get_color() == enemy_color)
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::attack, from, to
          )
        );
      }
    }
  }
  if (can_castle_kingside(p, g))
  {
    const square to{
      p.get_current_square().get_x(),
      p.get_current_square().get_y() + 2,
    };
    actions.push_back(
      piece_action(
        color, type, piece_action_type::castle_kingside, from, to
      )
    );
  }
  if (can_castle_queenside(p, g))
  {
    const square to{
      p.get_current_square().get_x(),
      p.get_current_square().get_y() - 2,
    };
    actions.push_back(
      piece_action(
        color, type, piece_action_type::castle_queenside, from, to
      )
    );
  }
  return actions;
}

std::vector<piece_action> collect_all_knight_actions(
  const game& g,
  const piece& p
)
{
  std::vector<piece_action> actions;
  const auto type{p.get_type()};
  assert(type == piece_type::knight);
  const auto color{p.get_color()};
  const auto& from{p.get_current_square()};
  const std::vector<square> targets{
    collect_all_knight_target_squares(from)
  };
  for (const square& to: targets)
  {
    if (is_empty(g, to))
    {
      actions.push_back(
        piece_action(
          color, type, piece_action_type::move, from, to
        )
      );
    }
    else
    {
      const auto enemy_color{get_other_color(color)};
      assert(is_piece_at(g, to));
      if (get_piece_at(g, to).get_color() == enemy_color)
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::attack, from, to
          )
        );
      }
    }
  }
  return actions;
}

std::vector<piece_action> collect_all_pawn_actions(
  const game& g,
  const piece& p
)
{
  std::vector<piece_action> actions;
  const auto type{p.get_type()};
  assert(type == piece_type::pawn);
  const auto move_actions{collect_all_pawn_move_actions(g, p)};
  const auto attack_actions{collect_all_pawn_attack_actions(g, p)};
  const auto en_passant_actions{collect_all_pawn_en_passant_actions(g, p)};
  return concatenate(
    concatenate(
      move_actions,
      attack_actions
    ),
    en_passant_actions
  );
}

std::vector<piece_action> collect_all_pawn_attack_actions(
  const game& g,
  const piece& p
)
{
  std::vector<piece_action> actions;
  const auto type{p.get_type()};
  assert(type == piece_type::pawn);
  const auto& s{p.get_current_square()};
  const auto x{s.get_x()};
  const auto y{s.get_y()};
  const auto color{p.get_color()};
  const auto& from{p.get_current_square()};
  if (color == chess_color::black)
  {
    // Attack
    if (x != 0)
    {
      const auto enemy_color{get_other_color(color)};
      if (y > 0)
      {
        const square enemy_square{square(x - 1, y - 1)};
        if (is_piece_at(g, enemy_square) && get_piece_at(g, enemy_square).get_color() == enemy_color)
        {
          actions.push_back(piece_action(color, type, piece_action_type::attack, from, enemy_square));
        }
      }
      if (y < 7)
      {
        const square enemy_square{square(x - 1, y + 1)};
        if (is_piece_at(g, enemy_square) && get_piece_at(g, enemy_square).get_color() == enemy_color)
        {
          actions.push_back(piece_action(color, type, piece_action_type::attack, from, enemy_square));
        }
      }
    }
  }
  else
  {
    assert(color == chess_color::white);
    assert(get_rank(s) != 1);
    // Attack
    if (x != 7)
    {
      const auto enemy_color{get_other_color(color)};
      if (y > 0)
      {
        const square enemy_square{square(x + 1, y - 1)};
        if (is_piece_at(g, enemy_square) && get_piece_at(g, enemy_square).get_color() == enemy_color)
        {
          actions.push_back(piece_action(color, type, piece_action_type::attack, from, enemy_square));
        }
      }
      if (y < 7)
      {
        const square enemy_square{square(x + 1, y + 1)};
        if (is_piece_at(g, enemy_square) && get_piece_at(g, enemy_square).get_color() == enemy_color)
        {
          actions.push_back(piece_action(color, type, piece_action_type::attack, from, enemy_square));
        }
      }
    }
  }
  return actions;
}

std::vector<piece_action> collect_all_pawn_en_passant_actions(
  const game& g,
  const piece& p
)
{
  std::vector<piece_action> actions;
  const auto type{p.get_type()};
  assert(type == piece_type::pawn);
  const auto& s{p.get_current_square()};
  const auto x{s.get_x()};
  const auto y{s.get_y()};
  const auto color{p.get_color()};
  const auto& from{p.get_current_square()};
  const auto enemy_color{get_other_color(color)};
  if (color == chess_color::black)
  {
    if (x != 3) return {};
    if (y > 0)
    {
      const square to_square{square(x - 1, y - 1)};
      const square enemy_square{square(x, y - 1)};
      if (is_empty(g, to_square)
        && is_piece_at(g, enemy_square)
        && get_piece_at(g, enemy_square).get_color() == enemy_color
        && has_just_double_moved(get_piece_at(g, enemy_square), g.get_in_game_time())
      )
      {
        actions.push_back(piece_action(color, type, piece_action_type::attack_en_passant, from, to_square));
      }
    }
    if (y < 7)
    {
      const square to_square{square(x - 1, y + 1)};
      const square enemy_square{square(x, y + 1)};
      if (is_empty(g, to_square)
        && is_piece_at(g, enemy_square)
        && get_piece_at(g, enemy_square).get_color() == enemy_color
        && has_just_double_moved(get_piece_at(g, enemy_square), g.get_in_game_time())
      )
      {
        actions.push_back(piece_action(color, type, piece_action_type::attack_en_passant, from, to_square));
      }
    }
  }
  else
  {
    assert(color == chess_color::white);
    if (x != 4) return {};
    if (y > 0)
    {
      const square to_square{square(x + 1, y - 1)};
      const square enemy_square{square(x, y - 1)};
      if (is_empty(g, to_square)
        && is_piece_at(g, enemy_square)
        && get_piece_at(g, enemy_square).get_color() == enemy_color
        && has_just_double_moved(get_piece_at(g, enemy_square), g.get_in_game_time())
      )
      {
        // This code is indeed active, as found out by Virtual Bastard 2025-09-01
        actions.push_back(piece_action(color, type, piece_action_type::attack_en_passant, from, to_square));
      }
    }
    if (y < 7)
    {
      const square to_square{square(x + 1, y + 1)};
      const square enemy_square{square(x, y + 1)};
      if (is_empty(g, to_square)
        && is_piece_at(g, enemy_square)
        && get_piece_at(g, enemy_square).get_color() == enemy_color
        && has_just_double_moved(get_piece_at(g, enemy_square), g.get_in_game_time())
      )
      {
        // This code can be triggered, e.g. by the Virtual Bastard on 2025-09-01
        actions.push_back(piece_action(color, type, piece_action_type::attack_en_passant, from, to_square));
      }
    }
  }
  return actions;
}


std::vector<piece_action> collect_all_pawn_move_actions(
  const game& g,
  const piece& p
)
{
  std::vector<piece_action> actions;
  const auto type{p.get_type()};
  assert(type == piece_type::pawn);
  const auto& s{p.get_current_square()};
  const auto x{s.get_x()};
  const auto y{s.get_y()};
  const auto color{p.get_color()};
  const auto& from{p.get_current_square()};
  if (color == chess_color::black)
  {
    // Move
    if (x == 6)
    {
      // Two forward
      if (is_empty(g, square(4, y)) && is_empty(g, square(5, y)))
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::move, from, square(4, y)
          )
        );
      }
      // One forward
      if (is_empty(g, square(5, y)))
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::move, from, square(5, y)
          )
        );
      }
    }
    else if (x == 0)
    {
      actions.push_back(
        piece_action(color, piece_type::queen, piece_action_type::promote_to_queen, from, from)
      );
      actions.push_back(
        piece_action(color, piece_type::rook, piece_action_type::promote_to_rook, from, from)
      );
      actions.push_back(
        piece_action(color, piece_type::bishop, piece_action_type::promote_to_bishop, from, from)
      );
      actions.push_back(
        piece_action(color, piece_type::knight, piece_action_type::promote_to_knight, from, from)
      );
    }
    else
    {
      assert(x - 1 >= 0);
      if (is_empty(g, square(x - 1, y)))
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::move, from, square(x - 1, y)
          )
        );
      }
    }
  }
  else
  {
    assert(color == chess_color::white);
    assert(get_rank(s) != 1);
    // Move
    if (get_rank(s) == 2)
    {
      // Two forward
      if (is_empty(g, square(2, y)) && is_empty(g, square(3, y)))
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::move, from, square(3, y)
          )
        );
      }
      // One forward
      if (is_empty(g, square(2, y)))
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::move, from, square(2, y)
          )
        );
      }
    }
    else if (get_rank(s) == 8)
    {
      actions.push_back(
        piece_action(color, piece_type::queen, piece_action_type::promote_to_queen, from, from)
      );
      actions.push_back(
        piece_action(color, piece_type::rook, piece_action_type::promote_to_rook, from, from)
      );
      actions.push_back(
        piece_action(color, piece_type::bishop, piece_action_type::promote_to_bishop, from, from)
      );
      actions.push_back(

        piece_action(color, piece_type::knight, piece_action_type::promote_to_knight, from, from)
      );
    }
    else
    {
      assert(x + 1 < 8);
      if (is_empty(g, square(x + 1, y)))
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::move, from, square(x + 1, y)
          )
        );
      }
    }
  }
  return actions;
}

std::vector<piece_action> collect_all_queen_actions(
  const game& g,
  const piece& p
)
{
  std::vector<piece_action> actions;
  const auto type{p.get_type()};
  assert(type == piece_type::queen);
  const auto color{p.get_color()};
  const auto& from{p.get_current_square()};
  const auto enemy_color{get_other_color(color)};
  const std::vector<std::vector<square>> targetses{
    collect_all_queen_target_squares(from)
  };
  for (const std::vector<square>& targets: targetses)
  {
    for (const square& to: targets)
    {
      if (is_empty(g, to))
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::move, from, to
          )
        );
      }
      else if (get_piece_at(g, to).get_color() == enemy_color)
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::attack, from, to
          )
        );
        // Nothing to be done behind an enemy piece
        break;
      }
      else
      {
        // Cannot capture own piece
        break;
      }
    }
  }
  return actions;
}

std::vector<piece_action> collect_all_rook_actions(
  const game& g,
  const piece& p
)
{
  std::vector<piece_action> actions;
  const auto type{p.get_type()};
  assert(type == piece_type::rook);
  const auto color{p.get_color()};
  const auto& from{p.get_current_square()};
  const auto enemy_color{get_other_color(color)};
  const std::vector<std::vector<square>> targetses{
    collect_all_rook_target_squares(from)
  };
  for (const std::vector<square>& targets: targetses)
  {
    for (const square& to: targets)
    {
      if (is_empty(g, to))
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::move, from, to
          )
        );
      }
      else if (get_piece_at(g, to).get_color() == enemy_color)
      {
        actions.push_back(
          piece_action(
            color, type, piece_action_type::attack, from, to
          )
        );
        // Nothing to be done behind an enemy piece
        break;
      }
      else
      {
        // Cannot capture own piece
        break;
      }
    }
  }
  return actions;
}

int count_piece_actions(const game& g)
{
  return count_piece_actions(g, chess_color::white)
    + count_piece_actions(g, chess_color::black)
  ;
}

int count_piece_actions(
  const game& g,
  const chess_color player
)
{
  return count_piece_actions(g.get_pieces(), player);
}

std::vector<piece> find_pieces(
  const game& g,
  const piece_type type,
  const chess_color color
)
{
  std::vector<piece> pieces;
  std::copy_if(
    std::begin(g.get_pieces()),
    std::end(g.get_pieces()),
    std::back_inserter(pieces),
    [type, color](const auto& piece)
    {
      return piece.get_color() == color
        && piece.get_type() == type;
    }
  );
  return pieces;
}

const piece& get_closest_piece_to(
  const game& g,
  const game_coordinate& coordinat
)
{
  return g.get_pieces()[get_index_of_closest_piece_to(g, coordinat)];
}

piece& get_closest_piece_to(
  game& g,
  const game_coordinate& coordinat
)
{
  return g.get_pieces()[get_index_of_closest_piece_to(g, coordinat)];
}

game create_game_from_fen_string(const fen_string& s) noexcept
{
  const game g(create_pieces_from_fen_string(s));
  assert(!g.get_pieces().empty());
  return g;
}


game create_game_with_standard_starting_position() noexcept
{
  return game(get_standard_starting_pieces());
}

game create_game_with_starting_position(
  starting_position_type t,
  const race lhs_race,
  const race rhs_race
) noexcept
{
  return game(get_starting_pieces(t, lhs_race, rhs_race));
}


int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinate& coordinat
)
{
  const std::vector<double> distances{
    calc_distances(g.get_pieces(), coordinat)
  };
  const auto iter = std::min_element(std::begin(distances), std::end(distances));
  const auto index = std::distance(std::begin(distances), iter);
  return index;
}

std::vector<square> get_unique_occupied_squares(const game& g) noexcept
{
  return get_unique_occupied_squares(get_pieces(g));
}

std::vector<square> get_occupied_squares(const game& g) noexcept
{
  return get_occupied_squares(get_pieces(g));
}

/*
const game_options& get_options(const game&)
{
  return game_options::get();
}
*/

std::vector<piece>& get_pieces(game& g) noexcept
{
  return g.get_pieces();
}

const std::vector<piece>& get_pieces(const game& g) noexcept
{
  return g.get_pieces();
}

const piece& get_piece_with_id(const game& g, const piece_id& id)
{
  assert(has_piece_with_id(g, id));
  for (const auto& p: g.get_pieces())
  {
    if (p.get_id() == id) return p;
  }
  assert(!"Should never get here");
}

piece& get_piece_with_id(game& g, const piece_id& id)
{
  assert(has_piece_with_id(g, id));
  for (auto& p: g.get_pieces())
  {
    if (p.get_id() == id) return p;
  }
  assert(!"Should never get here");
}

std::vector<message> collect_messages(const game& g) noexcept
{
  const auto& pieces{g.get_pieces()};
  std::vector<message> effects;
  for (const auto& piece: pieces)
  {
    const auto& es{piece.get_messages()};
    std::transform(
      std::begin(es),
      std::end(es),
      std::back_inserter(effects),
      [piece](const message_type type)
      {
        return message(
          type,
          piece.get_color(),
          piece.get_type()
        );
      }
    );
  }
  return effects;
}

std::vector<message_type> collect_message_types(const game& g) noexcept
{
  const auto& pieces{g.get_pieces()};
  std::vector<message_type> v;
  for (const auto& piece: pieces)
  {
    const auto& es{piece.get_messages()};
    std::copy(
      std::begin(es),
      std::end(es),
      std::back_inserter(v)
    );
  }
  return v;
}
const piece& get_piece_at(const game& g, const square& coordinat)
{
  return get_piece_at(g.get_pieces(), coordinat);
}

piece& get_piece_at(game& g, const square& coordinat)
{
  assert(is_piece_at(g, coordinat));
  return get_piece_at(g.get_pieces(), coordinat);
}

piece& get_piece_at(game& g, const std::string& square_str)
{
  return get_piece_at(g, square(square_str));
}

const piece& get_piece_at(const game& g, const std::string& coordinat_str)
{
  return get_piece_at(g, square(coordinat_str));
}

const in_game_time& get_time(const game& g) noexcept
{
  return g.get_in_game_time();
}

bool has_piece_with_id(
  const game& g,
  const piece_id& id
)
{
  for (const auto& p: g.get_pieces())
  {
    if (p.get_id() == id) return true;
  }
  return false;
}

bool is_draw(const game& g)
{
  return is_draw(g.get_pieces());
}


bool is_empty(const game& g, const square& s) noexcept
{
  return !is_piece_at(g, s);
}

bool is_empty_between(
  const game& g,
  const square& from,
  const square& to
)
{
  assert(from != to);
  const auto squares{get_intermediate_squares(from, to)};
  assert(squares.front() == from);
  assert(squares.back() == to);
  assert(squares.size() >= 2);
  const int n{static_cast<int>(squares.size())};
  for (int i{1}; i!=n-1;++i)
  {
    assert(i < n);
    if (!is_empty(g, squares[i])) return false;
  }
  return true;
}

bool is_empty_between(
  const game& g,
  const std::string& from_square_str,
  const std::string& to_square_str
)
{
  return is_empty_between(g, square(from_square_str), square(to_square_str));
}

bool is_idle(const game& g) noexcept
{
  return count_piece_actions(g) == 0;
}

bool is_piece_at(
  const game& g,
  const game_coordinate& coordinat,
  const double distance
) {
  return is_piece_at(g.get_pieces(), coordinat, distance);
}

bool is_piece_at(
  const game& g,
  const square& coordinat
) {
  return is_piece_at(g.get_pieces(), coordinat);
}

bool is_piece_at(
  const game& g,
  const std::string& square_str
) {
  return is_piece_at(g, square(square_str));
}

bool is_square_attacked(
  const game& g,
  const square& s,
  const chess_color attacker_color
)
{
  return is_square_attacked(g.get_pieces(), s, attacker_color);
}

void game::tick(const delta_t& dt)
{
  // Ensure the tick size is limited to its maximum
  const delta_t max_tick(0.25);
  delta_t to_do(dt);
  while (to_do > max_tick)
  {
    this->tick_impl(max_tick);
    to_do = to_do - max_tick;
  }
  // The last bit
  this->tick_impl(to_do);
}

void game::tick_impl(const delta_t& dt)
{
  assert(dt <= delta_t(0.25));

  check_game_and_pieces_agree_on_the_time();
  assert(count_dead_pieces(m_pieces) == 0);

  check_all_occupied_squares_are_unique();

  // Do those piece_actions
  for (auto& p: m_pieces)
  {
    const auto t_before = p.get_in_game_time();
    p.tick(dt, *this);
    const auto t_after = p.get_in_game_time();
    assert(t_before + dt == t_after);
  }

  // Remove dead pieces
  m_pieces.erase(
    std::remove_if(
      std::begin(m_pieces),
      std::end(m_pieces),
      [](const auto& p) { return is_dead(p); }
    ),
    std::end(m_pieces)
  );
  assert(count_dead_pieces(m_pieces) == 0);

  // Something has happened
  if (!collect_message_types(*this).empty())
  {
    check_if_there_is_a_winner();
  }

  // Keep track of the time
  m_in_game_time += dt;

  check_game_and_pieces_agree_on_the_time();
  check_all_occupied_squares_are_unique();
}

void tick(game& g, const delta_t dt)
{
  g.tick(dt);
}


void tick_until_idle(game& g)
{
  int cnt{0};
  while (!is_idle(g))
  {
    g.tick(delta_t(0.1));
    ++cnt;
    assert(cnt < 1000);
  }
}

std::string to_board_str(
  const game& c,
  const board_to_text_options& options
) noexcept
{
  return to_board_str(
    c.get_pieces(),
    options
  );
}

fen_string to_fen_string(const game& g)
{
  const int move_number = g.get_in_game_time().get(); // Round down
  const chess_color active_color{move_number % 2 == 0 ? chess_color::white : chess_color::black};
  const std::string castling_availability{"KQkq"};
  const std::string en_passant_target_square{"-"};
  const int halfmove_number{0};
  const int fullmove_number{move_number + 1};
  return to_fen_str(
    g.get_pieces(),
    active_color,
    castling_availability,
    en_passant_target_square,
    halfmove_number,
    fullmove_number
  );
}

std::string to_pgn(const game& g)
{
  return to_pgn(g.get_pieces());
}

std::ostream& operator<<(std::ostream& os, const game& g) noexcept
{
  os
    << "Time: " << g.get_in_game_time() << " ticks\n"
    << to_board_str(g.get_pieces(), board_to_text_options(true, true))
  ;
  return os;
}
