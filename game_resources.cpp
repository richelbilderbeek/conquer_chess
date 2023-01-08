#include "game_resources.h"

#ifndef LOGIC_ONLY

#include <QFile>
#include <cassert>
#include <optional>

std::optional<fonts> game_resources::m_fonts;
std::optional<lobby_menu_textures> game_resources::m_lobby_menu_textures = {};
std::optional<options_menu_textures> game_resources::m_options_menu_textures = {};
std::optional<loading_screen_fonts> game_resources::m_loading_screen_fonts = {};
loading_screen_songs * game_resources::m_loading_screen_songs{nullptr};
std::optional<loading_screen_textures> game_resources::m_loading_screen_textures = {};
std::optional<map_textures> game_resources::m_map_textures = {};
std::optional<piece_action_textures> game_resources::m_piece_action_textures = {};
std::optional<piece_textures> game_resources::m_piece_textures = {};
std::optional<piece_portrait_textures> game_resources::m_piece_portrait_textures = {};
songs * game_resources::m_songs{nullptr};
sound_effects * game_resources::m_sound_effects{nullptr};
std::optional<textures> game_resources::m_textures = {};

game_resources::game_resources()
{

}

sf::Texture& get_action_icon(
  game_resources& gr,
  piece_action_type t
) noexcept
{
  return gr.get_piece_action_textures().get_texture(t);
}


sf::Texture& get_black_square(game_resources& r) noexcept
{
  return get_square(r, chess_color::black);
}

sf::Font& get_arial_font(game_resources& r) noexcept
{
  return r.get_fonts().get_arial_font();
}

sf::Font& get_code_squared_font(game_resources& r) noexcept
{
  return r.get_fonts().get_code_squared_font();
}

fonts& game_resources::get_fonts() noexcept
{
  if (!m_fonts) m_fonts = fonts();
  assert(m_fonts);
  return m_fonts.value();
}


sf::Texture& get_game_option_icon(
  game_resources& gr,
  const options_view_item item
) noexcept
{
  return gr.get_options_menu_textures().get_texture(item);
}

lobby_menu_textures& game_resources::get_lobby_menu_textures() noexcept
{
  if (!m_lobby_menu_textures) m_lobby_menu_textures = lobby_menu_textures();
  assert(m_lobby_menu_textures);
  return m_lobby_menu_textures.value();
}

options_menu_textures& game_resources::get_options_menu_textures() noexcept
{
  if (!m_options_menu_textures) m_options_menu_textures = options_menu_textures();
  assert(m_options_menu_textures);
  return m_options_menu_textures.value();
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

sf::Texture& get_map(
  game_resources& gr,
  const race r
) noexcept
{
  return gr.get_map_textures().get_map(r);
}
map_textures& game_resources::get_map_textures() noexcept
{
  if (!m_map_textures) m_map_textures = map_textures();
  assert(m_map_textures);
  return m_map_textures.value();
}

int game_resources::get_n_lobby_menu_textures() noexcept
{
  return get_lobby_menu_textures().get_n_textures();
}

int game_resources::get_n_options_menu_textures() noexcept
{
  return get_options_menu_textures().get_n_textures();
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

sf::Texture& get_piece(
  game_resources& gr,
  const race r,
  const chess_color color,
  const piece_type type
)
{
  return gr.get_piece_textures().get_piece(r, color, type);
}

sf::Texture& get_piece_portrait(
  game_resources& gr,
  const race r,
  const chess_color color,
  const piece_type type
)
{
  return gr.get_piece_portrait_textures().get_portrait(r, color, type);
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

sf::Texture& get_white_square(game_resources& r) noexcept
{
  return get_white_square(r.get_textures());
}

sf::Texture& get_square(game_resources& r, const chess_color color) noexcept
{
  return r.get_textures().get_square(color);
}

sf::Texture& get_strip(game_resources& r, const chess_color c) noexcept
{
  return r.get_textures().get_strip(c);
}

sf::Texture& get_subtitle(game_resources& r) noexcept
{
  return r.get_textures().get_subtitle();
}

sf::Texture& get_title(game_resources& r) noexcept
{
  return r.get_textures().get_title();
}

void play(
  game_resources& r,
  const message& effect
)
{
  r.get_sound_effects().play(effect);
}

#endif // LOGIC_ONLY
