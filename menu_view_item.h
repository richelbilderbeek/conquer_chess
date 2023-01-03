#ifndef MENU_VIEW_ITEM_H
#define MENU_VIEW_ITEM_H

#include <vector>

/// The items in the menu view
enum class menu_view_item
{
  start, options, about, quit
};

std::vector<menu_view_item> get_all_menu_view_items() noexcept;

menu_view_item get_next(const menu_view_item there);
menu_view_item get_previous(const menu_view_item& there);

void test_menu_view_item();

#endif // MENU_VIEW_ITEM_H
