#include "user_inputs.h"

#include "asserts.h"
#include "game.h"
#include "game_controller.h"
#include "square.h"
#include "game_coordinat.h"
#include "piece_actions.h"

#include <cassert>
#include <iostream>
#include <sstream>

user_inputs::user_inputs(const std::vector<user_input>& inputs)
  : m_user_inputs{inputs}
{

}

void user_inputs::add(const user_input& action)
{
  m_user_inputs.push_back(action);
}

void add(user_inputs& current, const user_inputs& to_be_added)
{
  for (const auto& i: to_be_added.get_user_inputs())
  {
    current.add(i);
  }
}

int count_user_inputs(const user_inputs& a)
{
  return static_cast<int>(a.get_user_inputs().size());
}

user_inputs create_control_actions(
  const std::string& pgn_str,
  const chess_color color,
  const game& g
)
{
  return create_control_actions(
    chess_move(pgn_str, color),
    g
  );
}

user_inputs create_control_actions(
  const chess_move& m,
  const game& g
)
{
  assert(m.get_winner().empty());
  assert(!g.get_pieces().empty());

  return user_inputs();
}


void do_select(
  game& g,
  game_controller& /* c */,
  const game_coordinat& coordinat,
  const chess_color player_color
)
{
  // #|Has selected pieces? |Clicking on what?|Do what?
  // -|---------------------|-----------------|----------------
  // 1|Yes                  |Selected unit    |Unselect unit
  // 2|Yes                  |Unselected unit  |Select unit
  // 3|Yes                  |Empty square     |Unselect all units
  // 4|No                   |Selected unit    |NA
  // 5|No                   |Unselected unit  |Select unit
  // 6|No                   |Empty square     |Nothing
  if (has_selected_pieces(g, player_color))
  {
    if (is_piece_at(g, coordinat)) {

      auto& piece{get_closest_piece_to(g, coordinat)};
      if (piece.get_color() == player_color)
      {
        if (piece.is_selected())
        {
          unselect(piece); // 1
        }
        else
        {
          unselect_all_pieces(g, player_color);
          select(piece); // 2
        }
      }
    }
    else
    {
      unselect_all_pieces(g, player_color); // 3
    }
  }
  else
  {
    if (is_piece_at(g, coordinat)) {
      auto& piece{get_closest_piece_to(g, coordinat)};
      if (piece.get_color() == player_color)
      {
        if (piece.is_selected())
        {
          assert(!"Should never happen, as there are no selected pieces at all");
          unselect(piece); // 4
        }
        else
        {
          select(piece); // 5
        }
      }
    }
    else
    {
      // 6
    }
  }
}

void do_select(
  game& g,
  game_controller& /* c */,
  const square& coordinat,
  const chess_color player_color
)
{
  // #|Has selected pieces? |Clicking on what?|Do what?
  // -|---------------------|-----------------|----------------
  // 1|Yes                  |Selected unit    |Unselect unit
  // 2|Yes                  |Unselected unit  |Select unit
  // 3|Yes                  |Empty square     |Unselect all units
  // 4|No                   |Selected unit    |NA
  // 5|No                   |Unselected unit  |Select unit
  // 6|No                   |Empty square     |Nothing
  if (has_selected_pieces(g, player_color))
  {
    if (is_piece_at(g, coordinat)) {

      auto& piece{get_piece_at(g, coordinat)};
      if (piece.get_color() == player_color)
      {
        if (piece.is_selected())
        {
          unselect(piece); // 1
        }
        else
        {
          unselect_all_pieces(g, player_color);
          select(piece); // 2
        }
      }
    }
    else
    {
      unselect_all_pieces(g, player_color); // 3
    }
  }
  else
  {
    if (is_piece_at(g, coordinat)) {
      auto& piece{get_piece_at(g, coordinat)};
      if (piece.get_color() == player_color)
      {
        if (piece.is_selected())
        {
          assert(!"Should never happen, as there are no selected pieces at all");
          unselect(piece); // 4
        }
        else
        {
          select(piece); // 5
        }
      }
    }
    else
    {
      // 6
    }
  }
}

void do_select(
  game& g,
  game_controller& c,
  const std::string& square_str,
  const chess_color player_color
)
{
  do_select(g, c, square(square_str), player_color);
}

void do_select(
  game& g,
  game_controller& c,
  const std::string& square_str,
  const side player_side
)
{
  do_select(g, c, square_str, get_player_color(g, player_side));
}

