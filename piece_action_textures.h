#ifndef PIECE_ACTION_TEXTURES_H
#define PIECE_ACTION_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "piece_action_type.h"

/// The games' icon_textures
/// The raw game resources
class piece_action_textures
{
public:
  piece_action_textures();

  std::string get_filename(
    const piece_action_type item
  ) const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_textures.at(piece_action_type::attack))
    ;
  }

  /// Get an icon that accompanies a game option,
  /// to be used in the Options screen
  sf::Texture& get_texture(
    const piece_action_type t
  ) noexcept;

private:

  std::map<piece_action_type, sf::Texture> m_textures;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
