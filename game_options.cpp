#include "game_options.h"

#include "physical_controllers.h"
#include "game_view_layout.h"
#include "pieces.h"

#include <cassert>
#include <iostream>
#include <sstream>

game_options::game_options(
  const screen_coordinat& screen_size,
  const starting_position_type starting_position,
  const game_speed speed,
  const int margin_width
) : m_click_distance{0.5},
    m_game_speed{speed},
    //m_left_player_color{chess_color::white},
    m_margin_width{margin_width},
    m_replayer(replay("")),
    m_screen_size{screen_size},
    m_starting_position{starting_position},
    m_music_volume{0},
    m_sound_effects_volume{10} // percent
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
    get_default_starting_position(),
    get_default_game_speed(),
    get_default_margin_width()
  );
}

bool do_show_selected(const game_options& options) noexcept
{
  return options.do_show_selected();
}

const volume& get_music_volume(const game_options& options) noexcept
{
  return options.get_music_volume();
}

double get_music_volume_as_percentage(const game_options& options) noexcept
{
  return get_music_volume(options).get_percentage();
}

const volume& get_sound_effects_volume(const game_options& options) noexcept
{
  return options.get_sound_effects_volume();
}

std::vector<piece> get_starting_pieces(
  const game_options& options
) noexcept
{
  return get_starting_pieces(
    get_starting_position(options)
  );
}

starting_position_type get_starting_position(const game_options& options) noexcept
{
  return options.get_starting_position();
}

void test_game_options()
{
#ifndef NDEBUG
  // get_starting_position and game::get_starting_position
  {
    const auto options{create_default_game_options()};
    assert(options.get_starting_position() == get_starting_position(options));
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
  // game_options::set_music_volume
  {
    auto options{create_default_game_options()};
    const volume v(31);
    options.set_volume(v);
    assert(options.get_music_volume() == v);
  }
  // 40: operator<<
  {
    const auto options{create_default_game_options()};
    std::stringstream s;
    s << options;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const game_options& lhs, const game_options& rhs) noexcept
{
  return lhs.do_show_occupied() == rhs.do_show_occupied()
    && lhs.do_show_selected() == rhs.do_show_selected()
    && lhs.get_click_distance() == rhs.get_click_distance()
    && lhs.get_damage_per_chess_move() == rhs.get_damage_per_chess_move()
    && lhs.get_game_speed() == rhs.get_game_speed()
    // && lhs.get_left_player_color() == rhs.get_left_player_color()
    && lhs.get_margin_width() == rhs.get_margin_width()
    && lhs.get_message_display_time_secs() == rhs.get_message_display_time_secs()
    && lhs.get_replayer() == rhs.get_replayer()
    && lhs.get_screen_size() == rhs.get_screen_size()
    && lhs.get_starting_position() == rhs.get_starting_position()
    && lhs.get_music_volume() == rhs.get_music_volume()
    && lhs.get_sound_effects_volume() == rhs.get_sound_effects_volume()
  ;
}

std::ostream& operator<<(std::ostream& os, const game_options& options) noexcept
{
  os
    << "click distance: " << options.get_click_distance() << '\n'
    //<< "LHS controller: " << options.get_physical_controller(side::lhs) << '\n'
    //<< "RHS controller: " << options.get_physical_controller(side::rhs) << '\n'
    << "damage per chess move: " << options.get_damage_per_chess_move() << '\n'
    << "game speed: " << options.get_game_speed() << '\n'
    //<< "LHS color: " << options.get_left_player_color() << '\n'
    << "Margin width: " << options.get_margin_width() << '\n'
    << "Message display time (sec): " << options.get_message_display_time_secs() << '\n'
    << "Replayer: " << options.get_replayer() << '\n'
    << "Screen size: " << options.get_screen_size() << '\n'
    << "Starting position: " << options.get_starting_position() << '\n'
    << "Music volume: " << options.get_music_volume() << '\n'
    << "Sound effects volume: " << options.get_sound_effects_volume()
  ;
  return os;
}
