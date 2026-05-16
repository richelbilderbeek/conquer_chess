#include "game_options.h"

#include "layout.h"

#include <cassert>
#include <iostream>
#include <sstream>

game_options::game_options(
) : m_click_distance{get_default_click_distance()},
    m_game_speed{get_default_game_speed()},
    m_music_volume{get_default_music_volume()},
    m_sound_effects_volume{get_default_sound_effects_volume()},
    m_starting_position{get_default_starting_position()}
{
  assert(m_click_distance > 0.0);
}

void decrease_game_speed(game_options& g) noexcept
{
  g.set_game_speed(get_previous(g.get_game_speed()));
}

void decrease_music_volume(game_options& g) noexcept
{
  g.set_music_volume(get_previous(g.get_music_volume()));
}

void decrease_sound_effects_volume(game_options& g) noexcept
{
  g.set_sound_effects_volume(get_previous(g.get_sound_effects_volume()));
}

void decrease_starting_position(game_options& g) noexcept
{
  g.set_starting_position(get_previous(g.get_starting_position()));
}

void decrease_when_to_make_a_move(game_options& g) noexcept
{
  g.set_when_to_make_a_move_law(get_previous(g.get_when_to_make_a_move_law()));
}

bool do_show_selected(const game_options& g) noexcept
{
  return g.get_show_selected();
}

volume get_default_music_volume() noexcept
{
  return volume(20);
}

volume get_default_sound_effects_volume() noexcept
{
  return volume(40);
}

const volume& get_music_volume(const game_options& g) noexcept
{
  return g.get_music_volume();
}

double get_music_volume_as_percentage(const game_options& g) noexcept
{
  return get_music_volume(g).get_percentage();
}

const volume& get_sound_effects_volume(const game_options& g) noexcept
{
  return g.get_sound_effects_volume();
}

starting_position_type get_starting_position(const game_options& options) noexcept
{
  return options.get_starting_position();
}

void increase_game_speed(game_options& g) noexcept
{
  g.set_game_speed(get_next(g.get_game_speed()));
}

void increase_music_volume(game_options& g) noexcept
{
  g.set_music_volume(get_next(g.get_music_volume()));
}

void increase_sound_effects_volume(game_options& g) noexcept
{
  g.set_sound_effects_volume(get_next(g.get_sound_effects_volume()));
}

void increase_starting_position(game_options& g) noexcept
{
  g.set_starting_position(get_next(g.get_starting_position()));
}

void increase_when_to_make_a_move(game_options& g) noexcept
{
  g.set_when_to_make_a_move_law(get_next(g.get_when_to_make_a_move_law()));
}

void game_options::reset()
{
  m_click_distance = get_default_click_distance();
  m_game_speed = get_default_game_speed();
  m_starting_position = get_default_starting_position();
  m_music_volume = get_default_music_volume();
  m_sound_effects_volume = get_default_sound_effects_volume();
}

