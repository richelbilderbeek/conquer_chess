#include "game_resources.h"

#ifndef LOGIC_ONLY

#include <cassert>
#include <optional>

std::optional<artwork_textures> game_resources::m_artwork_textures;
std::optional<board_game_textures> game_resources::m_board_game_textures;
std::optional<fonts> game_resources::m_fonts;
std::optional<input_prompt_textures> game_resources::m_input_prompt_textures;
std::optional<lobby_menu_textures> game_resources::m_lobby_menu_textures = {};
std::optional<options_controls_textures> game_resources::m_options_controls_textures = {};
std::optional<options_laws_textures> game_resources::m_options_laws_textures = {};
std::optional<options_video_and_audio_textures> game_resources::m_options_video_and_audio_textures = {};
std::optional<loading_screen_fonts> game_resources::m_loading_screen_fonts = {};
loading_screen_songs * game_resources::m_loading_screen_songs{nullptr};
std::optional<loading_screen_textures> game_resources::m_loading_screen_textures = {};
std::optional<map_textures> game_resources::m_map_textures = {};
std::optional<misc_textures> game_resources::m_misc_textures = {};
std::optional<physical_controller_textures> game_resources::m_physical_controller_type_textures = {};
std::optional<piece_action_textures> game_resources::m_piece_action_textures = {};
std::optional<piece_textures> game_resources::m_piece_textures = {};
std::optional<piece_portrait_textures> game_resources::m_piece_portrait_textures = {};
songs * game_resources::m_songs{nullptr};
sound_effects * game_resources::m_sound_effects{nullptr};
std::optional<textures> game_resources::m_textures = {};
std::optional<themba_textures> game_resources::m_themba_textures = {};

game_resources::game_resources()
{

}

sf::Texture& get_action_icon(
  piece_action_type t
) noexcept
{
  return game_resources::get().get_piece_action_textures().get_texture(t);
}

sf::Font& get_arial_font() noexcept
{
  return game_resources::get().get_fonts().get_arial_font();
}

artwork_textures& game_resources::get_artwork_textures() noexcept
{
  if (!m_artwork_textures) m_artwork_textures = artwork_textures();
  assert(m_artwork_textures);
  return m_artwork_textures.value();
}

board_game_textures& game_resources::get_board_game_textures() noexcept
{
  if (!m_board_game_textures) m_board_game_textures = board_game_textures();
  assert(m_board_game_textures);
  return m_board_game_textures.value();
}

sf::Font& get_code_squared_font() noexcept
{
  return game_resources::get().get_fonts().get_code_squared_font();
}

fonts& game_resources::get_fonts() noexcept
{
  if (!m_fonts) m_fonts = fonts();
  assert(m_fonts);
  return m_fonts.value();
}


/*
sf::Texture& get_game_option_icon(
  const options_view_item item
) noexcept
{
  return game_resources::get().get_options_menu_textures().get_texture(item);
}
*/

sf::Texture& get_game_controls_icon(
  const options_controls_item item
) noexcept
{
  return game_resources::get().get_options_controls_textures().get_texture(item);
}

sf::Texture& get_game_laws_icon(
  const options_laws_item item
) noexcept
{
  return game_resources::get().get_options_laws_textures().get_texture(item);
}

sf::Texture& get_game_video_and_audio_icon(
  const options_video_and_audio_item item
) noexcept
{
  return game_resources::get().get_options_video_and_audio_textures().get_texture(item);
}

input_prompt_textures& game_resources::get_input_prompt_textures() noexcept
{
  if (!m_input_prompt_textures) m_input_prompt_textures = input_prompt_textures();
  assert(m_input_prompt_textures);
  return m_input_prompt_textures.value();
}

lobby_menu_textures& game_resources::get_lobby_menu_textures() noexcept
{
  if (!m_lobby_menu_textures) m_lobby_menu_textures = lobby_menu_textures();
  assert(m_lobby_menu_textures);
  return m_lobby_menu_textures.value();
}

/*
options_menu_textures& game_resources::get_options_menu_textures() noexcept
{
  if (!m_options_menu_textures) m_options_menu_textures = options_menu_textures();
  assert(m_options_menu_textures);
  return m_options_menu_textures.value();
}
*/

options_controls_textures& game_resources::get_options_controls_textures() noexcept
{
  if (!m_options_controls_textures) m_options_controls_textures = options_controls_textures();
  assert(m_options_controls_textures);
  return m_options_controls_textures.value();
}

options_laws_textures& game_resources::get_options_laws_textures() noexcept
{
  if (!m_options_laws_textures) m_options_laws_textures = options_laws_textures();
  assert(m_options_laws_textures);
  return m_options_laws_textures.value();
}

options_video_and_audio_textures& game_resources::get_options_video_and_audio_textures() noexcept
{
  if (!m_options_video_and_audio_textures) m_options_video_and_audio_textures = options_video_and_audio_textures();
  assert(m_options_video_and_audio_textures);
  return m_options_video_and_audio_textures.value();
}

int game_resources::get_n_fonts() noexcept
{
  return get_fonts().get_n_fonts();
}

loading_screen_fonts& game_resources::get_loading_screen_fonts() noexcept
{
  if (!m_loading_screen_fonts) m_loading_screen_fonts = loading_screen_fonts();
  assert(m_loading_screen_fonts);
  return m_loading_screen_fonts.value();
}

loading_screen_songs& game_resources::get_loading_screen_songs() noexcept
{
  if (!m_loading_screen_songs)
  {
    m_loading_screen_songs = new loading_screen_songs();
  }
  assert(m_loading_screen_songs);
  return *m_loading_screen_songs;
}

