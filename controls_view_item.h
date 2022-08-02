#ifndef CONTROLS_VIEW_ITEM_H
#define CONTROLS_VIEW_ITEM_H

#include <vector>

/// The items in the menu view
enum class controls_view_item
{
  type, up, right, down, left,
  action_1, action_2, action_3, action_4,
  do_action, next_action
};

std::vector<controls_view_item> get_all_controls_view_items() noexcept;

controls_view_item get_next(const controls_view_item item) noexcept;
controls_view_item get_previous(const controls_view_item item) noexcept;

void test_controls_view_item();

#endif // controls_VIEW_ITEM_H
