#include "game_resources.h"

#ifndef LOGIC_ONLY

#include <QFile>
#include <cassert>
#include <optional>

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

sf::Texture& game_resources::get_piece(
  const chess_color color,
  const piece_type type
)
{
  return m_textures.get_piece(color, type);
}

sf::Texture& game_resources::get_piece_portrait(
  const chess_color color,
  const piece_type type
)
{
  return m_textures.get_piece_portrait(color, type);
}

sf::Texture& get_white_square(game_resources& r) noexcept
{
  return get_white_square(r.get_textures());
}

sf::Texture& game_resources::get_square(const chess_color color) noexcept
{
  return m_textures.get_square(color);
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
  m_sound_effects.play(effect);
}

void test_game_resources()
{
  #ifndef NDEBUG // no tests in release
  game_resources r;
  assert(r.get_square(chess_color::black).getSize().x > 0.0);
  #endif
}

#endif // LOGIC_ONLY
