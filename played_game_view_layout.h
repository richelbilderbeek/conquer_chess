#ifndef PLAYED_GAME_VIEW_LAYOUT_H
#define PLAYED_GAME_VIEW_LAYOUT_H

#include "screen_rect.h"
#include "layout.h"

#include <vector>

/// The layout of the menu view
///   x          x
///   1          2
///
/// +--------------+
/// |              |
/// | +----------+ | y1
/// | | text     | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | +----------+ | y2
/// |              |
/// +--------------+
///
///   ^          ^
///   |          |
///   +----------+
///    panel_width
///
class played_game_view_layout
{
public:
  explicit played_game_view_layout(
    const screen_coordinat& window_size = get_default_played_game_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  const auto& get_text() const noexcept { return m_text; }

  /// Get the size of the font that would fit nicely
  int get_font_size() const noexcept { return m_font_size; }

  screen_coordinat get_window_size() const noexcept { return m_window_size; }

private:

  screen_rect m_text;

  /// The size of the font that would fit nicely
  int m_font_size;

  /// The size of the window
  screen_coordinat m_window_size;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const played_game_view_layout& layout);

/// Test the played_game_view_layout class
void test_played_game_view_layout();

#endif // PLAYED_GAME_VIEW_LAYOUT_H
