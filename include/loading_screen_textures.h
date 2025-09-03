#ifndef LOADING_SCREEN_TEXTURES_H
#define LOADING_SCREEN_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>

#include <map>
/// The textures used in the loading screen.
///
/// This must a seperate class,
/// so that the loading screen only loads the minimal amount of
/// resources at the start.
class loading_screen_textures
{
public:
  loading_screen_textures();

  std::string get_filename(
    const int i
  ) const noexcept;

  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_all_races.at(1))
    ;
  }

  /// Get texture of a artwork with all races
  sf::Texture& get_all_races(
    const int i
  );
private:

  std::map<int, sf::Texture> m_all_races;
};

#endif // LOGIC_ONLY

#endif // LOADING_SCREEN_TEXTURES_H
