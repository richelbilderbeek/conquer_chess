#include "user_inputs.h"

#include "game.h"
#include "square.h"
#include "piece_actions.h"

#include <cassert>
#include <iostream>
#include <sstream>

user_inputs::user_inputs()
{

}

void user_inputs::add(const user_input& action)
{
  m_control_actions.push_back(action);
}

int count_user_inputs(const user_inputs& a)
{
  return static_cast<int>(a.get_actions().size());
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
  const std::string& square_str,
  const chess_color player_color
)
{
  do_select(g, square(square_str), player_color);
}

void do_select(
  game& g,
  const std::string& square_str,
  const side player_side
)
{
  do_select(g, square_str, get_player_color(g, player_side));
}

void do_select_and_move_piece(
  game& g,
  const std::string& from_square_str,
  const std::string& to_square_str,
  const side player_side
)
{
  do_select(g, from_square_str, player_side);
  move_cursor_to(g, to_square_str, player_side);
  if (get_controller_type(g, player_side) == controller_type::keyboard)
  {
    g.add_action(
      create_press_action_1(
        to_coordinat(to_square_str),
        player_side
      )
    );
  }
  else
  {
    g.add_action(
      create_press_lmb_action(
        to_coordinat(to_square_str),
        player_side
      )
    );
  }
  g.tick(delta_t(0.0));
  for (int i{0}; i!=2; ++i)
  {
    g.tick(delta_t(0.5));
  }
}

void process_press_action_1(game& g, const user_input& action)
{
  assert(action.get_control_action_type() == user_input_type::press_action_1);
  // press_action_1 can be
  //  1. select (when cursor is pointed to a square with a piece of own color)
  //  2. move (when a piece is selected and cursor points to empty square
  //  3. promote to queen (for a pawn at the final file)

  const side player_side{action.get_player()};
  const chess_color player_color{get_player_color(g, player_side)};
  const square to{square(get_player_pos(g, player_side))};
  #ifdef CONTROL_SYSTEM_2
  const auto maybe_action{get_default_piece_action(g, player_side)};
  if (!maybe_action)
  {
    unselect_all_pieces(g, player_color);
    return;
  }
  const auto piece_action_type{maybe_action.value()};
  const auto selected_pieces{get_selected_pieces(g, player_side)};
  if (selected_pieces.size() == 0)
  {
    assert(piece_action_type == piece_action_type::select);
    const auto cursor_pos{action.get_coordinat()};
    assert(is_piece_at(g, square(cursor_pos)));
    piece& p{get_piece_at(g, square(cursor_pos))};
    assert(player_color == get_player_color(g, player_side));
    assert(player_color == p.get_color());
    p.add_action(
      piece_action(
      player_color,
      p.get_type(),
      piece_action_type,
      p.get_current_square(),
      to
      )
    );
    return;
  }
  const auto selected_piece_copy{selected_pieces[0]};
  auto& selected_piece{get_piece_at(g, selected_piece_copy.get_current_square())};
  selected_piece.add_action(
    piece_action(
      player_color,
      selected_piece.get_type(),
      piece_action_type,
      selected_piece.get_current_square(),
      to
    )
  );
  #else
  if (is_piece_at(g, to) && get_piece_at(g, to).get_color() == player_color)
  {
    const auto& p{get_piece_at(g, to)};
    if (p.is_selected() && can_promote(p))
    {
      //  3. promote to queen (for a pawn at the final file)
      unselect_all_pieces(g, player_color);
      get_piece_at(g, to).add_action(
        piece_action(
          player_color,
          piece_type::queen,
          piece_action_type::promote_to_queen,
          to,
          to
        )
      );
      return;
    }
    else
    {
      //  1. select (when cursor is pointed to a square with a piece of own color)
      unselect_all_pieces(g, player_color);
      get_piece_at(g, to).set_selected(true);
      return;
    }
  }

  //  2. move (when a piece is selected and cursor points to empty square
  if (!has_selected_pieces(g, player_side)) return;

  // 'start_move_unit' will check for piece, color, etc.
  start_move_unit(
    g,
    get_player_pos(g, action.get_player()),
    get_player_color(g, action.get_player())
  );
  #endif
}

void process_press_action_2(game& g, const user_input& action)
{
  assert(action.get_control_action_type() == user_input_type::press_action_2);
  // press_action_2 can be
  //  1. attack (for a selected piece and cursor points to square with enemy
  //  3. promote to rook (for a selected pawn at the final file)

  const side player_side{action.get_player()};
  const chess_color player_color{get_player_color(g, player_side)};
  const square to{square(get_player_pos(g, player_side))};

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
    get_player_pos(g, action.get_player()),
    get_player_color(g, action.get_player())
  );
}

