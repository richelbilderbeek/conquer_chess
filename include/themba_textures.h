#ifndef THEMBA_TEXTURES_H
#define THEMBA_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>

/// Themba Textures
class themba_textures
{
public:
  themba_textures();

  std::string get_filename(
    const int item
  ) const noexcept;

  int get_n_textures() const
  {
    return sizeof(*this)
      / sizeof(m_textures.at(1))
    ;
  }

  /// Get an icon that accompanies a game option,
  /// to be used in the Options screen
  sf::Texture& get_texture(
    const int index
  );

private:

  std::map<int, sf::Texture> m_textures;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
