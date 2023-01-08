#include "map_textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>
#include <sstream>

map_textures::map_textures()
{
  for (const auto r: get_all_races())
  {
    const std::string filename_str{get_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/maps/" + filename);
    f.copy(filename);
    if (!m_textures[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}

std::string map_textures::get_filename(
  const race r
) const noexcept
{
  std::stringstream s;
  s << r << "_map.png";
  return s.str();
}

sf::Texture& map_textures::get_map(
  const race race
)
{
  return m_textures[race];
}

#endif // LOGIC_ONLY
