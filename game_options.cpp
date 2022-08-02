#include "game_options.h"

#include "controllers.h"
#include "game_view_layout.h"
#include "pieces.h"
#include <cassert>

game_options::game_options(
  const screen_coordinat& screen_size,
  const starting_position_type starting_position,
  const game_speed speed,
  const int margin_width
) : m_click_distance{0.5},
    m_controllers{get_default_controllers()},
    m_game_speed{speed},
    //m_left_controller_type{controller_type::keyboard},
    m_left_player_color{chess_color::white},
    m_margin_width{margin_width},
    m_replayer(replay("")),
    //m_right_controller_type{controller_type::mouse},
    m_screen_size{screen_size},
    m_starting_position{starting_position},
    m_music_volume{0}
{
  assert(m_click_distance > 0.0);
  assert(m_margin_width >= 0);
  assert(m_screen_size.get_x() > 0);
  assert(m_screen_size.get_y() > 0);
  assert(::get_keyboard_user_player_color(*this) != ::get_mouse_user_player_color(*this));
}

bool do_show_selected(const game_options& options) noexcept
{
  return options.do_show_selected();
}

const controller& game_options::get_controller(const side& player) const
{
  if (player == side::lhs)
  {
    assert(m_controllers.size() >= 1);
    assert(m_controllers[0].get_player() == player);
    return m_controllers[0];
  }
  assert(player == side::rhs);
  assert(m_controllers.size() >= 2);
  assert(m_controllers[1].get_player() == player);
  return m_controllers[1];
}

game_options get_default_game_options()
{
  return game_options(
    get_default_screen_size(),
    get_default_starting_position(),
    get_default_game_speed(),
    get_default_margin_width()
  );
}

chess_color get_keyboard_user_player_color(const game_options& options)
{
  return get_player_color(
    options,
    get_keyboard_user_player_side(options.get_controllers())
  );

  /*
  if (options.get_left_controller_type() == controller_type::keyboard)
  {
    return get_left_player_color(options);
  }
  assert(options.get_right_controller_type() == controller_type::keyboard);
  return get_right_player_color(options);
  */
}

chess_color get_left_player_color(const game_options& options) noexcept
{
  return options.get_left_player_color();
}

controller_type get_left_player_controller(const game_options& options) noexcept
{
  assert(options.get_controllers().size() >= 1);
  assert(options.get_controllers()[0].get_player() == side::lhs);
  return options.get_controllers()[0].get_type();
}

chess_color get_mouse_user_player_color(const game_options& options)
{
  return get_player_color(
    options,
    get_mouse_user_player_side(options.get_controllers())
  );
  /*
  if (options.get_left_controller_type() == controller_type::mouse)
  {
    return get_left_player_color(options);
  }
  assert(options.get_right_controller_type() == controller_type::mouse);
  return get_right_player_color(options);
  */
}

const volume& get_music_volume(const game_options& options) noexcept
{
  return options.get_music_volume();
}

double get_music_volume_as_percentage(const game_options& options) noexcept
{
  return get_music_volume(options).get_percentage();
}

chess_color get_player_color(
  const game_options& options,
  const side player
) noexcept
{
  if (player == side::lhs) return get_left_player_color(options);
  assert(player == side::rhs);
  return get_right_player_color(options);
}

controller_type get_player_controller(
  const game_options& options,
  const side player
) noexcept
{
  if (player == side::lhs) return get_left_player_controller(options);
  assert(player == side::rhs);
  return get_right_player_controller(options);
}

chess_color get_right_player_color(const game_options& options) noexcept
{
  return get_other_color(options.get_left_player_color());
}

controller_type get_right_player_controller(const game_options& options) noexcept
{
  assert(options.get_controllers().size() >= 2);
  assert(options.get_controllers()[1].get_player() == side::rhs);
  return options.get_controllers()[1].get_type();
}

void game_options::set_left_player_color(const chess_color c) noexcept
{
  m_left_player_color = c;
}

