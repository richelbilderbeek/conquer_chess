#ifndef OPTIONS_LAWS_TEXTURES_H
#define OPTIONS_LAWS_TEXTURES_H

#ifndef LOGIC_ONLY

#include "options_laws_item.h"

#include <SFML/Graphics.hpp>

#include <map>

/// Textures in the Options menu.
class options_laws_textures
{
public:
  options_laws_textures();

  std::string get_filename(
    const options_laws_item item
  ) const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_textures.at(options_laws_item::game_speed))
    ;
  }

  /// Get an icon that accompanies a game option,
  /// to be used in the Options screen
  sf::Texture& get_texture(
    const options_laws_item item
  ) noexcept;

private:

  std::map<options_laws_item, sf::Texture> m_textures;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
