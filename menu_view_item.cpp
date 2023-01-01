#include "menu_view_item.h"

#include <algorithm>
#include <cassert>

#include "../magic_enum/include/magic_enum.hpp"

std::vector<menu_view_item> get_all_menu_view_items() noexcept
{
  const auto a{magic_enum::enum_values<menu_view_item>()};
  std::vector<menu_view_item> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

menu_view_item get_next(const menu_view_item item)
{
  const auto v{get_all_menu_view_items()};
  auto there{std::find(std::begin(v), std::end(v), item)};
  assert(there != std::end(v));
  if (there == std::end(v) - 1)
  {
    assert(!v.empty());
    const auto t{v.front()};
    return t;
  }
  return *(++there);
}

menu_view_item get_previous(const menu_view_item& item)
{
  const auto v{get_all_menu_view_items()};
  auto there{std::find(std::begin(v), std::end(v), item)};
  assert(there != std::end(v));
  if (there == std::begin(v))
  {
    assert(!v.empty());
    const auto t{v.back()};
    return t;
  }
  return *(--there);
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
