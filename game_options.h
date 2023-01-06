#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include "ccfwd.h"
//#include "physical_controllers.h"
//#include "physical_controller_type.h"
#include "delta_t.h"
#include "piece.h"
#include "screen_coordinat.h"
#include "starting_position_type.h"
#include "volume.h"
#include "replayer.h"
#include "game_speed.h"

#include <iosfwd>
#include <vector>

/// Options for the game, such as speed
class game_options
{
public:
  explicit game_options(
    const screen_coordinat& screen_size,
    const starting_position_type starting_position,
    const game_speed speed,
    const int margin_width
  );

  /// Show the squares that are actually occupied by the piecs?
  auto do_show_occupied() const noexcept { return false; }

  /// Are selected units highlighted?
  auto do_show_selected() const noexcept { return false; }

  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  auto get_click_distance() const noexcept { return m_click_distance; }

  /// Get the damage per chess move that all pieces deal
  auto get_damage_per_chess_move() const noexcept { return 1.0; }

  /// Get the game speed
  auto get_game_speed() const noexcept { return m_game_speed; }

  /// Get the width of the margin in pixels
  auto get_margin_width() const noexcept { return m_margin_width; }

  /// How long log messages are displayed
  double get_message_display_time_secs() const noexcept { return 5.0; }

  /// Get the replay of a match
  const auto& get_replayer() const noexcept { return m_replayer; }

  /// Get the size of the screen in pixels
  const auto& get_screen_size() const noexcept { return m_screen_size; }

  /// Get the starting position
  starting_position_type get_starting_position() const noexcept { return m_starting_position; }

  /// Get the music volume, as a percentage
  const auto& get_music_volume() const noexcept { return m_music_volume; }

  /// Get the sound effects volume
  const volume& get_sound_effects_volume() const noexcept { return m_sound_effects_volume; }

  /// Set the game speed
  void set_game_speed(const game_speed speed) noexcept { m_game_speed = speed; }

  /// Set the replayer
  void set_replayer(const replayer& r) noexcept { m_replayer = r; }

  /// Set the starting position
  void set_starting_position(const starting_position_type starting_position) noexcept { m_starting_position = starting_position; }

  /// Set the sound effects volume, as a percentage
  void set_sound_effects_volume(const volume& v) noexcept { m_sound_effects_volume = v; }

  /// Set the volume, as a percentage
  void set_volume(const volume& v) noexcept { m_music_volume = v; }

private:

  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  double m_click_distance;

  /// The game speed
  game_speed m_game_speed;

  /// The width of the margin in pixels
  int m_margin_width;

  /// Replay a match
  replayer m_replayer;

  /// The size of the screen in pixels
  screen_coordinat m_screen_size;

  /// The starting position
  starting_position_type m_starting_position;

  /// Music volume
  volume m_music_volume;

  /// Sound effects volume
  volume m_sound_effects_volume;

};

/// Create the default game options
game_options create_default_game_options() noexcept;

/// Are selected squares shown on-screen?
bool do_show_selected(const game_options& options) noexcept;

/// Get the music volume
const volume& get_music_volume(const game_options& options) noexcept;

/// Get the music volume as a percentage
double get_music_volume_as_percentage(const game_options& options) noexcept;

/// Get the sound effects volume
const volume& get_sound_effects_volume(const game_options& options) noexcept;

/// Get the starting position
starting_position_type get_starting_position(const game_options& options) noexcept;

/// Test this class and its free functions
void test_game_options();

bool operator==(const game_options& lhs, const game_options& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const game_options& options) noexcept;

#endif // GAME_OPTIONS_H
