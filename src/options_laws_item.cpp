#include "options_laws_item.h"

#include "magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <iterator>

std::vector<options_laws_item> get_all_options_laws_items() noexcept
{
  const auto a{magic_enum::enum_values<options_laws_item>()};
  std::vector<options_laws_item> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

options_laws_item get_above(const options_laws_item item) noexcept
{

  switch (item)
  {
    case options_laws_item::game_speed: return options_laws_item::right_controls;
    case options_laws_item::music_volume: return options_laws_item::game_speed;
    case options_laws_item::sound_effects_volume: return options_laws_item::music_volume;
    case options_laws_item::starting_position: return options_laws_item::sound_effects_volume;
    case options_laws_item::left_controls: return options_laws_item::starting_position;
    default:
    case options_laws_item::right_controls:
      assert(item == options_laws_item::right_controls);
      return options_laws_item::left_controls;
  }
}

options_laws_item get_below(const options_laws_item item) noexcept
{
  switch (item)
  {
    case options_laws_item::game_speed: return options_laws_item::music_volume;
    case options_laws_item::music_volume: return options_laws_item::sound_effects_volume;
    case options_laws_item::sound_effects_volume: return options_laws_item::starting_position;
    case options_laws_item::starting_position: return options_laws_item::left_controls;
    case options_laws_item::left_controls: return options_laws_item::right_controls;
    default:
    case options_laws_item::right_controls:
      assert(item == options_laws_item::right_controls);
      return options_laws_item::game_speed;
  }
}

options_laws_item get_left_of(const options_laws_item item)
 noexcept
{
  switch (item)
  {
    case options_laws_item::game_speed: return options_laws_item::right_controls;
    case options_laws_item::music_volume: return options_laws_item::game_speed;
    case options_laws_item::sound_effects_volume: return options_laws_item::music_volume;
    case options_laws_item::starting_position: return options_laws_item::sound_effects_volume;
    case options_laws_item::left_controls: return options_laws_item::starting_position;
    default:
    case options_laws_item::right_controls:
      assert(item == options_laws_item::right_controls);
      return options_laws_item::game_speed;
  }
}

options_laws_item get_right_of(const options_laws_item item)
 noexcept
{
  switch (item)
  {
    case options_laws_item::game_speed: return options_laws_item::music_volume;
    case options_laws_item::music_volume: return options_laws_item::sound_effects_volume;
    case options_laws_item::sound_effects_volume: return options_laws_item::starting_position;
    case options_laws_item::starting_position: return options_laws_item::left_controls;
    case options_laws_item::left_controls: return options_laws_item::right_controls;
    default:
    case options_laws_item::right_controls:
      assert(item == options_laws_item::right_controls);
      return options_laws_item::game_speed;
  }
}

void test_options_laws_item()
{
#ifndef NDEBUG
  // get_all_options_laws_items
  {
    const auto v{get_all_options_laws_items()};
    assert(!v.empty());
  }
  // Game speed
  {
    assert(get_above(options_laws_item::game_speed) == options_laws_item::right_controls);
    assert(get_right_of(options_laws_item::game_speed) == options_laws_item::music_volume);
    assert(get_below(options_laws_item::game_speed) == options_laws_item::music_volume);
    assert(get_left_of(options_laws_item::game_speed) == options_laws_item::right_controls);
  }
  // Music volume
  {
    assert(get_above(options_laws_item::music_volume) == options_laws_item::game_speed);
    assert(get_right_of(options_laws_item::music_volume) == options_laws_item::sound_effects_volume);
    assert(get_below(options_laws_item::music_volume) == options_laws_item::sound_effects_volume);
    assert(get_left_of(options_laws_item::music_volume) == options_laws_item::game_speed);
  }
  // Starting position
  {
    assert(get_above(options_laws_item::starting_position) == options_laws_item::sound_effects_volume);
    assert(get_right_of(options_laws_item::starting_position) == options_laws_item::left_controls);
    assert(get_below(options_laws_item::starting_position) == options_laws_item::left_controls);
    assert(get_left_of(options_laws_item::starting_position) == options_laws_item::sound_effects_volume);
  }
  // Left controls
  {
    assert(get_above(options_laws_item::left_controls) == options_laws_item::starting_position);
    assert(get_right_of(options_laws_item::left_controls) == options_laws_item::right_controls);
    assert(get_below(options_laws_item::left_controls) == options_laws_item::right_controls);
    assert(get_left_of(options_laws_item::left_controls) == options_laws_item::starting_position);
  }
  // Right controls
  {
    assert(get_above(options_laws_item::right_controls) == options_laws_item::left_controls);
    assert(get_right_of(options_laws_item::right_controls) == options_laws_item::game_speed);
    assert(get_below(options_laws_item::right_controls) == options_laws_item::game_speed);
    assert(get_left_of(options_laws_item::right_controls) == options_laws_item::game_speed);
  }
  // get_above
  {
    for (const auto i: get_all_options_laws_items())
    {
      assert(get_above(i) != i);
    }
  }
  // get_below
  {
    for (const auto i: get_all_options_laws_items())
    {
      assert(get_below(i) != i);
    }
  }
  // get_left_of
  {
    for (const auto i: get_all_options_laws_items())
    {
      assert(get_left_of(i) != i);
    }
  }
  // get_right_of
  {
    for (const auto i: get_all_options_laws_items())
    {
      assert(get_right_of(i) != i);
    }
  }
  // to_str
  {
    for (const auto i: get_all_options_laws_items())
    {
      assert(!to_str(i).empty());
    }
  }
  // operator<<
  {
    for (const auto i: get_all_options_laws_items())
    {
      std::stringstream s;
      s << i;
      assert(!s.str().empty());
    }
  }
#endif // NEDBUG
}

std::string to_str(const options_laws_item t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}

std::ostream& operator<<(std::ostream& os, const options_laws_item i) noexcept
{
  os << to_str(i);
  return os;
}
