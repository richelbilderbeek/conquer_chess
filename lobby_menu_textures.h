#ifndef LOBBY_MENU_TEXTURES_H
#define LOBBY_MENU_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include <map>
#include "race.h"
#include "chess_color.h"

/// The games' icon_textures
/// The raw game resources
class lobby_menu_textures
{
public:
  lobby_menu_textures();

  /// Get an abstract chess color
  sf::Texture& get_color(const chess_color color) noexcept;

  std::string get_color_filename(
    const chess_color race
  ) const noexcept;

  /// Get the head of a representative of the race
  sf::Texture& get_head(const race r) noexcept;

  std::string get_head_filename(
    const race race
  ) const noexcept;

  /// Get a texture to state if the person is ready yes/no
  sf::Texture& get_ready(const bool is_ready) noexcept;

  std::string get_ready_filename(
    const bool is_ready
  ) const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this) / sizeof(m_heads.at(race::classic));
  }

private:

  std::map<race, sf::Texture> m_heads;

  std::map<chess_color, sf::Texture> m_color;

  std::map<bool, sf::Texture> m_ready;

};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
