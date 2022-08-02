#include "controls_view_item.h"

#include <algorithm>
#include <cassert>

#include "../magic_enum/include/magic_enum.hpp"

std::vector<controls_view_item> get_all_controls_view_items() noexcept
{
  const auto a{magic_enum::enum_values<controls_view_item>()};
  std::vector<controls_view_item> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

controls_view_item get_next(const controls_view_item item) noexcept
{
  const auto v{get_all_controls_view_items()};
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

controls_view_item get_previous(const controls_view_item item) noexcept
{
  const auto v{get_all_controls_view_items()};
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

void test_controls_view_item()
{
  #ifndef NDEBUG
  // Get next
  {
    for (const auto i: get_all_controls_view_items())
    {
      assert(get_next(i) != i);
    }
  }
  // Get previous
  {
    for (const auto i: get_all_controls_view_items())
    {
      assert(get_previous(i) != i);
      assert(get_previous(i) != get_next(i));
    }
  }
  // Get previous and get_next are symmetrical
  {
    for (const auto i: get_all_controls_view_items())
    {
      assert(get_next(get_previous(i)) == i);
      assert(get_previous(get_next(i)) == i);
    }
  }
  #endif
}
