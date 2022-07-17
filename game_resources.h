#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#ifndef LOGIC_ONLY

#include "chess_color.h"
#include "piece_type.h"
#include "textures.h"

#include <SFML/Audio.hpp>

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

  /// Get texture of a piece
  void play_sound(
    const chess_color color,
    const piece_type type
  );

  textures& get_textures() noexcept {return m_textures; }

private:
  textures m_textures;

  sf::Font m_font;
  sf::Music m_ninja_gods;

  sf::Sound m_faring_into_battle;
  sf::Sound m_its_time_to_rock;
  sf::Sound m_jumping_into_battle;
  sf::Sound m_lets_rule;
  sf::Sound m_moving_forward;
  sf::Sound m_to_rule_is_to_act;

  sf::SoundBuffer m_faring_into_battle_buffer;
  sf::SoundBuffer m_its_time_to_rock_buffer;
  sf::SoundBuffer m_jumping_into_battle_buffer;
  sf::SoundBuffer m_lets_rule_buffer;
  sf::SoundBuffer m_moving_forward_buffer;
  sf::SoundBuffer m_to_rule_is_to_act_buffer;
};

/// Get a picture of a black/dark square
sf::Texture& get_black_square(game_resources& r) noexcept;

/// Get a picture of a white/light square
sf::Texture& get_white_square(game_resources& r) noexcept;

/// Test the game resources
void test_game_resources();

#endif // LOGIC_ONLY

#endif // GAME_RESOURCES_H
