#include "options_view_item.h"

options_view_item get_above(const options_view_item item) noexcept
{
  switch (item)
  {
    case options_view_item::game_speed: return options_view_item::right_color;
    case options_view_item::music_volume: return options_view_item::game_speed;
    case options_view_item::starting_position: return options_view_item::music_volume;
    case options_view_item::left_color: return options_view_item::starting_position;
    case options_view_item::left_controls: return options_view_item::starting_position;
    case options_view_item::right_color: return options_view_item::left_color;
    default:
    case options_view_item::right_controls: return options_view_item::left_controls;
  }
}

options_view_item get_below(const options_view_item item) noexcept
{
  switch (item)
  {
    case options_view_item::game_speed: return options_view_item::music_volume;
    case options_view_item::music_volume: return options_view_item::starting_position;
    case options_view_item::starting_position: return options_view_item::left_color;
    case options_view_item::left_color: return options_view_item::right_color;
    case options_view_item::left_controls: return options_view_item::right_controls;
    case options_view_item::right_color: return options_view_item::game_speed;
    default:
    case options_view_item::right_controls: return options_view_item::game_speed;
  }
}

options_view_item get_left_of(const options_view_item item)
 noexcept
{
  switch (item)
  {
    case options_view_item::game_speed: return options_view_item::right_color;
    case options_view_item::music_volume: return options_view_item::game_speed;
    case options_view_item::starting_position: return options_view_item::music_volume;
    case options_view_item::left_color: return options_view_item::starting_position;
    case options_view_item::left_controls: return options_view_item::left_color;
    case options_view_item::right_color: return options_view_item::left_color;
    default:
    case options_view_item::right_controls: return options_view_item::right_color;
  }
}

options_view_item get_right_of(const options_view_item item)
 noexcept
{
  switch (item)
  {
    case options_view_item::game_speed: return options_view_item::music_volume;
    case options_view_item::music_volume: return options_view_item::starting_position;
    case options_view_item::starting_position: return options_view_item::left_color;
    case options_view_item::left_color: return options_view_item::left_controls;
    case options_view_item::left_controls: return options_view_item::right_controls;
    case options_view_item::right_color: return options_view_item::right_controls;
    default:
    case options_view_item::right_controls: return options_view_item::game_speed;
  }
}