void do_select_and_move_piece(
  game& g,
  game_controller& c,
  const std::string& from_square_str,
  const std::string& to_square_str,
  const side player_side
)
{
  do_select(g, c, from_square_str, player_side);
  move_cursor_to(c, to_square_str, player_side);
  add_user_input(
    c,
    get_user_input_to_select(c, player_side)
  );
  user_inputs inputs{c.get_user_inputs()};
  c.apply_user_inputs_to_game(g);
  g.tick(delta_t(0.0));
  for (int i{0}; i!=2; ++i)
  {
    g.tick(delta_t(0.5));
  }
}

bool is_empty(const user_inputs& inputs) noexcept
{
  return inputs.get_user_inputs().empty();
}

void process_press_action_1(
  game& g,
  game_controller& c,
  const user_input& action
)
{
  process_press_action_1_or_lmb_down(g, c, action);
}

void process_press_action_1_or_lmb_down(
  game& g,
  game_controller& c,
  const user_input& action
)
{
  assert(action.get_user_input_type() == user_input_type::press_action_1
    || action.get_user_input_type() == user_input_type::lmb_down
  );
  // this action can be
  //  1. select (when cursor is pointed to a square with a piece of own color)
  //  2. move (when a piece is selected and cursor points to empty square
  //  3. promote to queen (for a pawn at the final file)
  //  4. unselect (when cursor is pointed to a square with a piece of own color)
  //  5. promotion (when a king is selected and cursor points to empty square
  //  6. attack (when a piece is selected and cursor points to an enemy square)
  //  7. castling (when a king is selected and cursor points to an empty target)



  // Cursor location | Selected piece location |
  //                 |

  const side player_side{action.get_player()};
  const chess_color player_color{get_player_color(g, player_side)};
  const game_coordinat cursor_pos{get_cursor_pos(c, player_side)};
  if (!is_coordinat_on_board(cursor_pos))
  {
    return;
  }
  const square cursor{square(cursor_pos)};
  const bool is_promotion_to_queen{
       is_piece_at(g, cursor)
    && get_piece_at(g, cursor).get_color() == player_color
    && get_piece_at(g, cursor).is_selected()
    && can_promote(get_piece_at(g, cursor))
  };
  #define FIX_ISSUE_3
  #ifdef FIX_ISSUE_3
  const square king_square{get_default_king_square(player_color)};
  const bool is_castle_kingside{
       !is_piece_at(g, cursor)
    && is_piece_at(g, king_square)
    && get_piece_at(g, king_square).get_color() == player_color
    && get_piece_at(g, king_square).is_selected()
    && can_castle_kingside(get_piece_at(g, king_square), g)
  };
  if (is_castle_kingside)
  {
    unselect_all_pieces(g, player_color);
    get_piece_at(g, get_default_king_square(player_color)).add_action(
      piece_action(
        player_color,
        piece_type::king,
        piece_action_type::castle_kingside,
        get_default_king_square(player_color),
        cursor
      )
    );
    get_piece_at(g, get_default_rook_square(player_color, castling_type::king_side)).add_action(
      piece_action(
        player_color,
        piece_type::rook,
        piece_action_type::castle_kingside,
        get_default_rook_square(player_color, castling_type::king_side),
        cursor
      )
    );
    return;
  }
  #endif // FIX_ISSUE_3
  if (is_piece_at(g, cursor) && get_piece_at(g, cursor).get_color() == player_color)
  {
    const auto& p{get_piece_at(g, cursor)};
    if (p.is_selected())
    {
      if (can_promote(p))
      {
        //  3. promote to queen (for a pawn at the final file)
        unselect_all_pieces(g, player_color);
        assert(is_promotion_to_queen);
        get_piece_at(g, cursor).add_action(
          piece_action(
            player_color,
            piece_type::queen,
            piece_action_type::promote_to_queen,
            cursor,
            cursor
          )
        );
        return;
      }
      else
      {
        //  4. unselect (when cursor is pointed to a square with a piece of own color)
        get_piece_at(g, cursor).add_action(
          piece_action(
            player_color,
            p.get_type(),
            piece_action_type::unselect,
            cursor,
            cursor
          )
        );
        return;
      }
    }
    else
    {
      //  1. select (when cursor is pointed to a square with a piece of own color)
      unselect_all_pieces(g, player_color);
      get_piece_at(g, cursor).add_action(
        piece_action(
          player_color,
          p.get_type(),
          piece_action_type::select,
          cursor,
          cursor
        )
      );
      return;
    }
  }

  // Options left:
  //  2. move (when a piece is selected and cursor points to empty square
  //  5. promotion (when a king is selected and cursor points to empty square

  if (!has_selected_pieces(g, player_side)) return;

  // 'start_move_unit' will check for piece, color, etc.
  start_move_unit(
    g,
    c,
    get_cursor_pos(c, action.get_player()),
    get_player_color(g, action.get_player())
  );
}

