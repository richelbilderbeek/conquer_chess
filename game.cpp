#include "game.h"

#include "id.h"
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
    m_pieces{get_starting_pieces(options)},
    m_t{0.0}
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

void clear_piece_messages(game& g) noexcept
{
  for (auto& p: g.get_pieces()) p.clear_messages();
}

int count_control_actions(const game& g)
{
  return count_control_actions(g.get_actions());
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

void do_move_keyboard_player_piece(game& g, const square& s)
{
  assert(count_selected_units(g, get_keyboard_user_player_color(g)) == 1);
  set_keyboard_player_pos(g, s);
  assert(square(get_keyboard_player_pos(g)) == s);
  g.add_action(create_press_move_action());
  g.tick();
  assert(count_control_actions(g) == 0);
}

void do_select_and_move_keyboard_player_piece(
  game& g,
  const square& from,
  const square& to
)
{
  do_select_for_keyboard_player(g, from);
  do_move_keyboard_player_piece(g, to);
}

void do_select_for_keyboard_player(game& g, const square& s)
{
  assert(is_piece_at(g, s));
  assert(!get_piece_at(g, s).is_selected());
  set_keyboard_player_pos(g, s);
  assert(square(get_keyboard_player_pos(g)) == s);
  g.add_action(create_press_select_action());
  g.tick();
  assert(count_control_actions(g) == 0);
  assert(get_piece_at(g, s).is_selected());
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

const game_view_layout& get_layout(const game& g) noexcept
{
  return g.get_layout();
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

chess_color get_player_color(
  const game& g,
  const side player
) noexcept
{
  return get_player_color(g.get_options(), player);
}

std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
)
{
  return get_selected_pieces(g.get_pieces(), player);
}

std::vector<message> collect_messages(const game& g) noexcept
{
  const auto& pieces{g.get_pieces()};
  std::vector<message> effects;
  for (const auto& piece: pieces)
  {
    const auto& es{piece.get_sound_effects()};
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
  return get_piece_at(g.get_pieces(), coordinat);
}

const delta_t& get_time(const game& g) noexcept
{
  return g.get_time();
}

bool has_selected_pieces(const game& g, const chess_color player)
{
  return !get_selected_pieces(g, player).empty();
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

bool piece_with_id_is_at(
  game& g,
  const id& i,
  const square& s
)
{
  assert(is_piece_at(g, s));
  return get_piece_at(g, s).get_id() == i;
}

void set_keyboard_player_pos(
  game& g,
  const square& s
)
{
  const auto current_pos{get_keyboard_player_pos(g)};
  const int n_right{(s.get_x() - square(current_pos).get_x() + 8) % 8};
  const int n_down{(s.get_y() - square(current_pos).get_y() + 8) % 8};
  for (int i{0}; i!=n_right; ++i)
  {
    g.add_action(create_press_right_action());
  }
  for (int i{0}; i!=n_down; ++i)
  {
    g.add_action(create_press_down_action());
  }
  g.tick();
  assert(count_control_actions(g) == 0);
}

void game::tick(const delta_t& dt)
{
  m_control_actions.process(*this, dt);
  for (auto& p: m_pieces) p.tick(dt);

  // Keep track of the in-game time
  m_t += dt;
}

void unselect_all_pieces(
  game& g,
  const chess_color color
)
{
  return unselect_all_pieces(g.get_pieces(), color);
}

void tick_until_idle(game& g)
{
  while (!is_idle(g))
  {
    g.tick(delta_t(0.1));
  }
}

void toggle_left_player_color(game& g)
{
  toggle_left_player_color(g.get_options());
}

