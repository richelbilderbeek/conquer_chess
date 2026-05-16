#include "options_controls_view_layout.h"

#include "options_controls_view_item.h"
#include <cassert>
#include <cmath>

options_controls_view_layout::options_controls_view_layout(
  const screen_rect& r,
  const int margin_size
) : m_background{r}
{
  const int panel_height{100};
  const int top_panel_width{900};
  const int chess_board_width{8 * 24};
  const int chess_board_height{chess_board_width};

  // Top panel
  const int x1{128};
  const int x3{x1 + (top_panel_width / 2)};
  const int x5{x3 + (top_panel_width / 2)};


  // Center panel
  const int chess_board_tl_x{
    x3 - (chess_board_width / 2)
  };
  const int chess_board_br_x{
    chess_board_tl_x + chess_board_width
  };

  const int y1{16 + margin_size};
  const int y2{y1 + panel_height};
  const int y3{y2 + panel_height};
  const int y4{y3 + panel_height};
  const int y5{y4 + panel_height};
  const int y6{y5 + margin_size + margin_size};
  const int y7{y6 + chess_board_height};
  const int y8{y7 + margin_size + margin_size};
  const int y9{y8 + panel_height};
  const int y10{y9 + panel_height};

  m_game_speed_label = screen_rect(
    screen_coordinate(x1, y1),
    screen_coordinate(x3, y2)
  );
  m_game_speed_value = screen_rect(
    screen_coordinate(x3, y1),
    screen_coordinate(x5, y2)
  );

  m_music_volume_label = screen_rect(
    screen_coordinate(x1, y2),
    screen_coordinate(x3, y3)
  );
  m_music_volume_value = screen_rect(
    screen_coordinate(x3, y2),
    screen_coordinate(x5, y3)
  );

  m_sound_effects_volume_label = screen_rect(
    screen_coordinate(x1, y3),
    screen_coordinate(x3, y4)
  );
  m_sound_effects_volume_value = screen_rect(
    screen_coordinate(x3, y3),
    screen_coordinate(x5, y4)
  );

  m_starting_pos_label = screen_rect(
    screen_coordinate(x1, y4),
    screen_coordinate(x3, y5)
  );
  m_starting_pos_value = screen_rect(
    screen_coordinate(x3, y4),
    screen_coordinate(x5, y5)
  );


  m_chess_board = screen_rect(
    screen_coordinate(chess_board_tl_x, y6),
    screen_coordinate(chess_board_br_x, y7)
  );

  m_controls_label = screen_rect(
    screen_coordinate(x1, y8),
    screen_coordinate(x5, y9)
  );
  m_left_controls_value = screen_rect(
    screen_coordinate(x1, y9),
    screen_coordinate(x3, y10)
  );
  m_right_controls_value = screen_rect(
    screen_coordinate(x3, y9),
    screen_coordinate(x5, y10)
  );
}

const screen_rect& options_controls_view_layout::get_controller_type_value(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_left_controls_value;
  }
  assert(player_side == side::rhs);
  return m_right_controls_value;
}

const screen_rect& options_controls_view_layout::get_selectable_rect(const options_controls_view_item item) const noexcept
{
  switch (item)
  {
    case options_controls_view_item::game_speed: return m_game_speed_value;
    case options_controls_view_item::music_volume: return m_music_volume_value;
    case options_controls_view_item::sound_effects_volume: return m_sound_effects_volume_value;
    case options_controls_view_item::starting_position: return m_starting_pos_value;
    case options_controls_view_item::left_controls: return m_left_controls_value;
    default:
    case options_controls_view_item::right_controls:
      assert(item == options_controls_view_item::right_controls);
      return m_right_controls_value;
  }
}

std::vector<screen_rect> get_panels(const options_controls_view_layout& layout)
{
  return
  {
    layout.get_game_speed_label(),
    layout.get_game_speed_value(),
    layout.get_music_volume_label(),
    layout.get_music_volume_value(),
    layout.get_sound_effects_volume_label(),
    layout.get_sound_effects_volume_value(),
    layout.get_starting_pos_label(),
    layout.get_starting_pos_value(),
    layout.get_chess_board().get_board(),
    layout.get_controls_label(),
    layout.get_controller_type_value(side::lhs),
    layout.get_controller_type_value(side::rhs)
  };
}

void test_options_controls_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const options_controls_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  // get_selectable_rect
  {
    const options_controls_view_layout layout;
    assert(layout.get_selectable_rect(options_controls_view_item::game_speed) == layout.get_game_speed_value());
    assert(layout.get_selectable_rect(options_controls_view_item::music_volume) == layout.get_music_volume_value());
    assert(layout.get_selectable_rect(options_controls_view_item::sound_effects_volume) == layout.get_sound_effects_volume_value());
    assert(layout.get_selectable_rect(options_controls_view_item::starting_position) == layout.get_starting_pos_value());
    assert(layout.get_selectable_rect(options_controls_view_item::left_controls) == layout.get_controller_type_value(side::lhs));
    assert(layout.get_selectable_rect(options_controls_view_item::right_controls) == layout.get_controller_type_value(side::rhs));
  }
  // get_selectable_rect on all items
  {
    const options_controls_view_layout layout;
    for (const auto i: get_all_options_controls_view_items())
    {
      assert(layout.get_selectable_rect(i).get_br().get_x() >= 0);
    }
  }
  #endif
}
