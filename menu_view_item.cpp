#include "menu_view_item.h"

#include <cassert>

menu_view_item get_next(const menu_view_item there)
{
  if (there == menu_view_item::start) return menu_view_item::options;
  if (there == menu_view_item::options) return menu_view_item::about;
  if (there == menu_view_item::about) return menu_view_item::quit;
  assert(there == menu_view_item::quit);
  return menu_view_item::start;
}

menu_view_item get_previous(const menu_view_item& there)
{
  if (there == menu_view_item::start) return menu_view_item::quit;
  if (there == menu_view_item::options) return menu_view_item::start;
  if (there == menu_view_item::about) return menu_view_item::options;
  assert(there == menu_view_item::quit);
  return menu_view_item::about;
}

void test_menu_view_item()
{
  #ifndef NDEBUG
  // Get next
  {
    assert(get_next(menu_view_item::start) == menu_view_item::options);
    assert(get_next(menu_view_item::options) == menu_view_item::about);
    assert(get_next(menu_view_item::about) == menu_view_item::quit);
    assert(get_next(menu_view_item::quit) == menu_view_item::start);
  }
  // Get previous
  {
    assert(get_previous(menu_view_item::start) == menu_view_item::quit);
    assert(get_previous(menu_view_item::options) == menu_view_item::start);
    assert(get_previous(menu_view_item::about) == menu_view_item::options);
    assert(get_previous(menu_view_item::quit) == menu_view_item::about);
  }
  #endif
}
