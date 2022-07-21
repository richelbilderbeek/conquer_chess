#include "game_options.h"

#include "game_view_layout.h"
#include <cassert>

game_options::game_options(
  const screen_coordinat& screen_size,
  const starting_position_type starting_position,
  const game_speed speed,
  const int margin_width
) : m_game_speed{speed},
    m_left_controller_type{controller_type::keyboard},
    m_left_player_color{chess_color::white},
    m_margin_width{margin_width},
    m_right_controller_type{controller_type::mouse},
    m_screen_size{screen_size},
    m_starting_position{starting_position},
    m_volume{0}
{
  assert(m_margin_width >= 0);
  assert(m_screen_size.get_x() > 0);
  assert(m_screen_size.get_y() > 0);
  assert(::get_keyboard_user_player_color(*this) != ::get_mouse_user_player_color(*this));
}

bool do_show_selected(const game_options& options) noexcept
{
  return options.do_show_selected();
}

game_options get_default_game_options()
{
  return game_options(
    get_default_screen_size(),
    get_default_starting_position(),
    game_speed::normal,
    get_default_margin_width()
  );
}

chess_color get_keyboard_user_player_color(const game_options& options)
{
  if (options.get_left_controller_type() == controller_type::keyboard)
  {
    return get_left_player_color(options);
  }
  assert(options.get_right_controller_type() == controller_type::keyboard);
  return get_right_player_color(options);
}

chess_color get_left_player_color(const game_options& options) noexcept
{
  return options.get_left_player_color();
}

controller_type get_left_player_controller(const game_options& options) noexcept
{
  return options.get_left_controller_type();
}

chess_color get_mouse_user_player_color(const game_options& options)
{
  if (options.get_left_controller_type() == controller_type::mouse)
  {
    return get_left_player_color(options);
  }
  assert(options.get_right_controller_type() == controller_type::mouse);
  return get_right_player_color(options);
}

chess_color get_right_player_color(const game_options& options) noexcept
{
  return get_other_color(options.get_left_player_color());
}

controller_type get_right_player_controller(const game_options& options) noexcept
{
  return options.get_right_controller_type();
}

void game_options::set_left_player_color(const chess_color c) noexcept
{
  m_left_player_color = c;
}

void game_options::set_left_controller_type(const controller_type t) noexcept
{
  m_left_controller_type = t;
  if (m_right_controller_type == t)
  {
    m_right_controller_type = get_next(m_right_controller_type);
  }
}

void game_options::set_right_controller_type(const controller_type t) noexcept
{
  m_right_controller_type = t;
  if (m_left_controller_type == t)
  {
    m_left_controller_type = get_next(m_left_controller_type);
  }
}

/// Get all the pieces in the starting position type
std::vector<piece> get_starting_pieces(
  const game_options& options
) noexcept
{
  return get_starting_pieces(
    options.get_starting_position(),
    options.get_left_player_color()
  );
}

void test_game_options()
{
#ifndef NDEBUG
  // default game_options
  {
    const auto options{get_default_game_options()};
    assert(get_left_player_color(options) == chess_color::white);
    assert(get_right_player_color(options) == chess_color::black);
    assert(get_left_player_controller(options) == controller_type::keyboard);
    assert(get_right_player_controller(options) == controller_type::mouse);
    assert(get_keyboard_user_player_color(options) == chess_color::white);
    assert(get_mouse_user_player_color(options) == chess_color::black);
    assert(options.do_show_selected() || !options.do_show_selected());
    assert(options.get_click_distance() > 0.0);
  }
  // game_options with left/keyboard player being black
  {
    auto options{get_default_game_options()};
    options.set_left_player_color(chess_color::black);
    assert(get_left_player_color(options) == chess_color::black);
    assert(get_right_player_color(options) == chess_color::white);
    assert(get_keyboard_user_player_color(options) == chess_color::black);
    assert(get_mouse_user_player_color(options) == chess_color::white);
  }
  // game_options with left player using mouse (instead of keyboard)
  {
    auto options{get_default_game_options()};
    options.set_left_controller_type(controller_type::mouse);
    assert(get_keyboard_user_player_color(options) == chess_color::black);
    assert(get_mouse_user_player_color(options) == chess_color::white);
  }
  // game_options with left player being black and using mouse
  {
    auto options{get_default_game_options()};
    options.set_left_player_color(chess_color::black);
    options.set_left_controller_type(controller_type::mouse);
    assert(get_left_player_color(options) == chess_color::black);
    assert(get_right_player_color(options) == chess_color::white);
    assert(get_keyboard_user_player_color(options) == chess_color::white);
    assert(get_mouse_user_player_color(options) == chess_color::black);
  }
#endif // NDEBUG
}

void toggle_left_player_color(game_options& options)
{
  options.set_left_player_color(
    get_other_color(options.get_left_player_color())
  );
}
