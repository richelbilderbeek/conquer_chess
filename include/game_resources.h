#ifndef GAME_RESOURCES_H
#define GAME_RESOURCES_H

#ifndef LOGIC_ONLY

#include "artwork_textures.h"
#include "board_game_textures.h"
#include "chess_color.h"
#include "fonts.h"
#include "lobby_menu_textures.h"
#include "options_controls_textures.h"
#include "options_laws_textures.h"
#include "options_video_and_audio_textures.h"
#include "loading_screen_fonts.h"
#include "loading_screen_songs.h"
#include "loading_screen_textures.h"
#include "misc_textures.h"
#include "map_textures.h"
#include "message.h"
#include "physical_controller_textures.h"
#include "piece_portrait_textures.h"
#include "piece_action_textures.h"
#include "piece_textures.h"
#include "piece_type.h"
#include "piece_action_type.h"
#include "songs.h"
#include "sound_effects.h"
#include "textures.h"
#include "themba_textures.h"
#include "input_prompt_textures.h"
#include <optional>



/// The raw game resources.
///
/// There must be only one game_resources.
///
/// Use \link{game_resources::get} to get a handle to this class.
/// The resources are loaded by \class{resource_loader}
class game_resources
{
public:

  /// This class is a Singleton
  game_resources(const game_resources&) = delete;
  game_resources& operator=(const game_resources&) = delete;
  game_resources(game_resources &&) = delete;
  game_resources& operator=(game_resources&&) = delete;
  static auto& get(){
      static game_resources r;
      return r;
  }

  artwork_textures& get_artwork_textures() noexcept;

  board_game_textures& get_board_game_textures() noexcept;

  fonts& get_fonts() noexcept;

  input_prompt_textures& get_input_prompt_textures() noexcept;

  int get_n_fonts() noexcept;
  int get_n_artwork_textures() noexcept;
  int get_n_board_game_textures() noexcept;
  int get_n_input_prompt_textures() noexcept;
  int get_n_loading_screen_fonts() noexcept;
  int get_n_loading_screen_songs() noexcept;
  int get_n_loading_screen_textures() noexcept;
  int get_n_lobby_menu_textures() noexcept;
  int get_n_map_textures() noexcept;
  int get_n_misc_textures() noexcept;
  //int get_n_options_menu_textures() noexcept;
  int get_n_options_controls_textures() noexcept;
  int get_n_options_laws_textures() noexcept;
  int get_n_options_video_and_audio_textures() noexcept;
  int get_n_physical_controller_type_textures() noexcept;
  int get_n_piece_action_textures() noexcept;
  int get_n_piece_portrait_textures() noexcept;
  int get_n_piece_textures() noexcept;
  int get_n_songs() noexcept;
  int get_n_sound_effects() noexcept;
  int get_n_themba_textures() noexcept;
  int get_n_textures() noexcept;

  loading_screen_fonts& get_loading_screen_fonts() noexcept;

  loading_screen_songs& get_loading_screen_songs() noexcept;

  loading_screen_textures& get_loading_screen_textures() noexcept;

  lobby_menu_textures& get_lobby_menu_textures() noexcept;

  map_textures& get_map_textures() noexcept;

  misc_textures& get_misc_textures() noexcept;

  options_controls_textures& get_options_controls_textures() noexcept;
  options_laws_textures& get_options_laws_textures() noexcept;
  options_video_and_audio_textures& get_options_video_and_audio_textures() noexcept;

  physical_controller_textures& get_physical_controller_textures() noexcept;

  piece_action_textures& get_piece_action_textures() noexcept;

  piece_portrait_textures& get_piece_portrait_textures() noexcept;

  piece_textures& get_piece_textures() noexcept;

  songs& get_songs() noexcept;

  sound_effects& get_sound_effects() noexcept;

  textures& get_textures() noexcept;

  themba_textures& get_themba_textures() noexcept;

private:

  /// This is a Singleton, as there must be only one instance of this.
  game_resources();

  static std::optional<artwork_textures> m_artwork_textures;
  static std::optional<board_game_textures> m_board_game_textures;
  static std::optional<fonts> m_fonts;
  static std::optional<loading_screen_fonts> m_loading_screen_fonts;
  static std::optional<input_prompt_textures> m_input_prompt_textures;
  static loading_screen_songs * m_loading_screen_songs;
  static std::optional<loading_screen_textures> m_loading_screen_textures;
  static std::optional<lobby_menu_textures> m_lobby_menu_textures;
  static std::optional<map_textures> m_map_textures;
  static std::optional<misc_textures> m_misc_textures;
  static std::optional<options_controls_textures> m_options_controls_textures;
  static std::optional<options_laws_textures> m_options_laws_textures;
  static std::optional<options_video_and_audio_textures> m_options_video_and_audio_textures;
  static std::optional<physical_controller_textures> m_physical_controller_type_textures;
  static std::optional<piece_action_textures> m_piece_action_textures;
  static std::optional<piece_portrait_textures> m_piece_portrait_textures;
  static std::optional<piece_textures> m_piece_textures;
  static songs * m_songs;
  static sound_effects * m_sound_effects;
  static std::optional<textures> m_textures;
  static std::optional<themba_textures> m_themba_textures;
};


sf::Texture& get_about_texture() noexcept;

sf::Texture& get_action_icon(
  piece_action_type t
) noexcept;

/// Get the Arial font
sf::Font& get_arial_font() noexcept;

/// Get the Code Squared font
sf::Font& get_code_squared_font() noexcept;

/// Get an icon that accompanies a Controls option,
/// to be used in the Options screen
sf::Texture& get_game_controls_icon(
  const options_controls_item item
) noexcept;

/// Get an icon that accompanies a Laws option,
/// to be used in the Options screen
sf::Texture& get_game_laws_icon(
  const options_laws_item item
) noexcept;

/// Get an icon that accompanies a Video and Audio option,
/// to be used in the Options screen
sf::Texture& get_game_video_and_audio_icon(
  const options_video_and_audio_item item
) noexcept;


sf::Texture& get_options_texture(game_resources& r) noexcept;

/// Get texture of a piece
sf::Texture& get_piece_texture(
  const race r,
  const chess_color color,
  const piece_type type
);

/// Get texture of a portrait of a piece
sf::Texture& get_piece_portrait_texture(
  const race r,
  const chess_color color,
  const piece_type type
);

sf::Texture& get_quit_texture(game_resources& r) noexcept;

/// Get the map for a race
sf::Texture& get_map_texture(
  const race r
) noexcept;

sf::Texture& get_start_texture() noexcept;

sf::Texture& get_strip_texture(const chess_color c) noexcept;
sf::Texture& get_subtitle_texture() noexcept;
sf::Texture& get_title_texture() noexcept;

/// Play a sound effect
void play_sound_effect(const message& effect);

#endif // LOGIC_ONLY

#endif // GAME_RESOURCES_H
