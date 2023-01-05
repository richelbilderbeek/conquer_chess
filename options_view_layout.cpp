#include "options_view_layout.h"

#include "options_view_layout.h"

#include <cassert>
#include <cmath>

options_view_layout::options_view_layout(
  const screen_coordinat& window_size,
  const int margin_size
) : m_window_size{window_size}
{
  const int n_vertical_margins{4};
  const int n_vus_top{4}; // vus: vertical units
  const int n_vus_chessboard{3};
  const int n_vus_controls{4};
  const int n_vus{
    n_vus_top
    + n_vus_chessboard
    + n_vus_controls
  };
  const int width{window_size.get_x()};
  const int height{window_size.get_y()};
  const int panel_height{
    static_cast<int>(
      static_cast<double>(height - (n_vertical_margins * margin_size))
      / static_cast<double>(n_vus)
    )
  };
  const int top_panel_width{
    static_cast<int>(
      static_cast<double>(
        width - (2 * margin_size)
      ) / 2.0
    )
  };
  const int max_chess_board_height{
    height
    - ((n_vus_top + n_vus_controls) * panel_height)
    - (n_vertical_margins * margin_size)
  };
  const int max_chess_board_width{
    width - (2 * margin_size)
  };
  const int chess_board_width{
    std::min(max_chess_board_height, max_chess_board_width)
  };
  const int chess_board_height{
    chess_board_width
  };

  const int max_controller_height{
    height
    - ((n_vus_top + n_vus_chessboard + 1) * panel_height)
    - (n_vertical_margins * margin_size)
  };
  const int max_controller_width{
    (width - (2 * margin_size)) / 2
  };
  const int controller_height{
    std::min(max_controller_height, max_controller_width)
  };
  const int controller_width{
    controller_height
  };
  /*
  const int bottom_panel_height{
    controller_height + panel_height
  };
  */
  const int bottom_panel_width{
    controller_width * 2
  };

  // Top panel
  const int x1{margin_size};
  const int x3{x1 + top_panel_width};
  const int x5{x3 + top_panel_width};

  // Center panel
  const int chess_board_tl_x{
    (window_size.get_x() / 2) - (chess_board_width / 2)
  };
  const int chess_board_br_x{
    (window_size.get_x() / 2) + (chess_board_width / 2)
  };

  // Bottom panel
  const int x2{
    (window_size.get_x() / 2) - (bottom_panel_width / 2)
  };
  const int x4{
    (window_size.get_x() / 2) + (bottom_panel_width / 2)
  };

  const int y1{margin_size};
  const int y2{y1 + panel_height};
  const int y3{y2 + panel_height};
  const int y4{y3 + panel_height};
  const int y5{y4 + panel_height};
  const int y6{y5 + margin_size};
  const int y7{y6 + chess_board_height};
  const int y8{y7 + margin_size};
  const int y9{y8 + panel_height};
  const int y10{y9 + controller_height};

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

  m_controls_label = screen_rect(
    screen_coordinat(x2, y8),
    screen_coordinat(x4, y9)
  );
  m_left_controls_value = screen_rect(
    screen_coordinat(x2, y9),
    screen_coordinat(x3, y10)
  );
  m_right_controls_value = screen_rect(
    screen_coordinat(x3, y9),
    screen_coordinat(x4, y10)
  );

  m_font_size = std::min(
    panel_height / 2,
    bottom_panel_width / 6
  );
}

const screen_rect& options_view_layout::get_controller_type_value(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_left_controls_value;
  }
  assert(player_side == side::rhs);
  return m_right_controls_value;
}

const screen_rect& options_view_layout::get_selectable_rect(const options_view_item item) const noexcept
{
  switch (item)
  {
    case options_view_item::game_speed: return m_game_speed_value;
    case options_view_item::music_volume: return m_music_volume_value;
    case options_view_item::sound_effects_volume: return m_sound_effects_volume_value;
    case options_view_item::starting_position: return m_starting_pos_value;
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
    layout.get_controls_label(),
    layout.get_controller_type_value(side::lhs),
    layout.get_controller_type_value(side::rhs)
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
    assert(layout.get_selectable_rect(options_view_item::left_controls) == layout.get_controller_type_value(side::lhs));
    assert(layout.get_selectable_rect(options_view_item::right_controls) == layout.get_controller_type_value(side::rhs));
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