void test_game_options()
{
#ifndef NDEBUG
  // contstructor
  {
    game_options g;
    assert(g.get_click_distance() == get_default_click_distance());
    assert(g.get_game_speed() == get_default_game_speed());
    assert(g.get_starting_position() == get_default_starting_position());
    assert(g.get_music_volume() == get_default_music_volume());
    assert(g.get_sound_effects_volume() == get_default_sound_effects_volume());
  }
  // decrease_game_speed and increase_game_speed
  {
    game_options g;
    const auto s{game_speed::normal};
    g.set_game_speed(s);
    decrease_game_speed(g);
    assert(s != g.get_game_speed());
    increase_game_speed(g);
    assert(s == g.get_game_speed());
  }
  // decrease_music_volume and increase_music_volume
  {
    game_options g;
    const auto s{volume(50)};
    g.set_music_volume(s);
    decrease_music_volume(g);
    assert(s != g.get_music_volume());
    increase_music_volume(g);
    assert(s == g.get_music_volume());
  }
  // decrease_sound_effects_volume and increase_sound_effects_volume
  {
    game_options g;
    const auto s{volume(50)};
    g.set_sound_effects_volume(s);
    decrease_sound_effects_volume(g);
    assert(s != g.get_sound_effects_volume());
    increase_sound_effects_volume(g);
    assert(s == g.get_sound_effects_volume());
  }
  // decrease_starting_position and increase_starting_position
  {
    game_options g;
    const auto s{starting_position_type::kings_only};
    g.set_starting_position(s);
    decrease_starting_position(g);
    assert(s != g.get_starting_position());
    increase_starting_position(g);
    assert(s == g.get_starting_position());
  }
  // game_options::do_show_occupied
  {
    game_options g;
    assert(!g.get_show_occupied());
  }
  // game_options::do_show_selected
  {
    game_options g;
    assert(!do_show_selected(g));
  }
  // game_options::set_music_volume
  {
    game_options g;
    const auto volume_before{get_music_volume(g)};
    assert(volume_before == get_music_volume(g));
    const volume volume_after(get_next(volume_before));
    assert(volume_before != volume_after);
    g.set_music_volume(volume_after);
    assert(get_music_volume(g) == volume_after);
  }
  // get_music_volume_as_percentage
  {
    game_options g;
    const auto volume_as_percentage{get_music_volume_as_percentage(g)};
    assert(volume_as_percentage >= 0.0);
    assert(volume_as_percentage <= 100.001);
  }
  // get_sound_effects_volume
  {
    game_options g;
    assert(get_sound_effects_volume(g) == g.get_sound_effects_volume());
  }
  // get_starting_position
  {
    game_options g;
    assert(get_starting_position(g) == g.get_starting_position());
  }
  // reset
  {
    game_options g;
    decrease_game_speed(g);
    decrease_music_volume(g);
    decrease_sound_effects_volume(g);
    decrease_starting_position(g);
    const game_options h;
    assert(g != h);
    g.reset();
    assert(g == h);
  }
  // set_game_speed
  {
    game_options g;
    for (const auto s: { game_speed::slowest, game_speed::fastest })
    {
      g.set_game_speed(s);
      assert(g.get_game_speed() == s);
    }
  }
  // set_music_volume
  {
    game_options g;
    for (const auto v: { volume(10), volume(20) })
    {
      g.set_music_volume(v);
      assert(g.get_music_volume() == v);
    }
  }
  // set_sound_effects_volume
  {
    game_options g;
    for (const auto v: { volume(10), volume(20) })
    {
      g.set_sound_effects_volume(v);
      assert(g.get_sound_effects_volume() == v);
    }
  }
  // set_starting_position
  {
    game_options g;
    for (const auto p: { starting_position_type::before_en_passant, starting_position_type::standard })
    {
      g.set_starting_position(p);
      assert(g.get_starting_position() == p);
    }
  }
  // 40: operator<<
  {
    game_options g;
    std::stringstream s;
    s << g;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const game_options& lhs, const game_options& rhs) noexcept
{
  return
    lhs.get_click_distance() == rhs.get_click_distance()
    && lhs.get_damage_per_chess_move() == rhs.get_damage_per_chess_move()
    && lhs.get_game_speed() == rhs.get_game_speed()
    && lhs.get_message_display_time_secs() == rhs.get_message_display_time_secs()
    && lhs.get_show_debug_info() == rhs.get_show_debug_info()
    && lhs.get_show_occupied() == rhs.get_show_occupied()
    && lhs.get_show_selected() == rhs.get_show_selected()
    && lhs.get_starting_position() == rhs.get_starting_position()
    && lhs.get_music_volume() == rhs.get_music_volume()
    && lhs.get_sound_effects_volume() == rhs.get_sound_effects_volume()
  ;
}

bool operator!=(const game_options& lhs, const game_options& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const game_options& options) noexcept
{
  os
    << "click distance: " << options.get_click_distance() << '\n'
    << "damage per chess move: " << options.get_damage_per_chess_move() << '\n'
    << "game speed: " << options.get_game_speed() << '\n'
    << "Message display time (sec): " << options.get_message_display_time_secs() << '\n'
    << "Starting position: " << options.get_starting_position() << '\n'
    << "Music volume: " << options.get_music_volume() << '\n'
    << "Sound effects volume: " << options.get_sound_effects_volume()
  ;
  return os;
}
