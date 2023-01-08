#include "options_menu_textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>
#include <sstream>

options_menu_textures::options_menu_textures()
{
  for (const auto r: get_all_options_view_items())
  {
    const std::string filename_str{get_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/options_menu/" + filename);
    f.copy(filename);
    if (!m_textures[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}

std::string options_menu_textures::get_filename(
  const options_view_item item
) const noexcept
{
  std::stringstream s;
  s << item << ".jpg";
  return s.str();
}

sf::Texture& options_menu_textures::get_texture(
  const options_view_item item
) noexcept
{
  return m_textures[item];
}

#endif // LOGIC_ONLY
