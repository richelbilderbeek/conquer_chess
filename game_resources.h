#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class game_resources
{
public:
  game_resources();

  /// Get a picture of Franjo
  sf::Texture &get_white_square() noexcept { return m_white_square; }

  /// Get a font
  sf::Font &get_font() noexcept {return m_font; }

  sf::Music &get_ninja_gods() noexcept { return m_ninja_gods; }

private:
  /// Sprite of a white square
  sf::Texture m_white_square;

  /// Font
  sf::Font m_font;

  /// Ninja Gods music
  sf::Music m_ninja_gods;
};

/// Test the game resources
void test_game_resources();

#endif // GAME_RESOURCES_H