void process_press_action_2(
  game& g,
  game_controller& c,
  const user_input& action
)
{
  assert(action.get_user_input_type() == user_input_type::press_action_2);
  // press_action_2 can be
  //  1. attack (for a selected piece and cursor points to square with enemy
  //  3. promote to rook (for a selected pawn at the final file)

  const side player_side{action.get_player()};
  const chess_color player_color{get_player_color(g, player_side)};
  const square to{square(get_cursor_pos(c, player_side))};

  if (is_piece_at(g, to) && get_piece_at(g, to).get_color() == player_color)
  {
    const auto& p{get_piece_at(g, to)};
    if (p.is_selected())
    {
      if (can_promote(p))
      {
        //  3. promote to queen (for a pawn at the final file)
        unselect_all_pieces(g, player_color);
        get_piece_at(g, to).add_action(
          piece_action(
            player_color,
            piece_type::rook,
            piece_action_type::promote_to_rook,
            to,
            to
          )
        );
        return;
      }
    }
  }
  start_attack(
    g,
    c,
    get_cursor_pos(c, action.get_player()),
    get_player_color(g, action.get_player())
  );
}

void process_press_action_3(
  game& g,
  game_controller& c,
  const user_input& action
)
{
  assert(action.get_user_input_type() == user_input_type::press_action_3);
  // press_action_3 can be
  //  1. promote to bishop (for a selected pawn at the final file)
  //  2. castle kingside (for a selected king)

  const side player_side{action.get_player()};
  const chess_color player_color{get_player_color(g, player_side)};
  const square to{square(get_cursor_pos(c, player_side))};

  if (is_piece_at(g, to) && get_piece_at(g, to).get_color() == player_color)
  {
    const auto& p{get_piece_at(g, to)};
    if (p.is_selected())
    {
      if (can_promote(p))
      {
        //  3. promote to queen (for a pawn at the final file)
        unselect_all_pieces(g, player_color);
        get_piece_at(g, to).add_action(
          piece_action(
            player_color,
            piece_type::bishop,
            piece_action_type::promote_to_bishop,
            to,
            to
          )
        );
      }
      #ifdef FIX_ISSUE_3_2
      else if (can_castle(p, g))
      {
        unselect_all_pieces(g, player_color);
        get_piece_at(g, to).add_action(
          piece_action(
            player_color,
            piece_type::king,
            piece_action_type::castle_kingside,
            to,
            to
          )
        );
        assert(!"Also do the rook");
      }
      #endif // FIX_ISSUE_3_2
    }
  }
}

void process_press_action_4(
  game& g,
  game_controller& c,
  const user_input& action
)
{
  assert(action.get_user_input_type() == user_input_type::press_action_4);
  // press_action_3 can be
  //  1. promote to bishop (for a selected pawn at the final file)
  //  2. castle kingside (for a selected king)

  const side player_side{action.get_player()};
  const chess_color player_color{get_player_color(g, player_side)};
  const square to{square(get_cursor_pos(c, player_side))};

  if (is_piece_at(g, to) && get_piece_at(g, to).get_color() == player_color)
  {
    const auto& p{get_piece_at(g, to)};
    if (p.is_selected())
    {
      if (can_promote(p))
      {
        //  3. promote to knight (for a pawn at the final file)
        unselect_all_pieces(g, player_color);
        get_piece_at(g, to).add_action(
          piece_action(
            player_color,
            piece_type::knight,
            piece_action_type::promote_to_knight,
            to,
            to
          )
        );
      }
      #ifdef FIX_ISSUE_3_2
      else if (can_castle(p, g))
      {
        unselect_all_pieces(g, player_color);
        get_piece_at(g, to).add_action(
          piece_action(
            player_color,
            piece_type::king,
            piece_action_type::castle_queenside,
            to,
            to
          )
        );
        assert(!"Also do the rook");
      }
      #endif // FIX_ISSUE_3_2
    }
  }
}

void start_attack(
  game& g,
  game_controller& /* c */,
  const game_coordinat& coordinat,
  const chess_color player_color
)
{
  const auto actions{collect_all_piece_actions(g)};

  if (count_selected_units(g, player_color) == 0) return;

  for (auto& p: g.get_pieces())
  {
    if (p.is_selected() && p.get_color() == player_color)
    {
      const auto& from{p.get_current_square()};
      const auto& to{square(coordinat)};
      const piece_action action(
        p.get_color(),
        p.get_type(),
        piece_action_type::attack,
        square(from),
        square(to)
      );
      if (is_in(action, actions))
      {
        clear_actions(p);
        p.add_action(action);
      }
      else
      {
        p.add_message(message_type::cannot);
      }
    }
  }
  unselect_all_pieces(g, player_color);
}

