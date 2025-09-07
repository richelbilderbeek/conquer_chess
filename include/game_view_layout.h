#ifndef GAME_VIEW_LAYOUT_H
#define GAME_VIEW_LAYOUT_H

#include "ccfwd.h"
#include "screen_rect.h"
#include "game_coordinate.h"
#include "screen_coordinate.h"
#include "game_options.h"
#include "layout.h"
#include "side.h"
#include "board_layout.h"
#include "game_statistics_widget_layout.h"
#include "navigation_controls_layout.h"
#include "in_game_controls_layout.h"

#include <iosfwd>
#include <map>
#include <vector>

/// The layout of the Game dialog.
///
/// The layout of the Game dialog (\link{game_view}).
///
/// ```text
///   x          x x        x x      x x          x
///   1          2 3        4 5      6 7          8
///
/// +##----------###-----------------###----------##+
/// #                                               |
/// # Debug top bar                                 |
/// #                                               |
/// # +----------+ +-----------------+ +----------+ | y1
/// | # unit     | |                 | | unit     | |
/// | #          | |                 | |          | |
/// | #          | |                 | |          | |
/// | # info     | |                 | | info     | |
/// | +----------+ | board           | +----------+ | y2
/// | # controls | |                 | | controls | |
/// | #          | |                 | |          | |
/// | #          | |                 | |          | |
/// | #          | +-----------------+ |          | |
/// | #          |                     |          | |
/// | #          |                     |          | |
/// # +----------+                     +----------+ | y3
/// #                                               |
/// # +-------------------------------------------+ | y4
/// | #              game_statistics              | |
/// # +-------------------------------------------+ | y5
/// #                                               |
/// # +---------------------+ +-------------------+ | y6
/// | | log                 | | log               | |
/// # +---------------------+ +-------------------+ | y7
/// #                                               |
/// # +---------------------+ +-------------------+ | y8
/// | | debug               | | debug             | |
/// # +---------------------+ +-------------------+ | y9
/// #                                               |
/// +-----------------------------------------------+
///
///   ^          ^ ^            ^  ^          ^
///   |          | |            |  |          |
///   +----------+ +------------+  +----------+
///    panel_width  board_width     panel_width
///     300 pixels   remainder       300 pixels
/// ```
class game_view_layout
{
public:
  explicit game_view_layout(
    const screen_rect& r = get_default_screen_rect()
  );

  /// The entire screen
  const auto& get_background() const noexcept { return m_background; }

  const auto& get_board() const noexcept { return m_board; }

  const in_game_controls_layout& get_controls(const side player) const noexcept;

  const screen_rect& get_debug(const side player) const noexcept;

  const auto& get_game_info() const noexcept { return m_game_statistics; }

  /// The area where the frame rate (in frames per seconds) is dsplayed
  const screen_rect& get_log(const side player) const noexcept;

  const navigation_controls_layout& get_navigation_controls(const side player) const noexcept;

  /// Information about a unit
  const screen_rect& get_unit_info(const side player) const noexcept;

private:

  screen_rect m_background;
  board_layout m_board;
  std::map<side, in_game_controls_layout> m_controls;
  std::map<side, screen_rect> m_debug;
  game_options m_game_options;
  game_statistics_widget_layout m_game_statistics;
  std::map<side, screen_rect> m_log;
  std::map<side, screen_rect> m_unit_info;
};

/// Collect all the screen rects in this layout
std::vector<screen_rect> collect_screen_rects(const game_view_layout& layout);

/// Convert an in-game coordinat to an on-screen coordinat
screen_coordinate convert_to_screen_coordinate(
  const game_coordinate& coordinat,
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
game_coordinate convert_to_game_coordinate(
  const screen_coordinate& coordinat,
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

/// Test the game_view_layout class
void test_game_view_layout();

/// Convert a \link{game_view_layout} to the Well Known Text format
/// @seealso Use \link{to_wkt_file} to save the result to a file
std::string to_wkt(const game_view_layout& layout);

/// Convert a \link{game_view_layout} to a Well Known Text file
/// @seealso Use \link{to_wkt} to get the WKT as text
void to_wkt_file(const game_view_layout& layout, const std::string filename);

std::ostream& operator<<(std::ostream& os, const game_view_layout& layout) noexcept;

#endif // GAME_VIEW_LAYOUT_H