void process_press_action_3(game& g, const user_input& action)
{
  assert(action.get_control_action_type() == user_input_type::press_action_3);
  // press_action_3 can be
  //  1. promote to bishop (for a selected pawn at the final file)
  //  2. castle kingside (for a selected king)

  const side player_side{action.get_player()};
  const chess_color player_color{get_player_color(g, player_side)};
  const square to{square(get_player_pos(g, player_side))};

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
      #ifdef FIX_ISSUE_3
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
      #endif // FIX_ISSUE_3
    }
  }
}

void process_press_action_4(game& g, const user_input& action)
{
  assert(action.get_control_action_type() == user_input_type::press_action_4);
  // press_action_3 can be
  //  1. promote to bishop (for a selected pawn at the final file)
  //  2. castle kingside (for a selected king)

  const side player_side{action.get_player()};
  const chess_color player_color{get_player_color(g, player_side)};
  const square to{square(get_player_pos(g, player_side))};

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
      #ifdef FIX_ISSUE_3
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
      #endif // FIX_ISSUE_3
    }
  }
}

void user_inputs::process(game& g)
{
  for (const auto& action: m_control_actions)
  {
    if (action.get_control_action_type() == user_input_type::press_action_1)
    {
      process_press_action_1(g, action);
    }
    else if (action.get_control_action_type() == user_input_type::press_action_2)
    {
      process_press_action_2(g, action);
    }
    else if (action.get_control_action_type() == user_input_type::press_action_3)
    {
      process_press_action_3(g, action);
    }
    else if (action.get_control_action_type() == user_input_type::press_action_4)
    {
      process_press_action_4(g, action);
    }
    else if (action.get_control_action_type() == user_input_type::press_down)
    {
      auto& pos{get_player_pos(g, action.get_player())};
      pos = get_below(pos);
    }
    else if (action.get_control_action_type() == user_input_type::press_left)
    {
      auto& pos{get_player_pos(g, action.get_player())};
      pos = get_left(pos);
    }
    else if (action.get_control_action_type() == user_input_type::press_right)
    {
      auto& pos{get_player_pos(g, action.get_player())};
      pos = get_right(pos);
    }
    else if (action.get_control_action_type() == user_input_type::press_up)
    {
      auto& pos{get_player_pos(g, action.get_player())};
      pos = get_above(pos);
    }
    else if (action.get_control_action_type() == user_input_type::mouse_move)
    {
      if (has_mouse_controller(g.get_options()))
      {
        auto& pos{get_player_pos(g, action.get_player())};
        pos = action.get_coordinat();
      }
    }
    #ifdef FIX_ISSUE_46
    // Below you see that LMB selects and RMB moves
    // Instead, the mouse controller has an active action
    // triggered by LMB, and RMB goes to the next
    #endif // FIX_ISSUE_46
    else if (action.get_control_action_type() == user_input_type::lmb_down)
    {
      if (!has_mouse_controller(g.get_options())) return;
      #define USE_CONTROL_SYSTEM_FROM_ACTION_1
      #ifdef USE_CONTROL_SYSTEM_FROM_ACTION_1
      assert(action.get_control_action_type() == user_input_type::lmb_down);
      // press_action_1 can be
      //  1. select (when cursor is pointed to a square with a piece of own color)
      //  2. move (when a piece is selected and cursor points to empty square
      //  3. promote to queen (for a pawn at the final file)

      const side player_side{action.get_player()};
      const chess_color player_color{get_player_color(g, player_side)};
      const square to{square(get_player_pos(g, player_side))};
      if (is_piece_at(g, to) && get_piece_at(g, to).get_color() == player_color)
      {
        const auto& p{get_piece_at(g, to)};
        if (p.is_selected() && can_promote(p))
        {
          //  3. promote to queen (for a pawn at the final file)
          unselect_all_pieces(g, player_color);
          get_piece_at(g, to).add_action(
            piece_action(
              player_color,
              piece_type::queen,
              piece_action_type::promote_to_queen,
              to,
              to
            )
          );
          return;
        }
        else
        {
          //  1. select (when cursor is pointed to a square with a piece of own color)
          unselect_all_pieces(g, player_color);
          get_piece_at(g, to).set_selected(true);
          return;
        }
      }

      //  2. move (when a piece is selected and cursor points to empty square
      if (!has_selected_pieces(g, player_side)) return;

      // 'start_move_unit' will check for piece, color, etc.
      start_move_unit(
        g,
        get_player_pos(g, action.get_player()),
        get_player_color(g, action.get_player())
      );
      #else
      const auto player_side{get_mouse_user_player_side(g)};
      const chess_color player_color{get_player_color(g, player_side)};
      const square to{square(get_player_pos(g, player_side))};
      const auto maybe_action{get_default_piece_action(g, player_side)};
      if (!maybe_action)
      {
        unselect_all_pieces(g, player_color);
        return;
      }
      const auto piece_action_type{maybe_action.value()};
      const auto selected_pieces{get_selected_pieces(g, player_side)};
      if (selected_pieces.size() == 0)
      {
        assert(piece_action_type == piece_action_type::select);
        const auto cursor_pos{action.get_coordinat()};
        assert(is_piece_at(g, square(cursor_pos)));
        piece& p{get_piece_at(g, square(cursor_pos))};
        assert(player_color == get_player_color(g, player_side));
        assert(player_color == p.get_color());
        p.add_action(
          piece_action(
          player_color,
          p.get_type(),
          piece_action_type,
          p.get_current_square(),
          to
          )
        );
        return;
      }
      const auto selected_piece_copy{selected_pieces[0]};
      auto& selected_piece{get_piece_at(g, selected_piece_copy.get_current_square())};
      selected_piece.add_action(
        piece_action(
          player_color,
          selected_piece.get_type(),
          piece_action_type,
          selected_piece.get_current_square(),
          to
        )
      );
      /*
      if (has_mouse_controller(g.get_options()))
      {
        do_select(
          g,
          action.get_coordinat(),
          get_player_color(g, action.get_player())
        );
      }
      */
      #endif
    }
    else if (action.get_control_action_type() == user_input_type::rmb_down)
    {
      if (has_mouse_controller(g.get_options()))
      {
        start_move_unit(
          g,
          action.get_coordinat(),
          get_player_color(g, action.get_player())
        );
      }
    }
  }
  m_control_actions = std::vector<user_input>();
}

