#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include <vector>

#include "delta_t.h"
#include "piece.h"
#include "screen_coordinat.h"
#include "starting_position_type.h"

/// Options for the game, such as speed
class game_options
{
public:
  game_options(
    const screen_coordinat& screen_size,
    const starting_position_type starting_position,
    const delta_t& dt,
    const int margin_width
  );

  /// Are selected units shown on-screen?
  auto do_show_selected() const noexcept { return false; }

  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  auto get_click_distance() const noexcept { return m_click_distance; }

  /// Get the fraction of a full move that is done per tick,
  /// where
  ///   * 0.01 means that only 1% of the full move is done
  ///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
  auto get_delta_t() const noexcept { return m_delta_t; }

  /// Get the color of the player that uses the keyboard
  auto get_keyboard_user_player_color() const noexcept { return m_keyboard_user_player_color; }

  /// Get the width of the margin in pixels
  auto get_margin_width() const noexcept { return m_margin_width; }

  /// Get the color of the player that uses the mouse
  auto get_mouse_user_player_color() const noexcept { return m_mouse_user_player_color; }

  /// Get the size of the screen in pixels
  const auto& get_screen_size() const noexcept { return m_screen_size; }

  /// Get the starting position
  auto get_starting_position() const noexcept { return m_starting_position; }

  auto get_volume() const noexcept { return m_volume; }

  /// Set the color of the player
  void set_player_color(const chess_color c) noexcept;

private:

  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  double m_click_distance;

  /// The fraction of a full move that is done per tick,
  /// where
  ///   * 0.01 means that only 1% of the full move is done
  ///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
  delta_t m_delta_t;

  /// The color of the player that uses the keyboard
  chess_color m_keyboard_user_player_color;

  /// The width of the margin in pixels
  int m_margin_width;

  /// The color of the player that uses the mouse
  chess_color m_mouse_user_player_color;

  /// The size of the screen in pixels
  screen_coordinat m_screen_size;

  /// The starting position
  starting_position_type m_starting_position;

  /// Volume in percent: 0.0 is silent, 100 is loudest
  double m_volume;
};

/// Are selected squares shown on-screen?
bool do_show_selected(const game_options& options) noexcept;

/// Get the default delta_t,
/// which is the fraction of a full move that is done per tick,
/// where
///   * 0.01 means that only 1% of the full move is done
///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
double get_default_delta_t();

game_options get_default_game_options();

/// Toggle the color of the active player
void toggle_player(game_options& options);

#endif // GAME_OPTIONS_H
