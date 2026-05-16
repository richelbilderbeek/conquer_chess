#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include "ccfwd.h"
#include "screen_coordinate.h"
#include "starting_position_type.h"
#include "volume.h"
#include "when_to_make_a_move_law.h"
#include "game_speed.h"

#include <iosfwd>

/// Options for the game.
///
/// Options for the game, such as speed.
/// Some options are set by the user in the \link{options_view},
/// some are variables obtained by tuning the game.
class game_options
{
public:

  game_options();

  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  auto get_click_distance() const noexcept { return m_click_distance; }

  /// Get the damage per chess move that all pieces deal
  auto get_damage_per_chess_move() const noexcept { return 1.0; }

  /// Get the game speed
  auto get_game_speed() const noexcept { return m_game_speed; }

  /// How long log messages are displayed
  double get_message_display_time_secs() const noexcept { return 5.0; }

  /// Show the debug info
  auto get_show_debug_info() const noexcept { return m_show_debug_info; }

  /// Show the squares that are actually occupied by the piecs?
  auto get_show_occupied() const noexcept { return false; }

  /// Are selected units highlighted?
  auto get_show_selected() const noexcept { return false; }

  /// Get the starting position
  starting_position_type get_starting_position() const noexcept { return m_starting_position; }

  /// Get the law when a move can be made
  when_to_make_a_move_law get_when_to_make_a_move_law() const noexcept { return m_when_to_make_a_move_law; }

  /// Get the music volume, as a percentage
  const auto& get_music_volume() const noexcept { return m_music_volume; }

  /// Get the sound effects volume
  const volume& get_sound_effects_volume() const noexcept { return m_sound_effects_volume; }

  /// Reset all values to defaults
  void reset();

  /// Set the game speed
  void set_game_speed(const game_speed speed) noexcept { m_game_speed = speed; }

  /// Set the volume, as a percentage
  void set_music_volume(const volume& v) noexcept { m_music_volume = v; }

  /// Set showing the debug info
  void set_show_debug_info(const bool b) noexcept { m_show_debug_info = b; }

  /// Set the starting position
  void set_starting_position(const starting_position_type starting_position) noexcept { m_starting_position = starting_position; }

  /// Set the sound effects volume, as a percentage
  void set_sound_effects_volume(const volume& v) noexcept { m_sound_effects_volume = v; }

  /// Set the law when to make a move
  void set_when_to_make_a_move_law(const when_to_make_a_move_law law) noexcept { m_when_to_make_a_move_law = law; }

private:


  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  double m_click_distance;

  /// The game speed
  game_speed m_game_speed;

  /// Music volume
  volume m_music_volume;

  /// Show debug info
  #ifndef NDEBUG
  bool m_show_debug_info{true};
  #else
  bool m_show_debug_info{false};
  #endif

  /// Sound effects volume
  volume m_sound_effects_volume;

  /// The starting position
  starting_position_type m_starting_position;

  /// The starting position
  when_to_make_a_move_law m_when_to_make_a_move_law;
};

/// Decrease the game speed
void decrease_game_speed(game_options& g) noexcept;

/// Decrease the music volume
void decrease_music_volume(game_options& g) noexcept;

/// Decrease the sound effects volume
void decrease_sound_effects_volume(game_options& g) noexcept;

/// Decrease the sound effects volume
void decrease_starting_position(game_options& g) noexcept;

/// Decrease the law on when moves can be made
void decrease_when_to_make_a_move(game_options& g) noexcept;

/// Are selected squares shown on-screen?
bool do_show_selected(const game_options& g) noexcept;

/// The default pixels distance for the mouse
/// for a click to connect to a piece.
constexpr double get_default_click_distance() noexcept { return 0.5; }

/// How long log messages are displayed
constexpr double get_default_message_display_time_secs() noexcept { return 5.0; }

/// The default music volume.
///
/// In percent, i.e. 20 denotes 20% of the maximum.
volume get_default_music_volume() noexcept;

/// The default sound effects volume.
///
/// In percent, i.e. 20 denotes 20% of the maximum.
volume get_default_sound_effects_volume() noexcept;

/// Get the music volume
const volume& get_music_volume(const game_options& g) noexcept;

/// Get the music volume as a percentage
double get_music_volume_as_percentage(const game_options& g) noexcept;

/// Get the sound effects volume
const volume& get_sound_effects_volume(const game_options& g) noexcept;

/// Get the starting position
starting_position_type get_starting_position(const game_options& g) noexcept;

/// Increase the game speed
void increase_game_speed(game_options& g) noexcept;

/// Increase the music volume
void increase_music_volume(game_options& g) noexcept;

/// Increase the sound effects volume
void increase_sound_effects_volume(game_options& g) noexcept;

/// Increase the starting position
void increase_starting_position(game_options& g) noexcept;

/// Increase the law on when moves can be made
void increase_when_to_make_a_move(game_options& g) noexcept;

/// Test this class and its free functions
void test_game_options();

bool operator==(const game_options& lhs, const game_options& rhs) noexcept;
bool operator!=(const game_options& lhs, const game_options& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const game_options& options) noexcept;

#endif // GAME_OPTIONS_H
