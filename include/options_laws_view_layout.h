#ifndef OPTIONS_LAWS_VIEW_LAYOUT_H
#define OPTIONS_LAWS_VIEW_LAYOUT_H

#include "screen_rect.h"
#include "layout.h"
#include "options_laws_view_item.h"
#include "side.h"
#include "board_layout.h"
//#include "read_only.h"

#include <vector>

/// The layout of the Video and Audio Options dialog.
///
/// ```text
///        x x                 x                 x x
///        1 2                 3                 4 5
///
/// +------------------------------------------------+
/// |                                                |
/// |      +---------------------------------------+ |
/// |      |                                       | |
/// |      | +-----------------+-----------------+ | | y1
/// |      | | game speed      |                 | | |
/// |      | +-----------------+-----------------+ | | y2
/// |      | | music volume    |                 | | |
/// |      | +-----------------+-----------------+ | | y3
/// |      | | sound effects   | starting pos    | | |
/// |      | +-----------------+-----------------+ | | y4
/// |      | | starting pos    |                 | | |
/// |      | +-----------------+-----------------+ | | y5
/// |      |                                       | |
/// |      |            +-------------+            | | y6
/// |      |            |             |            | |
/// |      |            | BOARD       |            | |
/// |      |            |             |            | |
/// |      |            |             |            | |
/// |      |            |             |            | |
/// |      |            +-------------+            | | y7
/// |      |                                       | |
/// |      | +---------------------------------+   | | y8
/// |      | |           controls              |   | |
/// |      | +---------------------------------+   | | y9
/// |      | | left_controls | right_controls  |   | |
/// |      | +---------------------------------+   | | y10
/// |      |                                       | |
/// |      +---------------------------------------+ |
/// |                                                |
/// +------------------------------------------------+

/// ```text
///
/// - Top panel must be 400 pixels at least (i.e. with current font size)
/// - Bottom panel must be 400 pixels at least (i.e. with current font size)
class options_laws_view_layout
{
public:
  explicit options_laws_view_layout(
    const screen_rect& r = get_default_screen_rect(),
    const int margin_width = get_default_margin_width()
  );

  const screen_rect& get_background() const noexcept { return m_background; }

  const screen_rect& get_selectable_rect(const options_laws_view_item item) const noexcept;

  const screen_rect& get_game_speed_label() const noexcept { return m_game_speed_label; }
  const screen_rect& get_game_speed_value() const noexcept { return m_game_speed_value; }

  const screen_rect& get_music_volume_label() const noexcept { return m_music_volume_label; }
  const screen_rect& get_music_volume_value() const noexcept { return m_music_volume_value; }

  const screen_rect& get_sound_effects_volume_label() const noexcept { return m_sound_effects_volume_label; }
  const screen_rect& get_sound_effects_volume_value() const noexcept { return m_sound_effects_volume_value; }

  const screen_rect& get_starting_pos_label() const noexcept { return m_starting_pos_label; }
  const screen_rect& get_starting_pos_value() const noexcept { return m_starting_pos_value; }

  const board_layout& get_chess_board() const noexcept { return m_chess_board; }

  /// The word 'Controls'
  const screen_rect& get_controls_label() const noexcept { return m_controls_label; }

  /// The chosen type of controller for the LHS or RHS player
  const screen_rect& get_controller_type_value(const side player_side) const noexcept;

private:

  screen_rect m_background;
  board_layout m_chess_board;
  screen_rect m_controls_label;
  screen_rect m_game_speed_label;
  screen_rect m_game_speed_value;
  screen_rect m_left_controls_value;
  screen_rect m_music_volume_label;
  screen_rect m_music_volume_value;
  screen_rect m_right_controls_value;
  screen_rect m_sound_effects_volume_label;
  screen_rect m_sound_effects_volume_value;
  screen_rect m_starting_pos_label;
  screen_rect m_starting_pos_value;
};

/// Get the panels in the layout
std::vector<screen_rect> get_panels(const options_laws_view_layout& layout);

/// Test the options_laws_view_layout class
void test_options_laws_view_layout();

#endif // OPTIONS_LAWS_VIEW_LAYOUT_H
