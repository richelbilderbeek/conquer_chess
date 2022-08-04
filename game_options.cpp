#include "game_options.h"

#include "controllers.h"
#include "game_view_layout.h"
#include "pieces.h"
#include <cassert>

game_options::game_options(
  const screen_coordinat& screen_size,
  const std::vector<controller>& controllers,
  const starting_position_type starting_position,
  const game_speed speed,
  const int margin_width
) : m_click_distance{0.5},
    m_controllers{controllers},
    m_game_speed{speed},
    m_left_player_color{chess_color::white},
    m_margin_width{margin_width},
    m_replayer(replay("")),
    m_screen_size{screen_size},
    m_starting_position{starting_position},
    m_music_volume{0}
{
  assert(m_click_distance > 0.0);
  assert(m_margin_width >= 0);
  assert(m_screen_size.get_x() > 0);
  assert(m_screen_size.get_y() > 0);
}

game_options create_default_game_options() noexcept
{
  return game_options(
    get_default_screen_size(),
    create_default_controllers(),
    get_default_starting_position(),
    get_default_game_speed(),
    get_default_margin_width()
  );
}

game_options create_default_game_options_with_controllers(
  const std::vector<controller>& controllers
)
{
  return game_options(
    get_default_screen_size(),
    controllers,
    get_default_starting_position(),
    get_default_game_speed(),
    get_default_margin_width()
  );
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

chess_color get_keyboard_user_player_color(const game_options& options)
{
  return get_player_color(
    options,
    get_keyboard_user_player_side(options.get_controllers())
  );
}

side get_keyboard_user_player_side(const game_options& options)
{
  return get_keyboard_user_player_side(options.get_controllers());
}

sf::Keyboard::Key get_key_for_action(const game_options& options, const side player, const int action)
{
  assert(action >= 1);
  assert(action <= 4);
  return get_key_for_action(options.get_controller(player), action);
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
  assert(has_mouse_controller(options));
  return get_player_color(
    options,
    get_mouse_user_player_side(options.get_controllers())
  );
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

bool has_keyboard_controller(const game_options& options) noexcept
{
  return has_keyboard_controller(options.get_controllers());
}

bool has_mouse_controller(const game_options& options) noexcept
{
  return has_mouse_controller(options.get_controllers());
}

void game_options::set_left_player_color(const chess_color c) noexcept
{
  m_left_player_color = c;
}

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
    const auto options{create_default_game_options()};
    assert(options.get_starting_position() == get_starting_position(options));
  }
  // game_options::get_controller
  {
    const auto options{create_default_game_options()};
    const auto left_controller{options.get_controller(side::lhs)};
    const auto right_controller{options.get_controller(side::rhs)};
    assert(left_controller != right_controller);
  }
  // game_options::get_music_volume
  {
    const auto options{create_default_game_options()};
    assert(options.get_music_volume().get_percentage() >= 0.0);
  }
  // game_options::get_music_volume
  {
    const auto options{create_default_game_options()};
    assert(get_music_volume_as_percentage(options) >= 0.0);
  }
  // game_options::get_sound_effects_volume
  {
    const auto options{create_default_game_options()};
    assert(options.get_sound_effects_volume().get_percentage() >= 0.0);
  }
  // game_options::get_starting_position
  {
    const auto options{create_default_game_options()};
    assert(options.get_starting_position() == get_starting_position(options));
  }
  // game_options::get_sontroller
  {
    auto options{create_default_game_options()};
    // TODO: this is double
    options.set_controller(create_right_keyboard_controller(side::lhs), side::lhs);
    options.set_controller(create_left_keyboard_controller(side::rhs), side::rhs);
  }
  // game_options::set_music_volume
  {
    auto options{create_default_game_options()};
    const volume v(31);
    options.set_volume(v);
    assert(options.get_music_volume() == v);
  }
  // create_default_game_options
  {
    const auto options{create_default_game_options()};
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
  // create_default_game_options_with_controllers
  {
    const auto controllers{create_two_keyboard_controllers()};
    const auto options{create_default_game_options_with_controllers(controllers)};
    assert(options.get_controllers() == controllers);
  }
  // game_options with left/keyboard player being black
  {
    auto options{create_default_game_options()};
    options.set_left_player_color(chess_color::black);
    assert(get_left_player_color(options) == chess_color::black);
    assert(get_right_player_color(options) == chess_color::white);
    assert(get_keyboard_user_player_color(options) == chess_color::black);
    assert(get_mouse_user_player_color(options) == chess_color::white);
    assert(get_player_color(options, side::lhs) == chess_color::black);
    assert(get_player_color(options, side::rhs) == chess_color::white);
  }
  // game_options with left player being black and using mouse
  {
    auto options{create_default_game_options()};
    options.set_left_player_color(chess_color::black);
    assert(get_left_player_color(options) == chess_color::black);
    assert(get_right_player_color(options) == chess_color::white);
    assert(get_keyboard_user_player_color(options) == chess_color::black);
    assert(get_mouse_user_player_color(options) == chess_color::white);
  }
#endif // NDEBUG
}

void toggle_left_player_color(game_options& options)
{
  options.set_left_player_color(
    get_other_color(options.get_left_player_color())
  );
}

bool operator==(const game_options& lhs, const game_options& rhs) noexcept
{
  return lhs.do_show_occupied() == rhs.do_show_occupied()
    && lhs.do_show_selected() == rhs.do_show_selected()
    && lhs.get_click_distance() == rhs.get_click_distance()
    && lhs.get_controllers() == rhs.get_controllers()
    && lhs.get_damage_per_chess_move() == rhs.get_damage_per_chess_move()
    && lhs.get_game_speed() == rhs.get_game_speed()
    && lhs.get_left_player_color() == rhs.get_left_player_color()
    && lhs.get_margin_width() == rhs.get_margin_width()
    && lhs.get_message_display_time_secs() == rhs.get_message_display_time_secs()
    && lhs.get_replayer() == rhs.get_replayer()
    && lhs.get_screen_size() == rhs.get_screen_size()
    && lhs.get_starting_position() == rhs.get_starting_position()
    && lhs.get_music_volume() == rhs.get_music_volume()
    && lhs.get_sound_effects_volume() == rhs.get_sound_effects_volume()
  ;
}
