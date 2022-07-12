#ifndef GAME_VIEW_LAYOUT_H
#define GAME_VIEW_LAYOUT_H

#include "game_coordinat.h"
#include "screen_coordinat.h"

/// The layout of the game view
///
/// +------------------------------+
/// |                              |
/// | 1-------------+ 3----------+ |
/// | |             | | units    | |
/// | | board       | +----------4 |
/// | |             |              |
/// | |             | 5----------+ |
/// | |             | | controls | |
/// | |             | +----------6 |
/// | |             |              |
/// | |             | 7----------+ |
/// | |             | | debug    | |
/// | +-------------2 +----------8 |
/// |                              |
/// +------------------------------+
///
/// 1: tl_board
/// 2: br_board
/// 3: tl_units
/// 4: br_units
/// 5: tl_controls
/// 6: br_controls
/// 7: tl_debug
/// 8: br_debug
class game_view_layout
{
public:
  game_view_layout(
    const screen_coordinat& window_size = screen_coordinat(1024, 576),
    const int margin_width = 10
  );

  int get_board_height() const noexcept { return m_board_height; }
  int get_board_width() const noexcept { return m_board_width; }

  screen_coordinat get_br_board() const noexcept { return m_br_board; }
  screen_coordinat get_br_controls() const noexcept { return m_br_controls; }
  screen_coordinat get_br_debug() const noexcept { return m_br_debug; }
  screen_coordinat get_br_units() const noexcept { return m_br_units; }

  double get_square_height() const noexcept;
  double get_square_width() const noexcept;

  screen_coordinat get_tl_board() const noexcept { return m_tl_board; }
  screen_coordinat get_tl_controls() const noexcept { return m_tl_controls; }
  screen_coordinat get_tl_debug() const noexcept { return m_tl_debug; }
  screen_coordinat get_tl_units() const noexcept { return m_tl_units; }

private:

  int m_board_width;
  int m_board_height;

  screen_coordinat m_br_board;
  screen_coordinat m_br_controls;
  screen_coordinat m_br_debug;
  screen_coordinat m_br_units;

  int m_panel_width;
  int m_panel_height;

  screen_coordinat m_tl_board;
  screen_coordinat m_tl_controls;
  screen_coordinat m_tl_debug;
  screen_coordinat m_tl_units;

  /// The size of the window
  screen_coordinat m_window_size;
};

/// Convert an in-game coordinat to an on-screen coordinat
screen_coordinat convert_to_screen_coordinat(
  const game_coordinat& coordinat,
  const game_view_layout& layout
);

/// Convert an on-screen coordinat to an in-game coordinat
game_coordinat convert_to_game_coordinat(
  const screen_coordinat& coordinat,
  const game_view_layout& layout
);

/// Test the game_view_layout class
void test_game_view_layout();

#endif // GAME_VIEW_LAYOUT_H
