#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#ifndef LOGIC_ONLY

#include "chess_color.h"
#include "piece_type.h"
#include "textures.h"
#include "sound_effects.h"
#include "message.h"

/// The raw game resources
class game_resources
{
public:
  game_resources();

  /// Get a chess board square
  sf::Texture& get_square(const chess_color color) noexcept;

  /// Get texture of a piece
  sf::Texture& get_piece(
    const chess_color color,
    const piece_type type
  );

  /// Get texture of a portrait of a piece
  sf::Texture& get_piece_portrait(
    const chess_color color,
    const piece_type type
  );

  /// Get a font
  sf::Font& get_font() noexcept {return m_font; }

  sf::Music& get_ninja_gods() noexcept { return m_ninja_gods; }

  auto& get_sound_effects() noexcept { return m_sound_effects; }


  /// Play a sound effect
  void play(
    const message& effect
  );

  textures& get_textures() noexcept { return m_textures; }

private:
  sound_effects m_sound_effects;
  textures m_textures;

  sf::Font m_font;
  sf::Music m_ninja_gods;
};

sf::Texture& get_about(game_resources& r) noexcept;

/// Get a picture of a black/dark square
sf::Texture& get_black_square(game_resources& r) noexcept;

sf::Font& get_font(game_resources& r) noexcept;

sf::Texture& get_options(game_resources& r) noexcept;

sf::Texture& get_quit(game_resources& r) noexcept;

sf::Texture& get_start(game_resources& r) noexcept;
sf::Texture& get_strip(game_resources& r, const chess_color c) noexcept;
sf::Texture& get_subtitle(game_resources& r) noexcept;
sf::Texture& get_title(game_resources& r) noexcept;

/// Get a picture of a white/light square
sf::Texture& get_white_square(game_resources& r) noexcept;

/// Test the game resources
void test_game_resources();

#endif // LOGIC_ONLY

#endif // GAME_RESOURCES_H
