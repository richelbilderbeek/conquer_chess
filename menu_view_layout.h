#ifndef MENU_VIEW_LAYOUT_H
#define MENU_VIEW_LAYOUT_H

#include "screen_rect.h"
#include "layout.h"

#include <vector>

/// The layout of the menu view
///   x          x
///   1          2
///
/// +--------------+
/// |              |
/// | +----------+ | <--+                  y1
/// | | title    | |    +-- panel_height
/// | +----------+ | <--+                  y2
/// |              |
/// | +----------+ |                       y3
/// | | subtitle | |
/// | +----------+ |                       y4
/// |              |
/// | +----------+ |                       y5
/// | | start    | |
/// | +----------+ |                       y6
/// |              |
/// | +----------+ |                       y7
/// | | options  | |
/// | +----------+ |                       y8
/// |              |
/// | +----------+ |                       y9
/// | | about    | |
/// | +----------+ |                       y10
/// |              |
/// | +----------+ |                       y11
/// | | quit     | |
/// | +----------+ |                       y12
/// |              |
/// +--------------+
///
///   ^          ^
///   |          |
///   +----------+
///    panel_width
///
class menu_view_layout
{
public:
  menu_view_layout(
    const screen_coordinat& window_size = get_default_screen_size(),
    const int margin_width = get_default_margin_width()
  );


  screen_rect get_title() const noexcept { return m_title; }
  screen_rect get_subtitle() const noexcept { return m_subtitle; }
  screen_rect get_start() const noexcept { return m_start; }
  screen_rect get_about() const noexcept { return m_about; }
  screen_rect get_options() const noexcept { return m_options; }
  screen_rect get_quit() const noexcept { return m_quit; }

  screen_coordinat get_window_size() const noexcept { return m_window_size; }

private:

  screen_rect m_title;
  screen_rect m_subtitle;
  screen_rect m_start;
  screen_rect m_options;
  screen_rect m_about;
  screen_rect m_quit;

  /// The size of the window
  screen_coordinat m_window_size;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const menu_view_layout& layout);

/// Resize the layout to a new size
void resize(
  menu_view_layout& g,
  const screen_coordinat& window_size,
  const int margin_width = get_default_margin_width()
);

/// Test the menu_view_layout class
void test_menu_view_layout();

#endif // MENU_VIEW_LAYOUT_H
