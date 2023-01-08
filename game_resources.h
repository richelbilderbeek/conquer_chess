#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#ifndef LOGIC_ONLY

#include "chess_color.h"
#include "fonts.h"
#include "options_menu_textures.h"
#include "loading_screen_fonts.h"
#include "loading_screen_songs.h"
#include "loading_screen_textures.h"
#include "map_textures.h"
#include "message.h"
#include "piece_portrait_textures.h"
#include "piece_action_textures.h"
#include "piece_textures.h"
#include "piece_type.h"
#include "piece_action_type.h"
#include "songs.h"
#include "sound_effects.h"
#include "textures.h"

#include <optional>

/// The raw game resources,
class game_resources
{
public:

  game_resources();

  // Lazy loading
  fonts& get_fonts() noexcept;

  // Lazy loading
  options_menu_textures& get_game_options_menu_textures() noexcept;

  // Lazy loading
  loading_screen_fonts& get_loading_screen_fonts() noexcept;

  // Lazy loading
  loading_screen_textures& get_loading_screen_textures() noexcept;

  // Lazy loading
  loading_screen_songs& get_loading_screen_songs() noexcept;

  int get_n_fonts() noexcept;
  int get_n_game_options_menu_textures() noexcept;
  int get_n_loading_screen_fonts() noexcept;
  int get_n_loading_screen_songs() noexcept;
  int get_n_loading_screen_textures() noexcept;
  int get_n_map_textures() noexcept;
  int get_n_piece_action_textures() noexcept;
  int get_n_piece_portrait_textures() noexcept;
  int get_n_piece_textures() noexcept;
  int get_n_songs() noexcept;
  int get_n_sound_effects() noexcept;
  int get_n_textures() noexcept;

  // Lazy loading
  map_textures& get_map_textures() noexcept;

  // Lazy loading
  piece_action_textures& get_piece_action_textures() noexcept;

  // Lazy loading
  piece_portrait_textures& get_piece_portrait_textures() noexcept;

  // Lazy loading
  piece_textures& get_piece_textures() noexcept;

  // Lazy loading
  songs& get_songs() noexcept;

  // Lazy loading
  sound_effects& get_sound_effects() noexcept;

  // Lazy loading
  textures& get_textures() noexcept;

private:

  /// Lazy loading
  static std::optional<fonts> m_fonts;

  /// Lazy loading
  static std::optional<options_menu_textures> m_game_options_menu_textures;

  /// Lazy loading
  static std::optional<loading_screen_fonts> m_loading_screen_fonts;

  /// Lazy loading
  static loading_screen_songs * m_loading_screen_songs;

  /// Lazy loading
  static std::optional<loading_screen_textures> m_loading_screen_textures;

  /// Lazy loading
  static std::optional<map_textures> m_map_textures;

  /// Lazy loading
  static std::optional<piece_action_textures> m_piece_action_textures;

  /// Lazy loading
  static std::optional<piece_textures> m_piece_textures;

  /// Lazy loading
  static std::optional<piece_portrait_textures> m_piece_portrait_textures;

  /// Lazy loading
  static songs * m_songs;

  /// Lazy loading
  static sound_effects * m_sound_effects;

  /// Lazy loading
  static std::optional<textures> m_textures;
};

sf::Texture& get_about(game_resources& r) noexcept;

sf::Texture& get_action_icon(
  game_resources& r,
  piece_action_type t
) noexcept;

/// Get the Arial font
sf::Font& get_arial_font(game_resources& r) noexcept;

/// Get a picture of a black/dark square
sf::Texture& get_black_square(game_resources& r) noexcept;

/// Get the Arial font
sf::Font& get_code_squared_font(game_resources& r) noexcept;

/// Get an icon that accompanies a game option,
/// to be used in the Options screen
sf::Texture& get_game_option_icon(
  game_resources& r,
  const options_view_item item
) noexcept;


sf::Texture& get_options(game_resources& r) noexcept;

/// Get texture of a piece
sf::Texture& get_piece(
  game_resources& gr,
  const race r,
  const chess_color color,
  const piece_type type
);

/// Get texture of a portrait of a piece
sf::Texture& get_piece_portrait(
  game_resources& gr,
  const race r,
  const chess_color color,
  const piece_type type
);

sf::Texture& get_quit(game_resources& r) noexcept;

/// Get the map for a race
sf::Texture& get_map(
  game_resources& gr,
  const race r
) noexcept;

sf::Texture& get_start(game_resources& r) noexcept;

/// Get a chess board square
sf::Texture& get_square(game_resources& r, const chess_color color) noexcept;

sf::Texture& get_strip(game_resources& r, const chess_color c) noexcept;
sf::Texture& get_subtitle(game_resources& r) noexcept;
sf::Texture& get_title(game_resources& r) noexcept;

/// Get a picture of a white/light square
sf::Texture& get_white_square(game_resources& r) noexcept;

/// Play a sound effect
void play(
  game_resources& r,
  const message& effect
);

#endif // LOGIC_ONLY

#endif // GAME_RESOURCES_H
