#include "resource_loader.h"

#ifndef LOGIC_ONLY

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
        + std::to_string(resources.get_n_loading_screen_fonts())
        + " loading screen fonts";
      break;
    case 1:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_loading_screen_songs())
        + " loading screen songs";
      break;
    case 2:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_loading_screen_textures())
        + " loading screen textures";
      break;
    case 3:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_fonts())
        + " fonts";
      break;
    case 4:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_songs())
        + " songs";
      break;
    case 5:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_songs())
        + " songs";
      break;
    case 6:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_sound_effects())
        + " sound effects";
      break;
    case 7:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_game_options_menu_textures())
        + " game options menu textures";
      break;
    case 8:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_map_textures())
        + " maps";
      break;
    case 9:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_piece_textures())
        + " piece textures";
      break;
    case 10:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_piece_action_textures())
        + " piece actions";
      break;
    case 11:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_piece_portrait_textures())
        + " piece portraits";
      break;
    default:
    case 12:
      assert(m_index == 12);
      assert(m_index + 1 == get_n_items()); // If not, update get_m_items
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_textures())
        + " textures";
      break;
  }
  ++m_index;
}

#endif // LOGIC_ONLY
