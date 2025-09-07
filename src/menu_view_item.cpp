#include "menu_view_item.h"

#include <algorithm>
#include <cassert>
#include <iterator>

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<menu_view_item> get_all_menu_view_items() noexcept
{
  const auto a{magic_enum::enum_values<menu_view_item>()};
  std::vector<menu_view_item> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

menu_view_item get_down(const menu_view_item there) noexcept
{
  switch (there)
  {
    case menu_view_item::start: return menu_view_item::options;
    case menu_view_item::options: return menu_view_item::about;
    case menu_view_item::about: return menu_view_item::quit;
    case menu_view_item::quit: return menu_view_item::start;
    case menu_view_item::laws: return menu_view_item::controls;
    case menu_view_item::controls: return menu_view_item::video_and_audio;
    default:
    case menu_view_item::video_and_audio:
      assert(there == menu_view_item::video_and_audio);
      return menu_view_item::laws;
  }
}

menu_view_item get_left(const menu_view_item there) noexcept
{
  switch (there)
  {
    case menu_view_item::laws: return menu_view_item::options;
    case menu_view_item::controls: return menu_view_item::options;
    case menu_view_item::video_and_audio: return menu_view_item::options;
    default:
      return there;
  }
}

menu_view_item get_right(const menu_view_item there) noexcept
{
  switch (there)
  {
    case menu_view_item::options: return menu_view_item::controls;
    default:
      return there;
  }
}

menu_view_item get_up(const menu_view_item there) noexcept
{
  switch (there)
  {
    case menu_view_item::options: return menu_view_item::start;
    case menu_view_item::about: return menu_view_item::options;
    case menu_view_item::quit: return menu_view_item::about;
    case menu_view_item::start: return menu_view_item::quit;
    case menu_view_item::controls: return menu_view_item::laws;
    case menu_view_item::video_and_audio: return menu_view_item::controls;
    default:
    case menu_view_item::laws:
      assert(there == menu_view_item::laws);
      return menu_view_item::video_and_audio;
  }
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

bool is_in_options_submenu(const menu_view_item i) noexcept
{
  return i == menu_view_item::laws
    || i == menu_view_item::controls
    || i == menu_view_item::video_and_audio
  ;
}

void test_menu_view_item()
{
  #ifndef NDEBUG
  // get_down
  {
    assert(get_down(menu_view_item::about) == menu_view_item::quit);
    assert(get_down(menu_view_item::controls) == menu_view_item::video_and_audio);
    assert(get_down(menu_view_item::laws) == menu_view_item::controls);
    assert(get_down(menu_view_item::options) == menu_view_item::about);
    assert(get_down(menu_view_item::quit) == menu_view_item::start);
    assert(get_down(menu_view_item::start) == menu_view_item::options);
    assert(get_down(menu_view_item::video_and_audio) == menu_view_item::laws);
  }
  // get_left
  {
    assert(get_left(menu_view_item::about) == menu_view_item::about);
    assert(get_left(menu_view_item::controls) == menu_view_item::options); // Only change
    assert(get_left(menu_view_item::laws) == menu_view_item::options); // Only change
    assert(get_left(menu_view_item::options) == menu_view_item::options);
    assert(get_left(menu_view_item::quit) == menu_view_item::quit);
    assert(get_left(menu_view_item::start) == menu_view_item::start);
    assert(get_left(menu_view_item::video_and_audio) == menu_view_item::options); // Only change
  }
  // get_right
  {
    assert(get_right(menu_view_item::about) == menu_view_item::about);
    assert(get_right(menu_view_item::controls) == menu_view_item::controls);
    assert(get_right(menu_view_item::laws) == menu_view_item::laws);
    assert(get_right(menu_view_item::options) == menu_view_item::controls); // Only change
    assert(get_right(menu_view_item::quit) == menu_view_item::quit);
    assert(get_right(menu_view_item::start) == menu_view_item::start);
    assert(get_right(menu_view_item::video_and_audio) == menu_view_item::video_and_audio);
  }
  // get_up
  {
    assert(get_down(menu_view_item::about) == menu_view_item::quit);
    assert(get_down(menu_view_item::controls) == menu_view_item::video_and_audio);
    assert(get_down(menu_view_item::laws) == menu_view_item::controls);
    assert(get_down(menu_view_item::options) == menu_view_item::about);
    assert(get_down(menu_view_item::quit) == menu_view_item::start);
    assert(get_down(menu_view_item::start) == menu_view_item::options);
    assert(get_down(menu_view_item::video_and_audio) == menu_view_item::laws);
  }
  // get_next and get_previous are symmetrical
  {
    for (const auto i: get_all_menu_view_items())
    {
      assert(i == get_next(get_previous(i)));
      assert(i == get_previous(get_next(i)));
    }
  }
  // is_in_options_submenu
  {
    assert(is_in_options_submenu(menu_view_item::laws));
    assert(is_in_options_submenu(menu_view_item::controls));
    assert(is_in_options_submenu(menu_view_item::video_and_audio));
    assert(!is_in_options_submenu(menu_view_item::start));
    assert(!is_in_options_submenu(menu_view_item::options));
    assert(!is_in_options_submenu(menu_view_item::about));
    assert(!is_in_options_submenu(menu_view_item::quit));
  }
  #endif
}
