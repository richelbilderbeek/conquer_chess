#ifndef GAME_VIEW_LAYOUT_H
#define GAME_VIEW_LAYOUT_H

#include "ccfwd.h"
#include "screen_rect.h"
#include "game_coordinat.h"
#include "screen_coordinat.h"
#include "layout.h"
#include "side.h"

#include <iosfwd>
#include <vector>


/// The layout of the game view
///
///   x          x x             x x          x
///   1          2 3             4 5          6
///
/// +-------------------------------------------+
/// |                                           |
/// | 3----------+ 1-------------+ 9----------+ | y1 -
/// | | unit     | |             | | unit     | |    | 100 pixels
/// | +----------4 | board       | +----------A | y2 -
/// | | controls | |             | | controls | |    | 100 pixels
/// | +----------6 |             | +----------C | y4 -
/// |              |             |              |
/// | 5----------+ |             | B----------+ | y5
/// | | log      | |             | | log      | |
/// | +----------6 |             | +----------C | y6
/// |              |             |              |
/// | 7----------+ |             | D----------+ | y7
/// | | debug    | |             | | debug    | |
/// | +----------8 +-------------2 +----------E | y8
/// |                                           |
/// +-------------------------------------------+
///
///   ^          ^ ^            ^  ^          ^
///   |          | |            |  |          |
///   +----------+ +------------+  +----------+
///    panel_width  board_width     panel_width
///     300 pixels   remainder       300 pixels
///
class game_view_layout
{
public:
  explicit game_view_layout(
    const screen_coordinat& window_size = get_default_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  const auto& get_board() const noexcept { return m_board; }
  const screen_rect& get_controls(const side player) const noexcept;

  /// The square that shows:
  ///   1. the action name, e.g. 'attack'
  ///   2. the action icon, e.g. an arrow that points to a cross for attack
  ///   3. the user input, e.g. 'Q' or LMB,
  ///      use \link{get_controls_key_input} to get the screen_rect
  ///      for this section
  const screen_rect& get_controls_key(const side player, const int key) const noexcept;
  screen_rect get_controls_key_input(const side player, const int key) const noexcept;
  const screen_rect& get_debug(const side player) const noexcept;
  const screen_rect& get_log(const side player) const noexcept;
  const screen_rect& get_units(const side player) const noexcept;

  screen_coordinat get_window_size() const noexcept { return m_window_size; }

private:


  screen_rect m_board;
  screen_rect m_controls_lhs;
  screen_rect m_controls_lhs_key_1;
  screen_rect m_controls_lhs_key_2;
  screen_rect m_controls_lhs_key_3;
  screen_rect m_controls_lhs_key_4;
  screen_rect m_controls_rhs;
  screen_rect m_controls_rhs_key_1;
  screen_rect m_controls_rhs_key_2;
  screen_rect m_controls_rhs_key_3;
  screen_rect m_controls_rhs_key_4;
  screen_rect m_debug_lhs;
  screen_rect m_debug_rhs;
  screen_rect m_log_lhs;
  screen_rect m_log_rhs;
  screen_rect m_units_lhs;
  screen_rect m_units_rhs;

  /// The size of the window
  screen_coordinat m_window_size;
};

/// Convert an in-game coordinat to an on-screen coordinat
screen_coordinat convert_to_screen_coordinat(
  const game_coordinat& coordinat,
  const game_view_layout& layout
);

/// Convert an in-game coordinat to an on-screen coordinat
screen_rect convert_to_screen_rect(
  const game_rect& coordinat,
  const game_view_layout& layout
);

/// Convert an in-game square to an on-screen screen rectange
screen_rect convert_to_screen_rect(
  const square& s,
  const game_view_layout& layout
);

/// Convert an on-screen coordinat to an in-game coordinat
game_coordinat convert_to_game_coordinat(
  const screen_coordinat& coordinat,
  const game_view_layout& layout
);

/// Get the height of the board in pixels
int get_board_height(const game_view_layout& layout) noexcept;

/// Get the width of the board in pixels
int get_board_width(const game_view_layout& layout) noexcept;

/// Get the height of a square
double get_square_height(const game_view_layout& layout) noexcept;

/// Get the width of a square
double get_square_width(const game_view_layout& layout) noexcept;

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const game_view_layout& layout);

/// Test the game_view_layout class
void test_game_view_layout();

std::ostream& operator<<(std::ostream& os, const game_view_layout& layout) noexcept;

#endif // GAME_VIEW_LAYOUT_H
