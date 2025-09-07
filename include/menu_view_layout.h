#ifndef MENU_VIEW_LAYOUT_H
#define MENU_VIEW_LAYOUT_H

#include "menu_view_item.h"
#include "screen_rect.h"
#include "layout.h"

#include <vector>

/// The layout of the Menu dialog
///
/// ```text
/// Margins      _          _ _ _ _            _       _
/// ..........0 x x        x x x x x          x x     x width
/// ..........  1 2        3 4 5 6 7          8 9     1
/// ..........                                        0
/// ..........+-----------------------------------------+
/// ..........|   menu_panel                            |
/// ..........| +--------------+                        | y1
/// ..........| |              |                        |
/// ..........| | +----------+ |                        | y2
/// ..........| | |          | |                        |
/// ..........| | | title    | |                        |
/// ..........| | |          | |                        |
/// ..........| | +----------+ |                        | y3, y4
/// ..........| | | subtitle | |                        |
/// ..........| | +----------+ |                        | y5
/// ..........| |              |                        |
/// ..........| | +----------+ |                        | y6
/// ..........| | |spaces    | |  sub_menu_panel        |
/// ..........| | +----------+ | +--------------+       | y7
/// ..........| |              | |              |       |
/// ..........| | +----------+ | | +----------+ |       | y8
/// ..........| | | start    | | | | laws     | |       |
/// ..........| | +----------+ | | +----------+ |       | y9
/// ..........| |              | |              |       |
/// ..........| | +----------+ | | +----------+ |       | y10
/// ..........| | | options  | | | | controls | |       |
/// ..........| | +----------+ | | +----------+ |       | y11
/// ..........| |              | |              |       |
/// ..........| | +----------+ | | +----------+ |       | y12
/// ..........| | | about    | | | | vid&aud  | |       |
/// ..........| | +----------+ | | +----------+ |       | y13
/// ..........| |              | |              |       |
/// ..........| | +----------+ | +--------------+       | y14
/// ..........| | | quit     | |                        |
/// ..........| | +----------+ |                        | y15
/// ..........| |              |                        |
/// ..........| +--------------+                        | y16
/// ..........|                                         |
/// ..........| +-------------------------------------+ | y17
/// ..........| | Controls                            | |
/// ..........| +-------------------------------------+ | y18
/// ..........|                                         |
/// ..........+-----------------------------------------+
/// ..........
/// ..........    ^          ^
/// ..........    |          |
/// ..........    +----------+
/// ..........     panel_width
/// ```
class menu_view_layout
{
public:
  explicit menu_view_layout(
    const screen_rect& r = get_default_screen_rect(),
    const int margin_width = get_default_margin_width()
  );

  /// Get the rectangle to indicate selectness of an item
  const screen_rect& get_selectable_rect(const menu_view_item item) const noexcept;

  const auto& get_about() const noexcept { return m_about; }
  const auto& get_background_image() const noexcept { return m_background_image; }
  const auto& get_menu_panel() const noexcept { return m_menu_panel; }

  /// Main menu, options
  const auto& get_options() const noexcept { return m_options; }

  /// Submenu of options, controls
  const auto& get_options_controls() const noexcept { return m_options_controls; }

  /// Submenu of options, laws
  const auto& get_options_laws() const noexcept { return m_options_laws; }

  /// Submenu of options, video and audio
  const auto& get_options_video_and_audio() const noexcept { return m_options_video_and_audio; }

  const auto& get_quit() const noexcept { return m_quit; }
  const auto& get_start() const noexcept { return m_start; }
  const auto& get_sub_menu_panel() const noexcept { return m_sub_menu_panel; }
  const auto& get_subtitle() const noexcept { return m_subtitle; }
  const auto& get_title() const noexcept { return m_title; }

private:

  screen_rect m_about;
  screen_rect m_background_image;
  screen_rect m_menu_panel;
  screen_rect m_options;
  screen_rect m_options_controls;
  screen_rect m_options_laws;
  screen_rect m_options_video_and_audio;
  screen_rect m_quit;
  screen_rect m_start;
  screen_rect m_subtitle;
  screen_rect m_sub_menu_panel;
  screen_rect m_title;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const menu_view_layout& layout);

/// Test the menu_view_layout class
void test_menu_view_layout();

#endif // MENU_VIEW_LAYOUT_H
