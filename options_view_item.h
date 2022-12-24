#ifndef OPTIONS_VIEW_ITEM_H
#define OPTIONS_VIEW_ITEM_H

#include <string>
#include <vector>

/// A selectable item at the options_view
enum class options_view_item
{
  game_speed,
  left_color,
  left_controls,
  music_volume,
  right_color,
  right_controls,
  sound_effects_volume,
  starting_position,
};

/// Get all the options view items
std::vector<options_view_item> get_all_options_view_items() noexcept;

/// Get the item above the current one,
/// i.e. the item when the user presses up
options_view_item get_above(const options_view_item item) noexcept;

/// Get the item below the current one,
/// i.e. the item when the user presses down
options_view_item get_below(const options_view_item item) noexcept;

/// Get the item left of the current one,
/// i.e. the item when the user presses left
options_view_item get_left_of(const options_view_item item) noexcept;

/// Get the item right of the current one,
/// i.e. the item when the user presses right
options_view_item get_right_of(const options_view_item item) noexcept;

std::string to_str(const options_view_item item) noexcept;

/// Test this class and free functions
void test_options_view_item();

#endif // OPTIONS_VIEW_ITEM_H
