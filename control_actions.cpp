#include "control_actions.h"

#include "game.h"
#include "square.h"

#include <cassert>
#include <iostream>

control_actions::control_actions()
{

}

void control_actions::add(const control_action& action)
{
  m_control_actions.push_back(action);
}

int count_control_actions(const control_actions& a)
{
  return static_cast<int>(a.get_actions().size());
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

void process_press_action_1(game& g, const control_action& action)
{
  assert(action.get_type() == control_action_type::press_action_1);
  // press_action_1 can be
  //  1. select (when cursor is pointed to a square with a piece of own color)
  //  2. move (when a piece is selected and cursor points to empty square
  //  3. promote to queen (for a pawn at the final file)

  const side player_side{action.get_player()};
  const chess_color player_color{get_player_color(g, player_side)};
  const square to{square(get_player_pos(g, player_side))};

  if (is_piece_at(g, to) && get_piece_at(g, to).get_color() == player_color)
  {
    if (can_promote(get_piece_at(g, to)))
    {
      //  3. promote to queen (for a pawn at the final file)
      unselect_all_pieces(g, player_color);
      #ifdef FIX_ISSUE_4
      get_piece_at(g, to).add_action(
        piece_action(
          player_color,
          piece_type::queen,
          piece_action_type::promote,
          to,
          to
        )
      )
      //promote_to(piece_type::queen);
      #endif
      return;
    }
    //  1. select (when cursor is pointed to a square with a piece of own color)
    unselect_all_pieces(g, player_color);
    get_piece_at(g, to).set_selected(true);
    return;
  }

  //  2. move (when a piece is selected and cursor points to empty square
  if (!has_selected_pieces(g, player_side)) return;

  // 'start_move_unit' will check for piece, color, etc.
  start_move_unit(
    g,
    get_player_pos(g, action.get_player()),
    get_player_color(g, action.get_player())
  );
}

void control_actions::process(game& g)
{
  for (const auto& action: m_control_actions)
  {
    if (action.get_type() == control_action_type::press_action_1)
    {
      process_press_action_1(g, action);
    }
    else if (action.get_type() == control_action_type::press_action_2)
    {
      if (has_selected_pieces(g, action.get_player()))
      {
        start_attack(
          g,
          get_player_pos(g, action.get_player()),
          get_player_color(g, action.get_player())
        );
      }
    }
    else if (action.get_type() == control_action_type::press_down)
    {
      auto& pos{get_player_pos(g, action.get_player())};
      pos = get_below(pos);
    }
    else if (action.get_type() == control_action_type::press_left)
    {
      auto& pos{get_player_pos(g, action.get_player())};
      pos = get_left(pos);
    }
    else if (action.get_type() == control_action_type::press_right)
    {
      auto& pos{get_player_pos(g, action.get_player())};
      pos = get_right(pos);
    }
    else if (action.get_type() == control_action_type::press_up)
    {
      auto& pos{get_player_pos(g, action.get_player())};
      pos = get_above(pos);
    }
    else if (action.get_type() == control_action_type::mouse_move)
    {
      if (has_mouse_controller(g.get_options()))
      {
        auto& pos{get_player_pos(g, action.get_player())};
        pos = action.get_coordinat();
      }
    }
    else if (action.get_type() == control_action_type::lmb_down)
    {
      if (has_mouse_controller(g.get_options()))
      {
        do_select(
          g,
          action.get_coordinat(),
          get_player_color(g, action.get_player())
        );
      }
    }
    else if (action.get_type() == control_action_type::rmb_down)
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
  m_control_actions = std::vector<control_action>();
}

void start_attack(
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
    const control_actions c;
    assert(c.get_actions().empty());
  }
  // Move up does something
  {
    game g;
    const game_coordinat before{get_player_pos(g, side::lhs)};
    control_actions c;
    c.add(create_press_up_action(side::lhs));
    c.process(g);
    const game_coordinat after{get_player_pos(g, side::lhs)};
    assert(before != after);
  }
  // Move right does something
  {
    game g;
    const game_coordinat before{get_player_pos(g, side::lhs)};
    control_actions c;
    c.add(create_press_right_action(side::lhs));
    c.process(g);
    const game_coordinat after{get_player_pos(g, side::lhs)};
    assert(before != after);
  }
  // Move down does something
  {
    game g;
    const game_coordinat before{get_player_pos(g, side::lhs)};
    control_actions c;
    c.add(create_press_down_action(side::lhs));
    c.process(g);
    const game_coordinat after{get_player_pos(g, side::lhs)};
    assert(before != after);
  }
  // Move left does something
  {
    game g;
    const game_coordinat before{get_player_pos(g, side::lhs)};
    control_actions c;
    c.add(create_press_left_action(side::lhs));
    c.process(g);
    const game_coordinat after{get_player_pos(g, side::lhs)};
    assert(before != after);
  }
#endif // NDEBUG
}
