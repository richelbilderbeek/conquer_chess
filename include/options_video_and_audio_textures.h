#ifndef OPTIONS_VIDEO_AND_AUDIO_TEXTURES_H
#define OPTIONS_VIDEO_AND_AUDIO_TEXTURES_H

#ifndef LOGIC_ONLY

#include "options_video_and_audio_item.h"

#include <SFML/Graphics.hpp>

#include <map>

/// Textures in the Options menu.
class options_video_and_audio_textures
{
public:
  options_video_and_audio_textures();

  std::string get_filename(
    const options_video_and_audio_item item
  ) const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_textures.at(options_video_and_audio_item::game_speed))
    ;
  }

  /// Get an icon that accompanies a game option,
  /// to be used in the Options screen
  sf::Texture& get_texture(
    const options_video_and_audio_item item
  ) noexcept;

private:

  std::map<options_video_and_audio_item, sf::Texture> m_textures;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
