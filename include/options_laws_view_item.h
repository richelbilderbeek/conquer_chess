#ifndef OPTIONS_LAWS_VIEW_ITEM_H
#define OPTIONS_LAWS_VIEW_ITEM_H

#include <iosfwd>
#include <string>
#include <vector>

/// A selectable item at in the Options menu.
///
/// A thing that can be changed in the Options menu.
enum class options_laws_view_item
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
std::vector<options_laws_view_item> get_all_options_laws_view_items() noexcept;

/// Get the item above the current one,
/// i.e. the item when the user presses up
options_laws_view_item get_above(const options_laws_view_item item) noexcept;

/// Get the item below the current one,
/// i.e. the item when the user presses down
options_laws_view_item get_below(const options_laws_view_item item) noexcept;

/// Get the item left of the current one,
/// i.e. the item when the user presses left
options_laws_view_item get_left_of(const options_laws_view_item item) noexcept;

/// Get the item right of the current one,
/// i.e. the item when the user presses right
options_laws_view_item get_right_of(const options_laws_view_item item) noexcept;

std::string to_str(const options_laws_view_item item) noexcept;

/// Test this class and free functions
void test_options_laws_view_item();

std::ostream& operator<<(std::ostream& os, const options_laws_view_item i) noexcept;

#endif // OPTIONS_LAWS_VIEW_ITEM_H
