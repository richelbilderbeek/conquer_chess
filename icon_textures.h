#ifndef ICON_TEXTURES_H
#define ICON_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "chess_color.h"
#include "piece_type.h"
#include "options_view_item.h"

/// The games' icon_textures
/// The raw game resources
class icon_textures
{
public:
  icon_textures();

  std::string get_filename(
    const options_view_item item
  ) const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_textures.at(options_view_item::game_speed))
    ;
  }

  /// Get an icon that accompanies a game option,
  /// to be used in the Options screen
  sf::Texture& get_game_option_icon(
    const options_view_item item
  ) noexcept;

private:

  std::map<options_view_item, sf::Texture> m_textures;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