loading_screen_textures& game_resources::get_loading_screen_textures() noexcept
{
  if (!m_loading_screen_textures) m_loading_screen_textures = loading_screen_textures();
  assert(m_loading_screen_textures);
  return m_loading_screen_textures.value();
}

sf::Texture& get_map_texture(
  const race r
) noexcept
{
  return game_resources::get().get_map_textures().get_map(r);
}
map_textures& game_resources::get_map_textures() noexcept
{
  if (!m_map_textures) m_map_textures = map_textures();
  assert(m_map_textures);
  return m_map_textures.value();
}

misc_textures& game_resources::get_misc_textures() noexcept
{
  if (!m_misc_textures) m_misc_textures = misc_textures();
  assert(m_misc_textures);
  return m_misc_textures.value();
}

int game_resources::get_n_artwork_textures() noexcept
{
  return get_artwork_textures().get_n_textures();
}

int game_resources::get_n_board_game_textures() noexcept
{
  return get_board_game_textures().get_n_textures();
}

int game_resources::get_n_input_prompt_textures() noexcept
{
  return get_input_prompt_textures().get_n_textures();
}

int game_resources::get_n_lobby_menu_textures() noexcept
{
  return get_lobby_menu_textures().get_n_textures();
}

/*
int game_resources::get_n_options_menu_textures() noexcept
{
  return get_options_menu_textures().get_n_textures();
}
*/

int game_resources::get_n_options_controls_textures() noexcept
{
  return get_options_controls_textures().get_n_textures();
}

int game_resources::get_n_options_laws_textures() noexcept
{
  return get_options_laws_textures().get_n_textures();
}

int game_resources::get_n_options_video_and_audio_textures() noexcept
{
  return get_options_video_and_audio_textures().get_n_textures();
}

int game_resources::get_n_loading_screen_fonts() noexcept
{
  return get_loading_screen_fonts().get_n_fonts();
}

int game_resources::get_n_loading_screen_songs() noexcept
{
  return get_loading_screen_songs().get_n_songs();
}

int game_resources::get_n_loading_screen_textures() noexcept
{
  return get_loading_screen_textures().get_n_textures();
}

int game_resources::get_n_map_textures() noexcept
{
  return get_map_textures().get_n_textures();
}

int game_resources::get_n_misc_textures() noexcept
{
  return get_misc_textures().get_n_textures();
}

int game_resources::get_n_physical_controller_type_textures() noexcept
{
  return get_physical_controller_textures().get_n_textures();
}

int game_resources::get_n_piece_action_textures() noexcept
{
  return get_piece_action_textures().get_n_textures();
}

int game_resources::get_n_piece_portrait_textures() noexcept
{
  return get_piece_portrait_textures().get_n_textures();
}

int game_resources::get_n_piece_textures() noexcept
{
  return get_piece_textures().get_n_textures();
}

int game_resources::get_n_songs() noexcept
{
  return get_songs().get_n_songs();
}

int game_resources::get_n_sound_effects() noexcept
{
  return get_sound_effects().get_n_sound_effects();
}

int game_resources::get_n_textures() noexcept
{
  return get_textures().get_n_textures();
}

int game_resources::get_n_themba_textures() noexcept
{
  return get_themba_textures().get_n_textures();
}

sf::Texture& get_piece_texture(
  const race r,
  const chess_color color,
  const piece_type type
)
{
  return game_resources::get().get_piece_textures().get_piece(r, color, type);
}

sf::Texture& get_piece_portrait_texture(
  const race r,
  const chess_color color,
  const piece_type type
)
{
  return game_resources::get().get_piece_portrait_textures().get_portrait(r, color, type);
}

// Lazy loading
physical_controller_textures& game_resources::get_physical_controller_textures() noexcept
{
  if (!m_physical_controller_type_textures) m_physical_controller_type_textures = physical_controller_textures();
  assert(m_physical_controller_type_textures);
  return m_physical_controller_type_textures.value();

}

piece_action_textures& game_resources::get_piece_action_textures() noexcept
{
  if (!m_piece_action_textures) m_piece_action_textures = piece_action_textures();
  assert(m_piece_action_textures);
  return m_piece_action_textures.value();
}

piece_portrait_textures& game_resources::get_piece_portrait_textures() noexcept
{
  if (!m_piece_portrait_textures) m_piece_portrait_textures = piece_portrait_textures();
  assert(m_piece_portrait_textures);
  return m_piece_portrait_textures.value();
}

piece_textures& game_resources::get_piece_textures() noexcept
{
  if (!m_piece_textures) m_piece_textures = piece_textures();
  assert(m_piece_textures);
  return m_piece_textures.value();
}

songs& game_resources::get_songs() noexcept
{
  if (!m_songs)
  {
    m_songs = new songs();
  }
  assert(m_songs);
  return *m_songs;
}

sound_effects& game_resources::get_sound_effects() noexcept
{
  if (!m_sound_effects) m_sound_effects = new sound_effects();
  assert(m_sound_effects);
  return *m_sound_effects;
}

textures& game_resources::get_textures() noexcept
{
  if (!m_textures) m_textures = textures();
  assert(m_textures);
  return m_textures.value();
}

themba_textures& game_resources::get_themba_textures() noexcept
{
  if (!m_themba_textures) m_themba_textures = themba_textures();
  assert(m_themba_textures);
  return m_themba_textures.value();
}

sf::Texture& get_strip_texture(const chess_color c) noexcept
{
  return game_resources::get().get_textures().get_strip(c);
}

sf::Texture& get_subtitle_texture() noexcept
{
  return game_resources::get().get_textures().get_subtitle();
}

sf::Texture& get_title_texture() noexcept
{
  return game_resources::get().get_textures().get_title();
}

void play_sound_effect(const message& effect)
{
  game_resources::get().get_sound_effects().play(effect);
}

#endif // LOGIC_ONLY
