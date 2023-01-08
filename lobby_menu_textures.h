#ifndef LOBBY_MENU_TEXTURES_H
#define LOBBY_MENU_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include <map>
#include "race.h"

/// The games' icon_textures
/// The raw game resources
class lobby_menu_textures
{
public:
  lobby_menu_textures();

  std::string get_head_filename(
    const race race
  ) const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this) / sizeof(m_heads.at(race::classic));
  }

  /// Get the head of a representative of the race
  sf::Texture& get_head(const race r) noexcept;


private:

  std::map<race, sf::Texture> m_heads;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
