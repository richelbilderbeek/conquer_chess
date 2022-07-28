#include "control_actions.h"

#include "game.h"
#include "square.h"

#include <cassert>

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

void control_actions::do_select(
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

void control_actions::do_select(
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

void control_actions::process(game& g)
{
  process_control_actions(g);
}

void control_actions::process_control_actions(game& g)
{
  for (const auto& action: m_control_actions)
  {
    if (action.get_type() == control_action_type::press_attack)
    {
      start_attack(
        g,
        get_keyboard_player_pos(g),
        get_keyboard_user_player_color(get_options(g))
      );
    }
    else if (action.get_type() == control_action_type::press_down)
    {
      auto& pos{get_keyboard_player_pos(g)};
      pos = get_below(pos);
    }
    else if (action.get_type() == control_action_type::press_left)
    {
      auto& pos{get_keyboard_player_pos(g)};
      pos = get_left(pos);
    }
    else if (action.get_type() == control_action_type::press_move)
    {
      start_move_unit(
        g,
        get_keyboard_player_pos(g),
        get_keyboard_user_player_color(get_options(g))
      );
    }
    else if (action.get_type() == control_action_type::press_right)
    {
      auto& pos{get_keyboard_player_pos(g)};
      pos = get_right(pos);
    }
    else if (action.get_type() == control_action_type::press_select)
    {
      do_select(
        g,
        get_keyboard_player_pos(g),
        get_keyboard_user_player_color(get_options(g))
      );
    }
    else if (action.get_type() == control_action_type::press_up)
    {
      auto& pos{get_keyboard_player_pos(g)};
      pos = get_above(pos);
    }
    else if (action.get_type() == control_action_type::mouse_move)
    {
      auto& pos{get_mouse_player_pos(g)};
      pos = action.get_coordinat();
    }
    else if (action.get_type() == control_action_type::lmb_down)
    {
      do_select(
        g,
        action.get_coordinat(),
        get_mouse_user_player_color(get_options(g))
      );
    }
    else if (action.get_type() == control_action_type::rmb_down)
    {
      start_move_unit(
        g,
        action.get_coordinat(),
        get_mouse_user_player_color(get_options(g))
      );
    }
  }
  m_control_actions = std::vector<control_action>();
}

void control_actions::start_attack(
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
      const auto& from{p.get_coordinat()};
      const auto& to{coordinat};
      if (from != to)
      {
        // No shift, so all current actions are void
        clear_actions(p);

        p.add_action(
          piece_action(
            p.get_player(),
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

void control_actions::start_move_unit(
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
      const auto& from{p.get_coordinat()};
      const auto& to{coordinat};
      if (from != to)
      {
        // No shift, so all current actions are void
        clear_actions(p);

        p.add_action(
          piece_action(
            p.get_player(),
            p.get_type(),
            piece_action_type::move,
            square(from),
            square(to)
          )
        );
      }
    }
  }
  unselect_all_pieces(g, player_color);
}
