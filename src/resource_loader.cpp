#include "resource_loader.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"
#include <cassert>

resource_loader::resource_loader()
  : m_index{0}
{

}

double get_progress(const resource_loader& loader) noexcept
{
  assert(loader.get_n_items() > 0);
  const double f{
    static_cast<double>(loader.get_index())
    / static_cast<double>(loader.get_n_items())
  };
  assert(f >= 0.0);
  assert(f <= 1.0);
  return f;
}

bool resource_loader::is_done() const noexcept
{
  assert(m_index <= get_n_items());
  if (m_is_done)
  {
    // Ensure all items are loaded
    assert(m_index == get_n_items());
  }
  else {
    // Ensure all items are loaded
    assert(m_index < get_n_items() && "Increase the value in resource_load::get_n_items");
  }
  return m_is_done;
}

void resource_loader::process_next()
{
  if (is_done()) return;
  game_resources& resources{game_resources::get()};

  // The loading takes place when the number of resources in that
  // group is calculated
  switch (m_index)
  {
    case 0:
      m_descriptor = "Start loading";
      break;
    case 1:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_loading_screen_fonts())
        + " loading screen fonts";
      break;
    case 2:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_loading_screen_songs())
        + " loading screen songs";
      break;
    case 3:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_loading_screen_textures())
        + " loading screen textures";
      break;
    case 4:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_fonts())
        + " fonts";
      break;
    case 5:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_songs())
        + " songs";
      break;
    case 6:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_songs())
        + " songs";
      break;
    case 7:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_sound_effects())
        + " sound effects";
      break;
    case 8:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_options_controls_textures())
        + " game options controls menu textures";
      break;
    case 9:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_options_laws_textures())
        + " game options laws menu textures";
      break;
    case 10:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_options_video_and_audio_textures())
        + " game options video and audio menu textures";
      break;
    case 11:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_map_textures())
        + " maps";
      break;
    case 12:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_physical_controller_type_textures())
        + " physical controller textures";
      break;
    case 13:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_piece_textures())
        + " piece textures";
      break;
    case 14:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_piece_action_textures())
        + " piece actions";
      break;
    case 15:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_piece_portrait_textures())
        + " piece portraits";
      break;
    case 16:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_lobby_menu_textures())
        + " lobby menu textures";
      break;
    case 17:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_input_prompt_textures())
        + " input prompt textures";
      break;
    case 18:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_themba_textures())
        + " Themba textures";
      break;
    case 19:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_artwork_textures())
        + " artwork textures";
      break;
    case 20:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_board_game_textures())
        + " board game textures";
      break;
    case 21:
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_misc_textures())
        + " miscellaneous textures";
      break;
    default:
    case 22:
      assert(m_index == 22);
      assert(m_index + 1 == get_n_items() && "Decrease the value in resource_load::get_n_items");
      m_descriptor = "Loaded "
        + std::to_string(resources.get_n_textures())
        + " textures";
      m_is_done = true;
      break;
  }
  ++m_index;
  assert(m_index <= get_n_items());
}

void test_resource_loader()
{
  resource_loader r;
  assert(get_progress(r) == 0.0);
  assert(r.get_index() == 0);
  assert(!r.is_done());
  for (int i{0}; i != r.get_n_items(); ++i)
  {
    assert(r.get_index() == i);
    r.process_next();
  }
  assert(get_progress(r) == 1.0);
  assert(r.get_index() == r.get_n_items());
  assert(r.is_done());
}


#endif // LOGIC_ONLY