void start_attack(
  game& g,
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
      /*
      if (from != to)
      {
        // No shift, so all current actions are void
        clear_actions(p);

        p.add_action(
          piece_action(
            p.get_color(),
            p.get_type(),
            piece_action_type::attack,
            square(from),
            square(to)
          )
        );
      }
      */
    }
  }
  unselect_all_pieces(g, player_color);
}

void start_move_unit(
  game& g,
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

void test_control_actions()
{
#ifndef NDEBUG
  // Empty on construction
  {
    const user_inputs c;
    assert(c.get_actions().empty());
  }
  // Move up does something
  {
    game g;
    const game_coordinat before{get_player_pos(g, side::lhs)};
    user_inputs c;
    c.add(create_press_up_action(side::lhs));
    c.process(g);
    const game_coordinat after{get_player_pos(g, side::lhs)};
    assert(before != after);
  }
  // Move right does something
  {
    game g;
    const game_coordinat before{get_player_pos(g, side::lhs)};
    user_inputs c;
    c.add(create_press_right_action(side::lhs));
    c.process(g);
    const game_coordinat after{get_player_pos(g, side::lhs)};
    assert(before != after);
  }
  // Move down does something
  {
    game g;
    const game_coordinat before{get_player_pos(g, side::lhs)};
    user_inputs c;
    c.add(create_press_down_action(side::lhs));
    c.process(g);
    const game_coordinat after{get_player_pos(g, side::lhs)};
    assert(before != after);
  }
  // Move left does something
  {
    game g;
    const game_coordinat before{get_player_pos(g, side::lhs)};
    user_inputs c;
    c.add(create_press_left_action(side::lhs));
    c.process(g);
    const game_coordinat after{get_player_pos(g, side::lhs)};
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
    actions.add(create_press_action_1(to_coordinat("d1"), side::lhs));
    std::stringstream s;
    s << actions;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

user_inputs to_user_inputs(const piece_action& pa, const game& g)
{
  const auto player_color{pa.get_color()};
  const side player_side{get_player_side(g, player_color)};
  const controller c{get_controller(g, player_side)};
  const user_input_type select_action_type{
    c.get_type() == controller_type::mouse ?
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
  return lhs.get_actions() == rhs.get_actions();
}

std::ostream& operator<<(std::ostream& os, const user_inputs& actions) noexcept
{
  const int n = actions.get_actions().size();
  if (n == 0) return os;
  std::stringstream s;
  for (int i{0}; i!=n; ++i)
  {
    s << i << ": " << actions.get_actions()[i] << ", ";
  }
  std::string u{s.str()};
  assert(!u.empty());
  u.pop_back();
  assert(!u.empty());
  u.pop_back();
  os << u;
  return os;
}
