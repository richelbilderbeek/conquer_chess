#include "piece_action_textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>
#include <sstream>

piece_action_textures::piece_action_textures()
{
  for (const auto r: get_all_piece_action_types())
  {
    const std::string filename_str{get_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/piece_actions/" + filename);
    f.copy(filename);
    if (!m_textures[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}

std::string piece_action_textures::get_filename(
  const piece_action_type t
) const noexcept
{
  std::stringstream s;
  s << t << ".png";
  return s.str();
}

sf::Texture& piece_action_textures::get_texture(
  const piece_action_type t
) noexcept
{
  return m_textures[t];
}

#endif // LOGIC_ONLY