/*
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
*/

std::vector<piece> get_starting_pieces(
  const game_options& options
) noexcept
{
  return get_starting_pieces(
    get_starting_position(options),
    get_left_player_color(options)
  );
}

starting_position_type get_starting_position(const game_options& options) noexcept
{
  return options.get_starting_position();
}

void game_options::set_controller(const controller& c, const side player)
{
  if (player == side::lhs)
  {
    assert(m_controllers.size() >= 1);
    assert(m_controllers[0].get_player() == player);
    m_controllers[0] = c;
    return;
  }
  assert(player == side::rhs);
  assert(m_controllers.size() >= 2);
  assert(m_controllers[1].get_player() == player);
  m_controllers[1] = c;
}
void test_game_options()
{
#ifndef NDEBUG
  // get_starting_position and game::get_starting_position
  {
    const auto options{get_default_game_options()};
    assert(options.get_starting_position() == get_starting_position(options));
  }
  // game_options::get_music_volume
  {
    const auto options{get_default_game_options()};
    assert(options.get_music_volume().get_percentage() >= 0.0);
  }
  // game_options::get_music_volume
  {
    const auto options{get_default_game_options()};
    assert(get_music_volume_as_percentage(options) >= 0.0);
  }
  // game_options::get_sound_effects_volume
  {
    const auto options{get_default_game_options()};
    assert(options.get_sound_effects_volume().get_percentage() >= 0.0);
  }
  // game_options::get_starting_position
  {
    const auto options{get_default_game_options()};
    assert(options.get_starting_position() == get_starting_position(options));
  }
  // game_options::set_music_volume
  {
    auto options{get_default_game_options()};
    const volume v(31);
    options.set_volume(v);
    assert(options.get_music_volume() == v);
  }
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
    assert(get_player_controller(options, side::lhs) == controller_type::keyboard);
    assert(get_player_controller(options, side::rhs) == controller_type::mouse);
    assert(get_player_color(options, side::lhs) == chess_color::white);
    assert(get_player_color(options, side::rhs) == chess_color::black);
  }
  // game_options with left/keyboard player being black
  {
    auto options{get_default_game_options()};
    options.set_left_player_color(chess_color::black);
    assert(get_left_player_color(options) == chess_color::black);
    assert(get_right_player_color(options) == chess_color::white);
    assert(get_keyboard_user_player_color(options) == chess_color::black);
    assert(get_mouse_user_player_color(options) == chess_color::white);
    assert(get_player_color(options, side::lhs) == chess_color::black);
    assert(get_player_color(options, side::rhs) == chess_color::white);
  }
  /*
  // game_options with left player using mouse (instead of keyboard)
  {
    auto options{get_default_game_options()};
    options.set_left_controller_type(controller_type::mouse);
    assert(get_keyboard_user_player_color(options) == chess_color::black);
    assert(get_mouse_user_player_color(options) == chess_color::white);
  }
  */
  // game_options with left player being black and using mouse
  {
    auto options{get_default_game_options()};
    options.set_left_player_color(chess_color::black);
    assert(get_left_player_color(options) == chess_color::black);
    assert(get_right_player_color(options) == chess_color::white);
    assert(get_keyboard_user_player_color(options) == chess_color::black);
    assert(get_mouse_user_player_color(options) == chess_color::white);
  }
  /*
  // set_right_controller_type
  {
    auto options{get_default_game_options()};
    options.set_right_controller_type(controller_type::keyboard);
    assert(options.get_left_controller_type() == controller_type::mouse);
    assert(options.get_right_controller_type() == controller_type::keyboard);
    options.set_right_controller_type(controller_type::mouse);
    assert(options.get_left_controller_type() == controller_type::keyboard);
    assert(options.get_right_controller_type() == controller_type::mouse);
  }
  */
#endif // NDEBUG
}

void toggle_left_player_color(game_options& options)
{
  options.set_left_player_color(
    get_other_color(options.get_left_player_color())
  );
}
