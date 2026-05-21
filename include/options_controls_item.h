#ifndef OPTIONS_CONTROLS_ITEM_H
#define OPTIONS_CONTROLS_ITEM_H

#include <iosfwd>
#include <string>
#include <vector>

/// A selectable item at in the Options menu.
///
/// A thing that can be changed in the Options menu.
enum class options_controls_item
{
  game_speed,
  left_controls,
  music_volume,
  right_controls,
  sound_effects_volume,
  starting_position,
  when_to_make_a_move,
};

/// Get all the options view items
std::vector<options_controls_item> get_all_options_controls_items() noexcept;

/// Get the item above the current one,
/// i.e. the item when the user presses up
options_controls_item get_above(const options_controls_item item) noexcept;

/// Get the item below the current one,
/// i.e. the item when the user presses down
options_controls_item get_below(const options_controls_item item) noexcept;

/// Get the item left of the current one,
/// i.e. the item when the user presses left
options_controls_item get_left_of(const options_controls_item item) noexcept;

/// Get the item right of the current one,
/// i.e. the item when the user presses right
options_controls_item get_right_of(const options_controls_item item) noexcept;

std::string to_str(const options_controls_item item) noexcept;

/// Test this class and free functions
void test_options_controls_item();

std::ostream& operator<<(std::ostream& os, const options_controls_item i) noexcept;

#endif // OPTIONS_CONTROLS_ITEM_H
