#ifndef GAME_OPTIONS_H
#define GAME_OPTIONS_H

#include <vector>

#include "piece.h"
#include "screen_coordinat.h"

/// Options for the game, such as speed
class game_options
{
public:
  game_options(
    const screen_coordinat& screen_size,
    const std::vector<piece>& starting_pieces,
    const double delta_t,
    const int margin_width
  );


  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  auto get_click_distance() const noexcept { return m_click_distance; }

  /// Get the fraction of a full move that is done per tick,
  /// where
  ///   * 0.01 means that only 1% of the full move is done
  ///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
  auto get_delta_t() const noexcept { return m_delta_t; }

  /// Get the width of the margin in pixels
  auto get_margin_width() const noexcept { return m_margin_width; }

  /// Get the size of the screen in pixels
  const auto& get_screen_size() const noexcept { return m_screen_size; }

  /// Get the starting pieces
  const auto& get_starting_pieces() const noexcept { return m_starting_pieces; }

private:


  /// Get the distance the mouse must be maximally in
  /// for a click to connect to a piece
  double m_click_distance;

  /// The fraction of a full move that is done per tick,
  /// where
  ///   * 0.01 means that only 1% of the full move is done
  ///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
  double m_delta_t;

  /// The width of the margin in pixels
  const int m_margin_width;

  /// The size of the screen in pixels
  screen_coordinat m_screen_size;


  /// The starting pieces
  std::vector<piece> m_starting_pieces;

};

/// Get the default delta_t,
/// which is the fraction of a full move that is done per tick,
/// where
///   * 0.01 means that only 1% of the full move is done
///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
double get_default_delta_t();

game_options get_default_game_options();

#endif // GAME_OPTIONS_H
