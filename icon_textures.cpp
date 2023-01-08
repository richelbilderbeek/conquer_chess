#include "icon_textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>
#include <sstream>

icon_textures::icon_textures()
{
  for (const auto r: get_all_options_view_items())
  {
    const std::string filename_str{get_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/icons/" + filename);
    f.copy(filename);
    if (!m_textures[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}

std::string icon_textures::get_filename(
  const options_view_item item
) const noexcept
{
  std::stringstream s;
  s << item << ".png";
  return s.str();
}

sf::Texture& icon_textures::get_game_option_icon(
  const options_view_item item
) noexcept
{
  return m_textures[item];
}

#endif // LOGIC_ONLY
