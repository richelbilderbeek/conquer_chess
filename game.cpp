#include "game.h"

#include "asserts.h"
#include "game_options.h"
#include "piece_actions.h"
#include "physical_controllers.h"
#include "id.h"
#include "sound_effects.h"
#include "square.h"
#include "user_inputs.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>
#include <sstream>

game::game(
  const game_options& options
)
  : m_options{options},
    m_pieces{get_starting_pieces(options)},
    m_replayer{options.get_replayer()},
    m_t{0.0}
{

}

bool can_castle_kingside(const piece& p, const game& g) noexcept
{
  if (p.get_type() != piece_type::king) return false;
  assert(p.get_type() == piece_type::king);
  if (has_moved(p)) return false;
  const auto king_square{p.get_current_square()};
  // In a starting position, it may be that king has not made a new move
  if (king_square != square("e1") && king_square != square("e8")) return false;
  const auto rook_square(square(king_square.get_x(), 0));
  if (!is_piece_at(g, rook_square)) return false;
  const auto rook{get_piece_at(g, rook_square)};
  if (rook.get_type() != piece_type::rook) return false;
  if (has_moved(rook)) return false;
  const auto b_pawn_square(square(king_square.get_x(), 1));
  if (!is_empty(g, b_pawn_square)) return false;
  const auto c_pawn_square(square(king_square.get_x(), 2));
  if (!is_empty(g, c_pawn_square)) return false;
  const auto d_pawn_square(square(king_square.get_x(), 3));
  if (!is_empty(g, d_pawn_square)) return false;
  // Do not check for moving through check or into check,
  // this would give recursions
  return true;
}

bool can_castle_queenside(const piece& p, const game& g) noexcept
{
  if (p.get_type() != piece_type::king) return false;
  assert(p.get_type() == piece_type::king);
  if (has_moved(p)) return false;
  const auto king_square{p.get_current_square()};
  // In a starting position, it may be that king has not made a new move
  if (king_square != square("e1") && king_square != square("e8")) return false;
  const auto rook_square(square(king_square.get_x(), 7));
  if (!is_piece_at(g, rook_square)) return false;
  const auto rook{get_piece_at(g, rook_square)};
  if (rook.get_type() != piece_type::rook) return false;
  if (has_moved(rook)) return false;
  const auto f_pawn_square(square(king_square.get_x(), 1));
  if (!is_empty(g, f_pawn_square)) return false;
  const auto g_pawn_square(square(king_square.get_x(), 2));
  if (!is_empty(g, g_pawn_square)) return false;
  // Do not check for moving through check or into check,
  // this would give recursions
  return true;
}

bool can_do(
  const game& g,
  const piece& selected_piece,
  const piece_action_type action,
  const square& cursor_square,
  const side player_side
)
{
  if (action == piece_action_type::en_passant)
  {
    return can_do_en_passant(g, selected_piece, cursor_square, player_side);
  }
  if (action == piece_action_type::attack)
  {
    return can_do_attack(g, selected_piece, cursor_square, player_side);
  }
  if (action == piece_action_type::castle_kingside)
  {
    return can_do_castle_kingside(g, selected_piece, cursor_square, player_side);
  }
  if (action == piece_action_type::castle_queenside)
  {
    return can_do_castle_queenside(g, selected_piece, cursor_square, player_side);
  }
  if (action == piece_action_type::move)
  {
    return can_do_move(g, selected_piece, cursor_square, player_side);
  }
  if (action == piece_action_type::promote_to_bishop
    || action == piece_action_type::promote_to_knight
    || action == piece_action_type::promote_to_queen
    || action == piece_action_type::promote_to_rook
  )
  {
    return can_do_promote(g, selected_piece, cursor_square, player_side);
  }
  return false;
}

bool can_do(const game& g,
  const piece& selected_piece,
  const piece_action_type action,
  const std::string& cursor_square_str,
  const side player_side
)
{
  return can_do(
    g,
    selected_piece,
    action,
    square(cursor_square_str),
    player_side
  );
}

