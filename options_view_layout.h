#ifndef OPTIONS_VIEW_LAYOUT_H
#define OPTIONS_VIEW_LAYOUT_H

#include "screen_rect.h"
#include "layout.h"
#include "options_view_item.h"
#include "side.h"

#include <vector>

/// The layout of the options view
///
///   x   x                 x                        x  x
///   1   2                 3                        4  5
///
/// +------------------------------------------------------+
/// |                                                      |
/// | +---------------------+----------------------------+ | y1
/// | | game speed          |                            | |
/// | +---------------------+----------------------------+ | y2
/// | | music volume        |                            | |
/// | +---------------------+----------------------------+ | y3
/// | | sound effects       |                            | |
/// | +---------------------+----------------------------+ | y4
/// | | starting pos        |                            | |
/// | +---------------------+----------------------------+ | y5
/// |                                                      |
/// |                     +-------------+                  | y6
/// |                     |             |                  |
/// |                     | BOARD       |                  |
/// |                     |             |                  |
/// |                     |             |                  |
/// |                     |             |                  |
/// |                     +-------------+                  | y7
/// |                                                      |
/// |     +------------------------------------------+     | y8
/// |     |                 controls                 |     |
/// |     +------------------------------------------+     | y9
/// |     |    left_controls    |   right_controls   |     |
/// |     +------------------------------------------+     | y10
/// |                                                      |
/// +------------------------------------------------------+
///
/// Top panel must be 400 pixels at least (i.e. with current font size)
/// Bottom panel must be 400 pixels at least (i.e. with current font size)
class options_view_layout
{
public:
  explicit options_view_layout(
    const screen_coordinat& window_size = get_default_screen_size(),
    const int margin_width = get_default_margin_width()
  );

  /// Get a selectable item
  const screen_rect& get_selectable_rect(const options_view_item item) const noexcept;

  const screen_rect& get_game_speed_label() const noexcept { return m_game_speed_label; }
  const screen_rect& get_game_speed_value() const noexcept { return m_game_speed_value; }

  const screen_rect& get_music_volume_label() const noexcept { return m_music_volume_label; }
  const screen_rect& get_music_volume_value() const noexcept { return m_music_volume_value; }

  const screen_rect& get_sound_effects_volume_label() const noexcept { return m_sound_effects_volume_label; }
  const screen_rect& get_sound_effects_volume_value() const noexcept { return m_sound_effects_volume_value; }

  const screen_rect& get_starting_pos_label() const noexcept { return m_starting_pos_label; }
  const screen_rect& get_starting_pos_value() const noexcept { return m_starting_pos_value; }

  const screen_rect& get_chess_board() const noexcept { return m_chess_board; }

  /// The word 'Controls'
  const screen_rect& get_controls_label() const noexcept { return m_controls_label; }

  /// The chosen type of controller for the LHS or RHS player
  const screen_rect& get_controller_type_value(const side player_side) const noexcept;

  /// Get the size of the font that would fit nicely
  int get_font_size() const noexcept { return m_font_size; }

  const screen_coordinat& get_window_size() const noexcept { return m_window_size; }

private:

  screen_rect m_game_speed_label;
  screen_rect m_game_speed_value;

  screen_rect m_music_volume_label;
  screen_rect m_music_volume_value;

  screen_rect m_sound_effects_volume_label;
  screen_rect m_sound_effects_volume_value;

  screen_rect m_starting_pos_label;
  screen_rect m_starting_pos_value;

  screen_rect m_chess_board;

  screen_rect m_controls_label;
  screen_rect m_left_controls_value;
  screen_rect m_right_controls_value;

  /// The size of the font that would fit nicely
  int m_font_size;

  /// The size of the window
  screen_coordinat m_window_size;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const options_view_layout& layout);

/// Test the options_view_layout class
void test_options_view_layout();

#endif // OPTIONS_VIEW_LAYOUT_H
