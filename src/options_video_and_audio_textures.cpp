#include "options_video_and_audio_textures.h"

#ifndef LOGIC_ONLY

#include <cassert>
#include <sstream>

options_video_and_audio_textures::options_video_and_audio_textures()
{
  for (const auto r: get_all_options_video_and_audio_items())
  {
    const std::string filename_str{get_filename(r)};
    const auto filename{
      std::string("resources/textures/options_video_and_audio/")
      + filename_str.c_str()
    };
    if (!m_textures[r].loadFromFile(filename))
    {
      auto msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
}

std::string options_video_and_audio_textures::get_filename(
  const options_video_and_audio_item item
) const noexcept
{
  std::stringstream s;
  s << item << ".jpg";
  return s.str();
}

sf::Texture& options_video_and_audio_textures::get_texture(
  const options_video_and_audio_item item
) noexcept
{
  return m_textures[item];
}

#endif // LOGIC_ONLY
