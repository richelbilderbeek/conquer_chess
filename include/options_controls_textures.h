#ifndef OPTIONS_CONTROLS_TEXTURES_H
#define OPTIONS_CONTROLS_TEXTURES_H

#ifndef LOGIC_ONLY

#include "options_controls_item.h"

#include <SFML/Graphics.hpp>

#include <map>

/// Textures in the Options menu.
class options_controls_textures
{
public:
  options_controls_textures();

  std::string get_filename(
    const options_controls_item item
  ) const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_textures.at(options_controls_item::game_speed))
    ;
  }

  /// Get an icon that accompanies a game option,
  /// to be used in the Options screen
  sf::Texture& get_texture(
    const options_controls_item item
  ) noexcept;

private:

  std::map<options_controls_item, sf::Texture> m_textures;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
