#include "game.h"

#include "sound_effects.h"
#include "square.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>

game::game(
  const game_options& options
)
  : m_control_actions{},
    m_layout{options.get_screen_size(), options.get_margin_width()},
    m_player_1_pos{0.5, 4.5},
    m_player_2_pos{7.5, 4.5},
    m_options{options},
    m_pieces{get_starting_pieces(options)}
{

}

void game::add_action(const control_action a)
{
  // These will be processed in 'tick'
  m_control_actions.add(a);
}

bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const chess_color player
)
{
  const auto& cursor_pos{get_cursor_pos(g, player)};
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
  return !piece.is_selected() && piece.get_color() == player;
}

void clear_sound_effects(game& g) noexcept
{
  g.get_actions().clear_sound_effects();
}

int count_control_actions(const game& g)
{
  return count_control_actions(g.get_actions());
}

int count_piece_actions(
  const game& g,
  const chess_color player
)
{
  return count_piece_actions(g.get_pieces(), player);
}

int count_selected_units(
  const game& g,
  const chess_color player
)
{
  return count_selected_units(g.get_pieces(), player);
}

game create_king_versus_king_game()
{
  const game_options options(
    get_default_screen_size(),
    starting_position_type::kings_only,
    game_speed::normal,
    get_default_margin_width()
  );
  return game(options);
}

bool do_show_selected(const game& g) noexcept
{
  return do_show_selected(g.get_options());
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

game_coordinat get_cursor_pos(
  const game& g,
  const chess_color c
)
{
  if (c == get_keyboard_user_player_color(g.get_options()))
  {
    return g.get_player_1_pos();
  }
  assert(c == get_mouse_user_player_color(g.get_options()));
  return g.get_player_2_pos();
}

game get_default_game() noexcept
{
  return game{
    get_default_game_options()
  };
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

game_coordinat& game::get_keyboard_player_pos()
{
  if (get_left_player_controller(m_options) == controller_type::keyboard)
  {
    return m_player_1_pos;
  }
  assert(get_right_player_controller(m_options) == controller_type::keyboard);
  return m_player_2_pos;
}

game_coordinat get_keyboard_player_pos(const game& g)
{
  if (get_left_player_controller(g.get_options()) == controller_type::keyboard)
  {
    return g.get_player_1_pos();
  }
  assert(get_right_player_controller(g.get_options()) == controller_type::keyboard);
  return g.get_player_2_pos();
}

game_coordinat& get_keyboard_player_pos(game& g)
{
  return g.get_keyboard_player_pos();
}

chess_color get_keyboard_user_player_color(const game& g)
{
  return get_keyboard_user_player_color(g.get_options());
}

game_coordinat& game::get_mouse_player_pos()
{
  if (get_left_player_controller(m_options) == controller_type::mouse)
  {
    return m_player_1_pos;
  }
  assert(get_right_player_controller(m_options) == controller_type::mouse);
  return m_player_2_pos;
}

game_coordinat get_mouse_player_pos(const game& g)
{
  if (get_left_player_controller(g.get_options()) == controller_type::mouse)
  {
    return g.get_player_1_pos();
  }
  assert(get_right_player_controller(g.get_options()) == controller_type::mouse);
  return g.get_player_2_pos();
}

game_coordinat& get_mouse_player_pos(game& g)
{
  return g.get_mouse_player_pos();
}

chess_color get_mouse_user_player_color(const game& g)
{
  return get_mouse_user_player_color(g.get_options());
}

game_options get_options(const game& g)
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

std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
)
{
  return get_selected_pieces(g.get_pieces(), player);
}

const std::vector<sound_effect>& get_sound_effects(const game& g) noexcept
{
  return get_sound_effects(g.get_actions());
}

const piece& get_piece_at(const game& g, const square& coordinat)
{
  return get_piece_at(g.get_pieces(), coordinat);
}

piece& get_piece_at(game& g, const square& coordinat)
{
  return get_piece_at(g.get_pieces(), coordinat);
}

bool has_selected_pieces(const game& g, const chess_color player)
{
  return !get_selected_pieces(g, player).empty();
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

void game::tick(const delta_t& dt)
{
  m_control_actions.process(*this, dt);
  for (auto& p: m_pieces) p.tick(dt);
}

void unselect_all_pieces(
  game& g,
  const chess_color color
)
{
  return unselect_all_pieces(g.get_pieces(), color);
}

void toggle_left_player_color(game& g)
{
  toggle_left_player_color(g.get_options());
}
