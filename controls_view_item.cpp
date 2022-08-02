#include "controls_view_item.h"

#include <cassert>

controls_view_item get_next(const controls_view_item there)
{
  if (there == controls_view_item::start) return controls_view_item::options;
  if (there == controls_view_item::options) return controls_view_item::about;
  if (there == controls_view_item::about) return controls_view_item::quit;
  assert(there == controls_view_item::quit);
  return controls_view_item::start;
}

controls_view_item get_previous(const controls_view_item& there)
{
  if (there == controls_view_item::start) return controls_view_item::quit;
  if (there == controls_view_item::options) return controls_view_item::start;
  if (there == controls_view_item::about) return controls_view_item::options;
  assert(there == controls_view_item::quit);
  return controls_view_item::about;
}

void test_controls_view_item()
{
  #ifndef NDEBUG
  // Get next
  {
    assert(get_next(controls_view_item::start) == controls_view_item::options);
    assert(get_next(controls_view_item::options) == controls_view_item::about);
    assert(get_next(controls_view_item::about) == controls_view_item::quit);
    assert(get_next(controls_view_item::quit) == controls_view_item::start);
  }
  // Get previous
  {
    assert(get_previous(controls_view_item::start) == controls_view_item::quit);
    assert(get_previous(controls_view_item::options) == controls_view_item::start);
    assert(get_previous(controls_view_item::about) == controls_view_item::options);
    assert(get_previous(controls_view_item::quit) == controls_view_item::about);
  }
  #endif
}
