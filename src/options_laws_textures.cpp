#include "options_laws_textures.h"

#ifndef LOGIC_ONLY

#include <cassert>
#include <sstream>

options_laws_textures::options_laws_textures()
{
  for (const auto r: get_all_options_laws_items())
  {
    const std::string filename_str{get_filename(r)};
    const auto filename{
      std::string("resources/textures/options_laws/")
      + filename_str.c_str()
    };
    if (!m_textures[r].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
}

std::string options_laws_textures::get_filename(
  const options_laws_item item
) const noexcept
{
  std::stringstream s;
  s << item << ".jpg";
  return s.str();
}

sf::Texture& options_laws_textures::get_texture(
  const options_laws_item item
) noexcept
{
  return m_textures[item];
}

#endif // LOGIC_ONLY
