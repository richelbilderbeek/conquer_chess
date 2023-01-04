#include "resource_loader.h"

#include "fonts.h"
#include "game_resources.h"
#include "songs.h"
#include "sound_effects.h"
#include "textures.h"

#include <cassert>

resource_loader::resource_loader()
  : m_index{-1}
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

void resource_loader::process_next(game_resources& resources)
{
  if (is_done()) return;
  switch (m_index)
  {
    case -1:
      m_descriptor = "Start loading";
      break;
    case 0:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_fonts())
        + " fonts";
      break;
    case 1:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_songs())
        + " songs";
      break;
    case 2:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_sound_effects())
        + " sound effects";
      break;

    default:
    case 3:
      assert(m_index == 3);
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_textures())
        + " textures";
      break;
  }
  ++m_index;
}