bool can_do_attack(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
)
{
  const auto player_color{get_player_color(g, player_side)};
  assert(player_color == selected_piece.get_color());
  // Is it theoretically possible, e.g. on an empty board?
  if (
    !can_attack(
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
  const square& cursor_square,
  const side player_side
)
{
  const auto player_color{get_player_color(g, player_side)};
  assert(player_color == selected_piece.get_color());
  const square king_square{get_default_king_square(player_color)};
  if (!is_piece_at(g, king_square)) return false;
  const square ksc_square{king_square.get_x(), king_square.get_y() + 2};
  if (cursor_square != ksc_square) return false;
  return can_castle_kingside(get_piece_at(g, king_square), g);
}

bool can_do_castle_queenside(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
)
{
  const auto player_color{get_player_color(g, player_side)};
  assert(player_color == selected_piece.get_color());
  const square king_square{get_default_king_square(player_color)};
  if (!is_piece_at(g, king_square)) return false;
  const square qsc_square{king_square.get_x(), king_square.get_y() - 2};
  if (cursor_square != qsc_square) return false;
  return can_castle_queenside(get_piece_at(g, king_square), g);
}

bool can_do_en_passant(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
)
{
  const auto player_color{get_player_color(g, player_side)};
  assert(player_color == selected_piece.get_color());
  assert(is_piece_at(g, selected_piece.get_current_square()));
  assert(get_piece_at(g, selected_piece.get_current_square()).get_color()
    == player_color
  );
  if (is_piece_at(g, cursor_square)) return false;
  #ifdef FIX_ISSUE_21
  assert(!"Find adjacent pieces");
  #endif
  return false;
}

bool can_do_move(
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
)
{
  const auto player_color{get_player_color(g, player_side)};
  assert(player_color == selected_piece.get_color());
  // Is it theoretically possible, e.g. on an empty board?
  if (
    !can_move(
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
  const game& g,
  const piece& selected_piece,
  const square& cursor_square,
  const side player_side
)
{
  const auto player_color{get_player_color(g, player_side)};
  assert(player_color == selected_piece.get_color());
  if (selected_piece.get_current_square() != cursor_square)
  {
    return false;
  }
  return can_promote(selected_piece);
}

bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const game_controller& c,
  const chess_color cursor_color
)
{
  const auto& cursor_pos{
    get_cursor_pos(
      c,
      get_player_side(g, cursor_color)
    )
  };
  if (
    !is_piece_at(
      g,
      cursor_pos,
      g.get_options().get_click_distance()
    )
  )
  {
    return false;
  }
  const auto& piece{get_closest_piece_to(g, cursor_pos)};
  return !piece.is_selected() && piece.get_color() == cursor_color;
}

void clear_piece_messages(game& g) noexcept
{
  for (auto& p: g.get_pieces()) p.clear_messages();
}

action_history collect_action_history(const game& g)
{
  return collect_action_history(g.get_pieces());
}

std::vector<user_inputs> collect_all_user_inputses(
  const game& g,
  const game_controller& c
)
{
  std::vector<user_inputs> user_inputs;

  const auto piece_actions{collect_all_piece_actions(g)};
  user_inputs.reserve(piece_actions.size());
  for (const auto& piece_action: piece_actions) {
    user_inputs.push_back(to_user_inputs(piece_action, g, c));
  }
  return user_inputs;
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
        && has_just_double_moved(get_piece_at(g, enemy_square), g.get_time())
      )
      {
        actions.push_back(piece_action(color, type, piece_action_type::en_passant, from, to_square));
      }
    }
    if (y < 7)
    {
      const square to_square{square(x - 1, y + 1)};
      const square enemy_square{square(x, y + 1)};
      if (is_empty(g, to_square)
        && is_piece_at(g, enemy_square)
        && get_piece_at(g, enemy_square).get_color() == enemy_color
        && has_just_double_moved(get_piece_at(g, enemy_square), g.get_time())
      )
      {
        actions.push_back(piece_action(color, type, piece_action_type::en_passant, from, to_square));
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
        && has_just_double_moved(get_piece_at(g, enemy_square), g.get_time())
      )
      {
        assert(!"YAY, triggered en-passant for FIX_ISSUE_21");
        actions.push_back(piece_action(color, type, piece_action_type::en_passant, from, to_square));
      }
    }
    if (y < 7)
    {
      const square to_square{square(x + 1, y + 1)};
      const square enemy_square{square(x, y + 1)};
      if (is_empty(g, to_square)
        && is_piece_at(g, enemy_square)
        && get_piece_at(g, enemy_square).get_color() == enemy_color
        && has_just_double_moved(get_piece_at(g, enemy_square), g.get_time())
      )
      {
        assert(!"YAY");
        actions.push_back(piece_action(color, type, piece_action_type::en_passant, from, to_square));
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

user_inputs convert_move_to_user_inputs(
  const game& g,
  const game_controller& c,
  const chess_move& m
)
{
  const auto player_side{get_player_side(g, m.get_color())};
  const square from{get_from(g, m)};

  user_inputs inputs;
  // Move the cursor to piece's square
  {
    const auto v{get_user_inputs_to_move_cursor_to(c, from, player_side)};
    add(inputs, v);
  }
  // Select the piece
  {
    const auto i{get_user_input_to_select(c, player_side)};
    inputs.add(i);
  }
  // Move the cursor to target's square
  {
    assert(m.get_to().has_value());
    const auto v{
      get_user_inputs_to_move_cursor_from_to(
        c,
        from,
        m.get_to().value(),
        player_side
      )
    };
    add(inputs, v);
  }
  // Do the action
  {
    const auto i{get_user_input_to_do_action_1(c, player_side)};
    inputs.add(i);
  }
  return inputs;
}

/*
int count_user_inputs(const game& g)
{
  return count_user_inputs(get_user_inputs(g));
}
*/

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

int count_selected_units(
  const game& g
)
{
  return count_selected_units(g.get_pieces());
}

int count_selected_units(
  const game& g,
  const chess_color player
)
{
  return count_selected_units(g.get_pieces(), player);
}

int count_selected_units(
  const game& g,
  const side player_side
)
{
  return count_selected_units(g, get_player_color(g, player_side));
}

game create_randomly_played_game(
  const int n_moves,
  const int seed
)
{
  game g;
  game_controller c;
  std::default_random_engine generator(seed);
  for (int i{0}; i!=n_moves; ++i)
  {
    const user_input action{
      create_random_user_input(generator)
    };
    add_user_input(c, action);
    g.tick(delta_t(0.1));
  }
  return g;
}


bool do_show_selected(const game& g) noexcept
{
  return do_show_selected(g.get_options());
}

void do_promote_keyboard_player_piece(
  game& g,
  game_controller& c,
  const square& pawn_location,
  const piece_type promote_to
)
{
  assert(has_keyboard_controller(c));
  assert(count_selected_units(g, get_keyboard_user_player_color(g, c)) == 1);
  set_keyboard_player_pos(c, pawn_location);
  assert(square(get_cursor_pos(c, side::lhs)) == pawn_location);
  assert(get_piece_at(g, pawn_location).get_type() == piece_type::pawn);
  switch (promote_to)
  {
    case piece_type::bishop:
      add_user_input(c, create_press_action_3(get_keyboard_user_player_side(c)));
      break;
    case piece_type::knight:
      add_user_input(c, create_press_action_4(get_keyboard_user_player_side(c)));
      break;
    case piece_type::king:
    case piece_type::pawn:
    case piece_type::queen:
      assert(promote_to == piece_type::queen);
      add_user_input(
        c,
        create_press_action_1(
          get_keyboard_user_player_side(c)
        )
      );
      break;
    case piece_type::rook:
      add_user_input(c, create_press_action_2(get_keyboard_user_player_side(c)));
      break;
  }
  c.apply_user_inputs_to_game(g);
  g.tick(delta_t(0.0));
  assert(count_user_inputs(c) == 0);
}

void do_start_attack_keyboard_player_piece(
  game& g,
  game_controller& c,
  const square& s
)
{
  assert(has_keyboard_controller(c));
  assert(count_selected_units(g, get_keyboard_user_player_color(g, c)) == 1);
  set_keyboard_player_pos(c, s);
  assert(square(get_cursor_pos(c, side::lhs)) == s);
  add_user_input(c, create_press_action_2(get_keyboard_user_player_side(c)));
  assert(count_user_inputs(c) != 0);
  c.apply_user_inputs_to_game(g);
  g.tick(delta_t(0.0));
  assert(count_user_inputs(c) == 0);
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
  const game_coordinat& coordinat
)
{
  return g.get_pieces()[get_index_of_closest_piece_to(g, coordinat)];
}

piece& get_closest_piece_to(
  game& g,
  const game_coordinat& coordinat
)
{
  return g.get_pieces()[get_index_of_closest_piece_to(g, coordinat)];
}

/*

const physical_controller& get_physical_controller(const game& g, const side player)
{
  return ::get_physical_controller(g.get_controller(), player);
}

physical_controller_type get_physical_controller_type(const game& g, const side player)
{
  return get_physical_controller(g, player).get_type();
}
*/

const game_coordinat& get_cursor_pos(
  const game& g,
  const game_controller& c,
  const chess_color cursor_color
)
{
  return get_cursor_pos(c, get_player_side(g, cursor_color));
}

/*
const game_coordinat& get_cursor_pos(
  const game& g,
  const side player_side
)
{
  return g.get_controller().get_cursor_pos(player_side);
}

square get_cursor_square(
  const game& g,
  const side player_side
)
{
  const game_coordinat cursor_pos{get_cursor_pos(g, player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  return square(cursor_pos);
}
*/

game get_default_game() noexcept
{
  return game{
    create_default_game_options()
  };
}

std::optional<piece_action_type> get_default_piece_action(
  const game& g,
  const square& cursor_square,
  const side player_side
) noexcept
{

  if (has_selected_pieces(g, player_side))
  {
    // Has selected pieces
    const chess_color player_color{get_player_color(g, player_side)};

    if (
      is_piece_at(g, cursor_square)
      && get_piece_at(g, cursor_square).get_color() == player_color
    )
    {
      const piece& p{get_piece_at(g, cursor_square)};
      if (p.is_selected())
      {
        if (can_promote(p))
        {
          return piece_action_type::promote_to_queen;
        }
        else
        {
          return piece_action_type::unselect;
        }
      }
    }
    else
    {
      // No own piece at cursor, maybe can move/castle/en-passant there?
      assert(get_selected_pieces(g, player_side).size() == 1);
      const auto selected_piece{get_selected_pieces(g, player_side)[0]};
      if (can_do(g, selected_piece, piece_action_type::en_passant, cursor_square, player_side))
      {
        return piece_action_type::en_passant;
      }
      if (can_do(g, selected_piece, piece_action_type::castle_kingside, cursor_square, player_side))
      {
        return piece_action_type::castle_kingside;
      }
      if (can_do(g, selected_piece, piece_action_type::castle_queenside, cursor_square, player_side))
      {
        return piece_action_type::castle_queenside;
      }
      if (can_do(g, selected_piece, piece_action_type::move, cursor_square, player_side))
      {
        return piece_action_type::move;
      }
      if (can_do(g, selected_piece, piece_action_type::attack, cursor_square, player_side))
      {
        return piece_action_type::attack;
      }
    }
  }
  else
  {
    if (is_piece_at(g, cursor_square))
    {
      const piece& p{get_piece_at(g, cursor_square)};
      const auto player_color{get_player_color(g, player_side)};
      if (p.get_color() == player_color)
      {
        if (!p.is_selected()) return piece_action_type::select;
      }
    }
  }
  return std::optional<piece_action_type>();
}

game get_game_with_starting_position(starting_position_type t) noexcept
{
  const game_options options(
    get_default_screen_size(),
    t,
    get_default_game_speed(),
    get_default_margin_width()
  );
  return game(options);
}

id get_id(const game& g, const square& s)
{
  assert(is_piece_at(g, s));
  return get_piece_at(g, s).get_id();
}

int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinat& coordinat
)
{
  const std::vector<double> distances{
    calc_distances(g.get_pieces(), coordinat)
  };
  const auto iter = std::min_element(std::begin(distances), std::end(distances));
  const auto index = std::distance(std::begin(distances), iter);
  return index;
}

chess_color get_keyboard_user_player_color(
  const game& g,
  const game_controller& c
)
{
  return get_player_color(g, get_keyboard_user_player_side(c));
}

/*
sf::Keyboard::Key get_key_for_action(
  const game& g,
  const side player,
  const action_number& action
)
{
  return get_key_for_action(g.get_options(), player, action);
}
*/

game get_kings_only_game() noexcept
{
  return get_game_with_starting_position(starting_position_type::kings_only);
}

/*
const game_view_layout& get_layout(const game& g) noexcept
{
  return g.get_layout();
}
*/

chess_color get_mouse_user_player_color(
  const game& g,
  const game_controller& c
)
{
  return get_player_color(g, get_mouse_user_player_side(c));
}

double get_music_volume_as_percentage(const game& g) noexcept
{
  return get_music_volume_as_percentage(g.get_options());
}

std::vector<square> get_occupied_squares(const game& g) noexcept
{
  return get_occupied_squares(get_pieces(g));
}

const game_options& get_options(const game& g)
{
  return g.get_options();
}

std::vector<piece>& get_pieces(game& g) noexcept
{
  return g.get_pieces();
}

const std::vector<piece>& get_pieces(const game& g) noexcept
{
  return g.get_pieces();
}

piece& get_piece_that_moves(game& g, const chess_move& move)
{
  assert(is_simple_move(move));
  assert(!g.get_pieces().empty());
  const auto& pieces{g.get_pieces()};
  const int n_pieces{static_cast<int>(pieces.size())};
  for (int i{0}; i!=n_pieces; ++i)
  {
    auto& ps{g.get_pieces()};
    auto& piece{ps[i]};
    if (piece.get_color() != move.get_color()) continue;
    const auto& color{piece.get_color()};
    assert(move.get_type().has_value());
    if (piece.get_type() != move.get_type().value()) continue;
    const auto& piece_type{piece.get_type()};
    const auto& from{piece.get_current_square()};
    assert(move.get_to().has_value());
    const auto& to{move.get_to().value()};
    if (can_move(color, piece_type, from, to))
    {
      return piece;
    }
  }
  assert(!"Should not het here");
}


piece get_piece_with_id(
  const game& g,
  const id& i
)
{
  return get_piece_with_id(g.get_pieces(), i);
}


chess_color get_player_color(
  const game& g,
  const side player_side
) noexcept
{
  return get_player_color(g.get_options(), player_side);
}

side get_player_side(const game& g, const chess_color& color) noexcept
{
  if (get_player_color(g, side::lhs) == color) return side::lhs;
  assert(get_player_color(g, side::rhs) == color);
  return side::rhs;
}

std::vector<square> get_possible_moves(
  const game& g,
  const side player
)
{
  const auto selected_pieces{get_selected_pieces(g, player)};
  if (selected_pieces.empty()) return {};
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};
  return get_possible_moves(
    get_pieces(g),
    selected_piece
  );
}

std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
)
{
  return get_selected_pieces(g.get_pieces(), player);
}

std::vector<piece> get_selected_pieces(
  const game& g,
  const side player
)
{
  return get_selected_pieces(g.get_pieces(), get_player_color(g, player));
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

const piece& get_piece_at(const game& g, const square& coordinat)
{
  return get_piece_at(g.get_pieces(), coordinat);
}

piece& get_piece_at(game& g, const square& coordinat)
{
  assert(is_piece_at(g, coordinat));
  return get_piece_at(g.get_pieces(), coordinat);
}

const piece& get_piece_at(const game& g, const std::string& coordinat_str)
{
  return get_piece_at(g, square(coordinat_str));
}

const delta_t& get_time(const game& g) noexcept
{
  return g.get_time();
}

bool has_selected_pieces(const game& g, const chess_color player)
{
  return !get_selected_pieces(g, player).empty();
}

bool has_selected_pieces(const game& g, const side player)
{
  return !get_selected_pieces(g, player).empty();
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
  const game_coordinat& coordinat,
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

bool piece_with_id_is_at(
  game& g,
  const id& i,
  const square& s
)
{
  assert(is_piece_at(g, s));
  return get_piece_at(g, s).get_id() == i;
}




void game::tick(const delta_t& dt)
{
  // Let the replayer do its move
  #ifdef FIX_ISSUE_REPLAYER_BACK
  m_replayer.do_move(m_controller, *this); // TODO: fix architecture
  #endif

  assert(count_dead_pieces(m_pieces) == 0);

  // Do those piece_actions
  for (auto& p: m_pieces) p.tick(dt, *this);

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

  // Keep track of the time
  m_t += dt;
}

void unselect_all_pieces(
  game& g,
  const chess_color color
)
{
  return unselect_all_pieces(g.get_pieces(), color);
}

void set_cursor_pos(
  game_controller& c,
  const game_coordinat& pos,
  const side player_side
) noexcept
{
  c.set_cursor_pos(pos, player_side);
}

void set_cursor_pos(
  game_controller& c,
  const square& s,
  const side player_side
) noexcept
{
  set_cursor_pos(c, to_coordinat(s), player_side);
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

void toggle_left_player_color(game& g)
{
  toggle_left_player_color(g.get_options());
}

std::string to_pgn(const game& g)
{
  return to_pgn(g.get_pieces());
}

std::ostream& operator<<(std::ostream& os, const game& g) noexcept
{
  os
    << "Time: " << g.get_time() << " ticks\n"
    << to_board_str(g.get_pieces(), board_to_text_options(true, true)) << '\n'
    //<< "Layout: " << g.get_layout() << '\n'
    << "Options: " << g.get_options() << '\n'
    << "Replayer: " << g.get_replayer()
  ;
  return os;
}
