#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include "screen_coordinat.h"

/// Options for the game, such as speed
class game_options
{
public:
  game_options(
    const screen_coordinat& screen_size,
    const double delta_t,
    const int margin_width
  );


  /// Get the fraction of a full move that is done per tick,
  /// where
  ///   * 0.01 means that only 1% of the full move is done
  ///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
  auto get_delta_t() const noexcept { return m_delta_t; }

  /// Get the width of the margin in pixels
  auto get_margin_width() const noexcept { return m_margin_width; }

  /// Get the size of the screen in pixels
  const auto& get_screen_size() const noexcept { return m_screen_size; }

private:

  /// The fraction of a full move that is done per tick,
  /// where
  ///   * 0.01 means that only 1% of the full move is done
  ///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
  double m_delta_t;

  /// The width of the margin in pixels
  const int m_margin_width;

  /// The size of the screen in pixels
  screen_coordinat m_screen_size;

};

game_options get_default_game_options();

#endif // GAME_OPTIONS_H
