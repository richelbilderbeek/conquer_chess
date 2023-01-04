#include "game_resources.h"

#ifndef LOGIC_ONLY

#include <QFile>
#include <cassert>
#include <optional>

std::optional<fonts> game_resources::m_fonts;
songs game_resources::m_songs = songs();
std::optional<sound_effects> game_resources::m_sound_effects = {};
std::optional<textures> game_resources::m_textures = {};

game_resources::game_resources()
{
}

sf::Texture& get_black_square(game_resources& r) noexcept
{
  return r.get_square(chess_color::black);
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

sf::Texture& game_resources::get_piece(
  const chess_color color,
  const piece_type type
)
{
  return get_textures().get_piece(color, type);
}

sf::Texture& game_resources::get_piece_portrait(
  const chess_color color,
  const piece_type type
)
{
  return get_textures().get_piece_portrait(color, type);
}

songs& game_resources::get_songs() noexcept
{
  return m_songs;
}

sound_effects& game_resources::get_sound_effects() noexcept
{
  if (!m_sound_effects) m_sound_effects = sound_effects();
  assert(m_sound_effects);
  return m_sound_effects.value();
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

sf::Texture& game_resources::get_square(const chess_color color) noexcept
{
  return get_textures().get_square(color);
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

void game_resources::play(
  const message& effect
)
{
  get_sound_effects().play(effect);
}

void test_game_resources()
{
  #ifndef NDEBUG // no tests in release
  game_resources r;
  assert(r.get_square(chess_color::black).getSize().x > 0.0);
  #endif
}

#endif // LOGIC_ONLY
