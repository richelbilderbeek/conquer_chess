#ifndef CONTROLS_VIEW_LAYOUT_H
#define CONTROLS_VIEW_LAYOUT_H

#include "controls_view_item.h"
#include "screen_rect.h"
#include "layout.h"

#include <vector>

/// The layout of the options view
///
///   x       x        x     x       x        x
///   1       2        3     4       5        6
///
/// +-------------------------------------------+
/// |                                           |
/// | +-------+--------+     +................+ | y1
/// | | type  | keybrd |     .       .        . |
/// | +-------+--------+     +................+ | y2
/// |                                           |
/// | +-------+--------+     +-------+--------+ | y3
/// | | up    | W      |     | do    | LMB    | |
/// | +-------+--------+     +-------+--------+ | y4
/// | | right | D      |     | next  | RMS    | |
/// | +-------+--------+     +-------+--------+ | y5
/// | | down  | S      |                        |
/// | +-------+--------+                        | y6
/// | | left  | A      |                        |
/// | +-------+--------+                        | y7
/// | | 1     | Q      |                        |
/// | +-------+--------+                        | y8
/// | | 2     | E      |                        |
/// | +-------+--------+                        | y9
/// | | 3     | Z      |                        |
/// | +-------+--------+                        | y10
/// | | 4     | C      |                        |
/// | +-------+--------+                        | y11
/// |                                           |
/// +-------------------------------------------+
///
class controls_view_layout
{
public:
  explicit controls_view_layout(
    const screen_coordinat& window_size = get_default_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  /// Get a screen rect
  const screen_rect& get_selectable_rect(const controls_view_item item) const noexcept;

  screen_coordinat get_window_size() const noexcept { return m_window_size; }

  const screen_rect& get_action_1_label() const noexcept { return m_action_1_label; }
  const screen_rect& get_action_1_value() const noexcept { return m_action_1_value; }
  const screen_rect& get_action_2_label() const noexcept { return m_action_2_label; }
  const screen_rect& get_action_2_value() const noexcept { return m_action_2_value; }
  const screen_rect& get_action_3_label() const noexcept { return m_action_3_label; }
  const screen_rect& get_action_3_value() const noexcept { return m_action_3_value; }
  const screen_rect& get_action_4_label() const noexcept { return m_action_4_label; }
  const screen_rect& get_action_4_value() const noexcept { return m_action_4_value; }
  const screen_rect& get_controller_type_label() const noexcept { return m_controller_type_label; }
  const screen_rect& get_controller_type_value() const noexcept { return m_controller_type_value; }
  const screen_rect& get_do_label() const noexcept { return m_do_label; }
  const screen_rect& get_do_value() const noexcept { return m_do_value; }
  const screen_rect& get_down_label() const noexcept { return m_down_label; }
  const screen_rect& get_down_value() const noexcept { return m_down_value; }
  const screen_rect& get_left_label() const noexcept { return m_left_label; }
  const screen_rect& get_left_value() const noexcept { return m_left_value; }
  const screen_rect& get_next_label() const noexcept { return m_next_label; }
  const screen_rect& get_next_value() const noexcept { return m_next_value; }
  const screen_rect& get_right_label() const noexcept { return m_right_label; }
  const screen_rect& get_right_value() const noexcept { return m_right_value; }
  const screen_rect& get_up_label() const noexcept { return m_up_label; }
  const screen_rect& get_up_value() const noexcept { return m_up_value; }

private:

  screen_rect m_action_1_label;
  screen_rect m_action_1_value;
  screen_rect m_action_2_label;
  screen_rect m_action_2_value;
  screen_rect m_action_3_label;
  screen_rect m_action_3_value;
  screen_rect m_action_4_label;
  screen_rect m_action_4_value;
  screen_rect m_controller_type_label;
  screen_rect m_controller_type_value;
  screen_rect m_do_label;
  screen_rect m_do_value;
  screen_rect m_down_label;
  screen_rect m_down_value;
  screen_rect m_left_label;
  screen_rect m_left_value;
  screen_rect m_next_label;
  screen_rect m_next_value;
  screen_rect m_right_label;
  screen_rect m_right_value;
  screen_rect m_up_label;
  screen_rect m_up_value;

  /// The size of the window
  screen_coordinat m_window_size;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const controls_view_layout& layout);

/// Test the controls_view_layout class
void test_controls_view_layout();

#endif // CONTROLS_VIEW_LAYOUT_H
