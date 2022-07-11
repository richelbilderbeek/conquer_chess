#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#include "chess_color.h"
#include "piece_type.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class game_resources
{
public:
  game_resources();

  /// Get a picture of a black/dark square
  sf::Texture& get_black_square() noexcept { return m_black_square; }

  /// Get texture of a piece
  sf::Texture& get_piece(
    const chess_color color,
    const piece_type type
  );

  /// Get a picture of a white/light square
  sf::Texture& get_white_square() noexcept { return m_white_square; }

  /// Get a font
  sf::Font &get_font() noexcept {return m_font; }

  sf::Music &get_ninja_gods() noexcept { return m_ninja_gods; }

private:
  /// Sprite of a black/dark king
  sf::Texture m_black_king;

  /// Sprite of a black/dark square
  sf::Texture m_black_square;

  /// Sprite of a white/light king
  sf::Texture m_white_king;

  /// Sprite of a white/light square
  sf::Texture m_white_square;

  /// Font
  sf::Font m_font;

  /// Ninja Gods music
  sf::Music m_ninja_gods;
};

/// Test the game resources
void test_game_resources();

#endif // GAME_RESOURCES_H
