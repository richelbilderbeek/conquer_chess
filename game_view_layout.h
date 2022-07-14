#ifndef GAME_VIEW_LAYOUT_H
#define GAME_VIEW_LAYOUT_H

#include "game_coordinat.h"
#include "screen_coordinat.h"

/// Get the default margin width/size, in pixels
constexpr int get_default_margin_width() noexcept { return 10; }

/// The layout of the game view
///   x          x x             x x          x
///   1          2 3             4 5          6
///
/// +-------------------------------------------+
/// |                                           |
/// | 3----------+ 1-------------+ 9----------+ | <--+                       y1
/// | | units    | |             | | units    | |    |
/// | +----------4 | board       | +----------A |    +-- board_height        y2
/// |              |             |              |    |
/// | 5----------+ |             | B----------+ |    | <--+                  y3
/// | | controls | |             | | controls | |    |    +-- panel_height
/// | +----------6 |             | +----------C |    | <--+                  y4
/// |              |             |              |    |
/// | 7----------+ |             | D----------+ |    |                       y5
/// | | debug    | |             | | debug    | |    |
/// | +----------8 +-------------2 +----------E | <--+                       y6
/// |                                           |
/// +-------------------------------------------+
///
///   ^          ^ ^            ^  ^          ^
///   |          | |            |  |          |
///   +----------+ +------------+  +----------+
///    panel_width  board_width     panel_width
///
/// 1: tl_board
/// 2: br_board
/// 3: tl_units_1
/// 4: br_units_1
/// 5: tl_controls_1
/// 6: br_controls_1
/// 7: tl_debug_1
/// 8: br_debug_1
/// 9: tl_units_2
/// A: br_units_2
/// B: tl_controls_2
/// C: br_controls_2
/// D: tl_debug_2
/// E: br_debug_2
class game_view_layout
{
public:
  game_view_layout(
    const screen_coordinat& window_size = get_default_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  int get_board_height() const noexcept { return m_board_height; }
  int get_board_width() const noexcept { return m_board_width; }

  screen_coordinat get_br_board() const noexcept { return m_br_board; }
  screen_coordinat get_br_controls_1() const noexcept { return m_br_controls_1; }
  screen_coordinat get_br_controls_2() const noexcept { return m_br_controls_2; }
  screen_coordinat get_br_debug_1() const noexcept { return m_br_debug_1; }
  screen_coordinat get_br_debug_2() const noexcept { return m_br_debug_2; }
  screen_coordinat get_br_units_1() const noexcept { return m_br_units_1; }
  screen_coordinat get_br_units_2() const noexcept { return m_br_units_2; }

  int get_panel_width() const noexcept { return m_panel_width; }
  int get_panel_height() const noexcept { return m_panel_height; }

  double get_square_height() const noexcept;
  double get_square_width() const noexcept;

  screen_coordinat get_tl_board() const noexcept { return m_tl_board; }
  screen_coordinat get_tl_controls_1() const noexcept { return m_tl_controls_1; }
  screen_coordinat get_tl_controls_2() const noexcept { return m_tl_controls_2; }
  screen_coordinat get_tl_debug_1() const noexcept { return m_tl_debug_1; }
  screen_coordinat get_tl_debug_2() const noexcept { return m_tl_debug_2; }
  screen_coordinat get_tl_units_1() const noexcept { return m_tl_units_1; }
  screen_coordinat get_tl_units_2() const noexcept { return m_tl_units_2; }

  screen_coordinat get_window_size() const noexcept { return m_window_size; }

private:

  int m_board_width;
  int m_board_height;

  screen_coordinat m_br_board;
  screen_coordinat m_br_controls_1;
  screen_coordinat m_br_controls_2;
  screen_coordinat m_br_debug_1;
  screen_coordinat m_br_debug_2;
  screen_coordinat m_br_units_1;
  screen_coordinat m_br_units_2;

  int m_panel_width;
  int m_panel_height;

  screen_coordinat m_tl_board;
  screen_coordinat m_tl_controls_1;
  screen_coordinat m_tl_controls_2;
  screen_coordinat m_tl_debug_1;
  screen_coordinat m_tl_debug_2;
  screen_coordinat m_tl_units_1;
  screen_coordinat m_tl_units_2;

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
