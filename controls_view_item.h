#ifndef CONTROLS_VIEW_ITEM_H
#define CONTROLS_VIEW_ITEM_H

/// The items in the menu view
enum class controls_view_item
{
  start, options, about, quit
};

controls_view_item get_next(const controls_view_item there);
controls_view_item get_previous(const controls_view_item& there);

void test_controls_view_item();

#endif // controls_VIEW_ITEM_H
