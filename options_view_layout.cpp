#include "options_view_layout.h"

#include "options_view_layout.h"

#include <cassert>
#include <cmath>

options_view_layout::options_view_layout(
  const screen_coordinat& window_size,
  const int margin_width
) : m_window_size{window_size}
{
  // There are 7 panels
  // the chessboard is aimed to be 4 panels high
  const int panel_height{
    static_cast<int>(
      static_cast<double>(window_size.get_y() - (4 * margin_width)) / 11.0
    )
  };
  const int top_panel_width{
    static_cast<int>(
      static_cast<double>(window_size.get_x() - (2 * margin_width)) / 2.0
    )
  };
  const int max_chess_board_height{
    m_window_size.get_y() - (7 * panel_height) - (4 * margin_width)
  };
  const int max_chess_board_width{
    window_size.get_x() - (2 * margin_width)
  };
  const int chess_board_width{
    std::min(max_chess_board_height, max_chess_board_width)
  };
  const int chess_board_height{
    chess_board_width
  };

  const int bottom_panel_width{
    static_cast<int>(
      static_cast<double>(window_size.get_x() - (2 * margin_width)) / 3.0
    )
  };

  const int x1{margin_width};
  const int x2{x1 + bottom_panel_width};
  const int x3{x1 + top_panel_width};
  const int x4{x2 + bottom_panel_width};
  const int x5{x3 + top_panel_width};

  const int chess_board_tl_x{
    (window_size.get_x() / 2) - (chess_board_width / 2)
  };
  const int chess_board_br_x{
    (window_size.get_x() / 2) + (chess_board_width / 2)
  };

  const int y1{margin_width};
  const int y2{y1 + panel_height};
  const int y3{y2 + panel_height};
  const int y4{y3 + panel_height};
  const int y5{y4 + panel_height};
  const int y6{y5 + margin_width};
  const int y7{y6 + chess_board_height};
  const int y8{y7 + margin_width};
  const int y9{y8 + panel_height};
  const int y10{y9 + panel_height};
  const int y11{y10 + panel_height};

  m_game_speed_label = screen_rect(
    screen_coordinat(x1, y1),
    screen_coordinat(x3, y2)
  );
  m_game_speed_value = screen_rect(
    screen_coordinat(x3, y1),
    screen_coordinat(x5, y2)
  );

  m_music_volume_label = screen_rect(
    screen_coordinat(x1, y2),
    screen_coordinat(x3, y3)
  );
  m_music_volume_value = screen_rect(
    screen_coordinat(x3, y2),
    screen_coordinat(x5, y3)
  );

  m_sound_effects_volume_label = screen_rect(
    screen_coordinat(x1, y3),
    screen_coordinat(x3, y4)
  );
  m_sound_effects_volume_value = screen_rect(
    screen_coordinat(x3, y3),
    screen_coordinat(x5, y4)
  );

  m_starting_pos_label = screen_rect(
    screen_coordinat(x1, y4),
    screen_coordinat(x3, y5)
  );
  m_starting_pos_value = screen_rect(
    screen_coordinat(x3, y4),
    screen_coordinat(x5, y5)
  );

  m_chess_board = screen_rect(
    screen_coordinat(chess_board_tl_x, y6),
    screen_coordinat(chess_board_br_x, y7)
  );

  m_player_label = screen_rect(
    screen_coordinat(x1, y8),
    screen_coordinat(x2, y9)
  );
  m_color_label = screen_rect(
    screen_coordinat(x2, y8),
    screen_coordinat(x4, y9)
  );
  m_controls_label = screen_rect(
    screen_coordinat(x4, y8),
    screen_coordinat(x5, y9)
  );
  m_left_label = screen_rect(
    screen_coordinat(x1, y9),
    screen_coordinat(x2, y10)
  );
  m_right_label = screen_rect(
    screen_coordinat(x1, y10),
    screen_coordinat(x2, y11)
  );

  m_left_color_value = screen_rect(
    screen_coordinat(x2, y9),
    screen_coordinat(x4, y10)
  );
  m_right_color_value = screen_rect(
    screen_coordinat(x2, y10),
    screen_coordinat(x4, y11)
  );

  m_left_controls_value = screen_rect(
    screen_coordinat(x4, y9),
    screen_coordinat(x5, y10)
  );
  m_right_controls_value = screen_rect(
    screen_coordinat(x4, y10),
    screen_coordinat(x5, y11)
  );

  m_font_size = std::min(
    panel_height / 2,
    bottom_panel_width / 6
  );
}

const screen_rect& options_view_layout::get_selectable_rect(const options_view_item item) const noexcept
{
  switch (item)
  {
    case options_view_item::game_speed: return m_game_speed_value;
    case options_view_item::music_volume: return m_music_volume_value;
    case options_view_item::sound_effects_volume: return m_sound_effects_volume_value;
    case options_view_item::starting_position: return m_starting_pos_value;
    case options_view_item::left_color: return m_left_color_value;
    case options_view_item::right_color: return m_right_color_value;
    case options_view_item::left_controls: return m_left_controls_value;
    default:
    case options_view_item::right_controls:
      assert(item == options_view_item::right_controls);
      return m_right_controls_value;
  }
}

std::vector<screen_rect> get_panels(const options_view_layout& layout)
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
    layout.get_chess_board(),
    layout.get_player_label(),
    layout.get_color_label(),
    layout.get_controls_label(),
    layout.get_left_label(),
    layout.get_right_label(),
    layout.get_left_color_value(),
    layout.get_right_color_value(),
    layout.get_left_controls_value(),
    layout.get_right_controls_value()
  };
}

void test_options_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const options_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  // get_selectable_rect
  {
    const options_view_layout layout;
    assert(layout.get_selectable_rect(options_view_item::game_speed) == layout.get_game_speed_value());
    assert(layout.get_selectable_rect(options_view_item::music_volume) == layout.get_music_volume_value());
    assert(layout.get_selectable_rect(options_view_item::sound_effects_volume) == layout.get_sound_effects_volume_value());
    assert(layout.get_selectable_rect(options_view_item::starting_position) == layout.get_starting_pos_value());
    assert(layout.get_selectable_rect(options_view_item::left_color) == layout.get_left_color_value());
    assert(layout.get_selectable_rect(options_view_item::left_controls) == layout.get_left_controls_value());
    assert(layout.get_selectable_rect(options_view_item::right_color) == layout.get_right_color_value());
    assert(layout.get_selectable_rect(options_view_item::right_controls) == layout.get_right_controls_value());
  }
  // get_selectable_rect on all items
  {
    const options_view_layout layout;
    for (const auto i: get_all_options_view_items())
    {
      assert(layout.get_selectable_rect(i).get_br().get_x() >= 0);
    }
  }
  #endif
}
