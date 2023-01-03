#ifndef LOBBY_VIEW_ITEM_H
#define LOBBY_VIEW_ITEM_H

#include <vector>

/// The items in the lobby view
enum class lobby_view_item
{
  color, race, start
};

std::vector<lobby_view_item> get_all_lobby_view_items() noexcept;

lobby_view_item get_next(const lobby_view_item there);
lobby_view_item get_previous(const lobby_view_item& there);

void test_lobby_view_item();

#endif // MENU_VIEW_ITEM_H
