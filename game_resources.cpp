#include "game_resources.h"

#ifndef LOGIC_ONLY

#include <QFile>
#include <cassert>

game_resources::game_resources()
{
  // Load font file
  {
    const QString filename{"arial.ttf"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_font.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load music
  // Playing sound on Travis gives thousands of error lines, which causes the
  // build to fail
  {
    const QString filename{"ninja_gods.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_ninja_gods.openFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }

  //
  m_ninja_gods.setLoop(true);
}

sf::Texture& get_black_square(game_resources& r) noexcept
{
  return r.get_square(chess_color::black);
}

sf::Font& get_font(game_resources& r) noexcept
{
  return r.get_font();
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
  const sound_effect& effect
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
