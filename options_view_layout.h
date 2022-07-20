#ifndef OPTIONS_VIEW_LAYOUT_H
#define OPTIONS_VIEW_LAYOUT_H

#include "screen_rect.h"
#include "layout.h"
#include "options_view_item.h"

#include <vector>

/// The layout of the options view
///
///   x               x         x       x               x
///   1               2         3       4               5
///
/// +------------------------------------------------------+
/// |                                                      |
/// | +-------------------------+------------------------+ | y1
/// | | game speed              |                        | |
/// | +-------------------------+------------------------+ | y2
/// | | music volume            |                        | |
/// | +-------------------------+------------------------+ | y3
/// | | starting pos            |                        | |
/// | +-------------------------+------------------------+ | y4
/// |                                                      |
/// | +----------------+----------------+----------------+ | y5
/// | | player         | color          | controls       | |
/// | +----------------+----------------+----------------+ | y6
/// | | left           | left_color     | left_controls  | |
/// | +----------------+----------------+----------------+ | y7
/// | | right          | right_color    | right_controls | |
/// | +----------------+----------------+----------------+ | y8
/// |                                                      |
/// +------------------------------------------------------+
///
class options_view_layout
{
public:
  options_view_layout(
    const screen_coordinat& window_size = get_default_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  /// Get a selectable item
  const screen_rect& get_selectable_rect(const options_view_item item) const noexcept;

  screen_rect get_game_speed_label() const noexcept { return m_game_speed_label; }
  screen_rect get_game_speed_value() const noexcept { return m_game_speed_value; }

  screen_rect get_music_volume_label() const noexcept { return m_music_volume_label; }
  screen_rect get_music_volume_value() const noexcept { return m_music_volume_value; }

  screen_rect get_starting_pos_label() const noexcept { return m_starting_pos_label; }
  screen_rect get_starting_pos_value() const noexcept { return m_starting_pos_value; }

  screen_rect get_player_label() const noexcept { return m_player_label; }
  screen_rect get_color_label() const noexcept { return m_color_label; }
  screen_rect get_controls_label() const noexcept { return m_controls_label; }
  screen_rect get_left_label() const noexcept { return m_left_label; }
  screen_rect get_right_label() const noexcept { return m_right_label; }

  screen_rect get_left_color_value() const noexcept { return m_left_color_value; }
  screen_rect get_right_color_value() const noexcept { return m_right_color_value; }

  screen_rect get_left_controls_value() const noexcept { return m_left_controls_value; }
  screen_rect get_right_controls_value() const noexcept { return m_right_controls_value; }

  screen_coordinat get_window_size() const noexcept { return m_window_size; }

private:

  screen_rect m_game_speed_label;
  screen_rect m_game_speed_value;

  screen_rect m_music_volume_label;
  screen_rect m_music_volume_value;

  screen_rect m_starting_pos_label;
  screen_rect m_starting_pos_value;

  screen_rect m_player_label;
  screen_rect m_color_label;
  screen_rect m_controls_label;
  screen_rect m_left_label;
  screen_rect m_right_label;

  screen_rect m_left_color_value;
  screen_rect m_right_color_value;

  screen_rect m_left_controls_value;
  screen_rect m_right_controls_value;

  /// The size of the window
  screen_coordinat m_window_size;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const options_view_layout& layout);

/// Resize the layout to a new size
void resize(
  options_view_layout& g,
  const screen_coordinat& window_size,
  const int margin_width = get_default_margin_width()
);

/// Test the options_view_layout class
void test_options_view_layout();

#endif // OPTIONS_VIEW_LAYOUT_H
