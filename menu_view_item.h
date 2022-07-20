#ifndef MENU_VIEW_ITEM_H
#define MENU_VIEW_ITEM_H

/// The items in the menu view
enum class menu_view_item
{
  start, options, about, quit
};

menu_view_item get_next(const menu_view_item there);
menu_view_item get_previous(const menu_view_item& there);

void test_menu_view_item();

#endif // MENU_VIEW_ITEM_H
