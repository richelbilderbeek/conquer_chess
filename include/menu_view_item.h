#ifndef MENU_VIEW_ITEM_H
#define MENU_VIEW_ITEM_H

#include <vector>

/// The items in the Menu dialog.
enum class menu_view_item
{
  start, options, about, quit, // Main menu
  laws, controls, video_and_audio // Options submenu
};

std::vector<menu_view_item> get_all_menu_view_items() noexcept;

/// Is the item part of the options submenu?
bool is_in_options_submenu(const menu_view_item i) noexcept;

/// Get the item below this one
menu_view_item get_down(const menu_view_item there) noexcept;

/// Get the item left of this one
menu_view_item get_left(const menu_view_item there) noexcept;

/// Get the item right of this one
menu_view_item get_right(const menu_view_item there) noexcept;

/// Get the item above this one
menu_view_item get_up(const menu_view_item there) noexcept;

/// Go through the enum values
menu_view_item get_next(const menu_view_item there);

/// Go through the enum values
menu_view_item get_previous(const menu_view_item& there);

void test_menu_view_item();

#endif // MENU_VIEW_ITEM_H
