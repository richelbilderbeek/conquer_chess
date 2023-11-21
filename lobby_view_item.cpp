#include "lobby_view_item.h"

#include <algorithm>
#include <cassert>

#include <magic_enum/magic_enum.hpp> // https://github.com/Neargye/magic_enum

std::vector<lobby_view_item> get_all_lobby_view_items() noexcept
{
  const auto a{magic_enum::enum_values<lobby_view_item>()};
  std::vector<lobby_view_item> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

lobby_view_item get_next(const lobby_view_item item)
{
  const auto v{get_all_lobby_view_items()};
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

lobby_view_item get_previous(const lobby_view_item& item)
{
  const auto v{get_all_lobby_view_items()};
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

void test_lobby_view_item()
{
  #ifndef NDEBUG
  // Get next
  {
    assert(get_next(lobby_view_item::color) == lobby_view_item::race);
    assert(get_next(lobby_view_item::race) == lobby_view_item::start);
    assert(get_next(lobby_view_item::start) == lobby_view_item::color);
  }
  // Get previous
  {
    for (const auto i: get_all_lobby_view_items())
    {
      assert(get_previous(get_next(i)) == i);
    }
  }
  #endif
}
