#ifndef ABOUT_VIEW_LAYOUT_H
#define ABOUT_VIEW_LAYOUT_H

#include "about_view_item.h"
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
/// | |          | |
/// | | title    | |
/// | | 2 panels | |
/// | +----------+ | y2
/// |              |
/// | +----------+ | y3
/// | | subtitle | |
/// | +----------+ | y4
/// |              |
/// | +----------+ | y5
/// | | text     | |
/// | | 4 pabels | |
/// | | high     | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | |          | |
/// | +----------+ | y6
/// |              |
/// +--------------+
///
///   ^          ^
///   |          |
///   +----------+
///    panel_width
///
class about_view_layout
{
public:
  explicit about_view_layout(
    const screen_coordinat& window_size = get_default_about_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  const auto& get_title() const noexcept { return m_title; }
  const auto& get_subtitle() const noexcept { return m_subtitle; }
  const auto& get_text() const noexcept { return m_text; }

  /// Get the size of the font that would fit nicely
  int get_font_size() const noexcept { return m_font_size; }

  screen_coordinat get_window_size() const noexcept { return m_window_size; }

private:

  screen_rect m_title;
  screen_rect m_subtitle;
  screen_rect m_text;

  /// The size of the font that would fit nicely
  int m_font_size;

  /// The size of the window
  screen_coordinat m_window_size;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const about_view_layout& layout);

/// Test the about_view_layout class
void test_about_view_layout();

#endif // ABOUT_VIEW_LAYOUT_H
