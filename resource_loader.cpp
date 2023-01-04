#include "resource_loader.h"

#include <cassert>
#include "game_resources.h"

resource_loader::resource_loader()
  : m_index{0}
{

}

double get_progress(const resource_loader& loader) noexcept
{
  assert(loader.get_n_items() > 0);
  return static_cast<double>(loader.get_index())
    / static_cast<double>(loader.get_n_items())
  ;
}

bool resource_loader::is_done() const noexcept
{
  return m_index == get_n_items();
}

void resource_loader::process_next(game_resources& /* resources */)
{
  ++m_index;
}
