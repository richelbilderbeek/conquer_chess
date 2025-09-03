#include "artwork_textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <cassert>
#include <sstream>

artwork_textures::artwork_textures()
{
  /*
  for (int i=1 ; i!=5; ++i)
  {
    const std::string filename_str{get_filename(i)};
    const auto filename{filename_str.c_str()};
    QFile f(":/resources/textures/artwork/" + filename);
    
    if (!m_all_races[i].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
  */
}


sf::Texture& artwork_textures::get_all_races(
  const int i
)
{
  return game_resources::get().get_loading_screen_textures().get_all_races(i);
}

#endif // LOGIC_ONLY