void start_move_unit(
  game& g,
  game_controller& /* c */,
  const game_coordinat& coordinat,
  const chess_color player_color
)
{
  if (count_selected_units(g, player_color) == 0) return;

  for (auto& p: g.get_pieces())
  {
    if (p.is_selected() && p.get_color() == player_color)
    {

      const auto& from{p.get_current_square()};
      const auto& to{square(coordinat)};
      if (from != to)
      {
        // No shift, so all current actions are void
        clear_actions(p);
        const auto action{
          piece_action(
            p.get_color(),
            p.get_type(),
            piece_action_type::move,
            square(from),
            square(to)
          )
        };
        p.add_action(action);
      }
    }
  }
  unselect_all_pieces(g, player_color);
}

void test_user_inputs()
{
#ifndef NDEBUG
  // Empty on construction
  {
    const user_inputs c;
    assert(c.get_user_inputs().empty());
  }
  // Move up does something
  {
    game g;
    game_controller c;
    const game_coordinat before{get_cursor_pos(c, side::lhs)};
    c.add_user_input(create_press_up_action(side::lhs));
    c.apply_user_inputs_to_game(g);
    const game_coordinat after{get_cursor_pos(c, side::lhs)};
    assert(before != after);
  }
  // Move right does something
  {
    game g;
    game_controller c;
    const game_coordinat before{get_cursor_pos(c, side::lhs)};
    c.add_user_input(create_press_right_action(side::lhs));
    c.apply_user_inputs_to_game(g);
    const game_coordinat after{get_cursor_pos(c, side::lhs)};
    assert(before != after);
  }
  // Move down does something
  {
    game g;
    game_controller c;
    const game_coordinat before{get_cursor_pos(c, side::lhs)};
    c.add_user_input(create_press_down_action(side::lhs));
    c.apply_user_inputs_to_game(g);
    const game_coordinat after{get_cursor_pos(c, side::lhs)};
    assert(before != after);
  }
  // Move left does something
  {
    game g;
    game_controller c;
    const game_coordinat before{get_cursor_pos(c, side::lhs)};
    c.add_user_input(create_press_left_action(side::lhs));
    c.apply_user_inputs_to_game(g);
    const game_coordinat after{get_cursor_pos(c, side::lhs)};
    assert(before != after);
  }
  // 37: operator<< for no actions
  {
    const user_inputs actions;
    std::stringstream s;
    s << actions;
    assert(s.str().empty());
  }
  // 37: operator<< for one action
  {
    user_inputs actions;
    actions.add(create_press_action_1(side::lhs));
    std::stringstream s;
    s << actions;
    assert(!s.str().empty());
  }
  #define FIX_ISSUE_64
  #ifdef FIX_ISSUE_64
  // 64: move white's cursor to e2
  {
    game g;
    game_controller c;
    assert(square(get_cursor_pos(c, side::lhs)) != square("e2"));
    auto inputs{
      get_user_inputs_to_move_cursor_to(c, square("e2"), side::lhs)
    };
    assert(!is_empty(inputs));
    add_user_inputs(c, inputs);
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));
    assert(square(get_cursor_pos(c, side::lhs)) == square("e2"));
  }
  // 64: move white's cursor to e2 and select the pawn
  {
    game g;
    game_controller c;
    move_cursor_to(c, "e2", side::lhs);
    assert(!get_piece_at(g, "e2").is_selected());
    const user_input input{get_user_input_to_select(c, side::lhs)};
    add_user_input(c, input);
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));
    assert(get_piece_at(g, "e2").is_selected());
  }
  #endif
#endif // NDEBUG
}

user_inputs to_user_inputs(
  const piece_action& pa,
  const game& g,
  const game_controller& c
)
{
  const auto player_color{pa.get_color()};
  const side player_side{get_player_side(g, player_color)};
  const physical_controller pc{get_physical_controller(c, player_side)};
  const user_input_type select_action_type{
    pc.get_type() == physical_controller_type::mouse ?
    user_input_type::lmb_down :
    user_input_type::press_down
  };

  const user_input select(
    select_action_type,
    player_side,
    to_coordinat(pa.get_from())
  );

  user_inputs v;
  v.add(select);
  return v;
}

bool operator==(const user_inputs& lhs, const user_inputs& rhs) noexcept
{
  return lhs.get_user_inputs() == rhs.get_user_inputs();
}

std::ostream& operator<<(std::ostream& os, const user_inputs& actions) noexcept
{
  const int n = actions.get_user_inputs().size();
  if (n == 0) return os;
  std::stringstream s;
  for (int i{0}; i!=n; ++i)
  {
    s << i << ": " << actions.get_user_inputs()[i] << ", ";
  }
  std::string u{s.str()};
  assert(!u.empty());
  u.pop_back();
  assert(!u.empty());
  u.pop_back();
  os << u;
  return os;
}
