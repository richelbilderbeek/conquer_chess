#ifndef GAME_VIEW_LAYOUT_H
#define GAME_VIEW_LAYOUT_H

#include "screen_coordinat.h"

/// The layout of the game view
///
/// +------------------------------+
/// |                              |
/// | 1-------------+ 2----------+ |
/// | |             | |          | |
/// | |             | |          | |
/// | |             | |          | |
/// | |             | |          | |
/// | |             | |          | |
/// | |             | |          | |
/// | +-------------3 +----------4 |
/// |                              |
/// +------------------------------+
///
/// 1: tl_board
/// 2: tl_side
/// 3: br_board
/// 4: br_side
class game_view_layout
{
public:
  game_view_layout(
    const screen_coordinat& window_size
  );
  screen_coordinat get_br_board() const noexcept { return m_br_board; }
  screen_coordinat get_br_side() const noexcept { return m_br_side; }

  double get_square_height() const noexcept;
  double get_square_width() const noexcept;

  screen_coordinat get_tl_board() const noexcept { return m_tl_board; }
  screen_coordinat get_tl_side() const noexcept { return m_tl_side; }

private:
  screen_coordinat m_br_board;
  screen_coordinat m_br_side;
  screen_coordinat m_tl_board;
  screen_coordinat m_tl_side;

  /// The size of the window
  screen_coordinat m_window_size;
};

#endif // GAME_VIEW_LAYOUT_H
