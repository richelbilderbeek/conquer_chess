#include "menu_view_options_item.h"

#include <algorithm>
#include <cassert>
#include <iterator>

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<menu_view_options_item> get_all_menu_view_options_items() noexcept
{
  const auto a{magic_enum::enum_values<menu_view_options_item>()};
  std::vector<menu_view_options_item> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

menu_view_options_item get_next(const menu_view_options_item item)
{
  const auto v{get_all_menu_view_options_items()};
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

menu_view_options_item get_previous(const menu_view_options_item& item)
{
  const auto v{get_all_menu_view_options_items()};
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

void test_menu_view_options_item()
{
  #ifndef NDEBUG
  // Get next
  {
    assert(get_next(menu_view_options_item::controls) == menu_view_options_item::laws);
  }
  // Get previous
  {
    assert(get_previous(menu_view_options_item::sound_and_video) == menu_view_options_item::laws);
  }
  #endif
}
