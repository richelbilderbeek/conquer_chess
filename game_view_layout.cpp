#include "game_view_layout.h"

#include "helper.h"

#include <cassert>
#include <cmath>

game_view_layout::game_view_layout(
  const screen_coordinat& window_size,
  const int margin_width
) : m_window_size{window_size}
{
  // board
  const int max_board_height = m_window_size.get_y() - margin_width - margin_width;
  const int max_board_width = m_window_size.get_x() - margin_width - margin_width;
  m_board_width = std::min(max_board_height, max_board_width);
  m_board_height = m_board_width;

  m_panel_height = static_cast<int>(static_cast<double>(window_size.get_y() - (4 * margin_width)) / 3.0);
  m_panel_width = static_cast<int>(static_cast<double>(window_size.get_x() - m_board_width - (4 * margin_width))) / 2;

  const int x1{margin_width};
  const int x2{x1 + m_panel_width};
  const int x3{x2 + margin_width};
  const int x4{x3 + m_board_width};
  const int x5{x4 + margin_width};
  const int x6{x5 + m_panel_width};

  const int y1{margin_width};
  const int y2{y1 + m_panel_height};
  const int y3{y2 + margin_width};
  const int y4{y3 + m_panel_height};
  const int y5{y4 + margin_width};
  const int y6{y5 + m_panel_height};


  // Panel 1
  m_tl_units_1 = screen_coordinat(x1, y1);
  m_tl_controls_1 = screen_coordinat(x1, y3);
  m_tl_debug_1 = screen_coordinat(x1, y5);

  m_br_units_1 = screen_coordinat(x2, y2);
  m_br_controls_1 = screen_coordinat(x2, y4);
  m_br_debug_1 = screen_coordinat(x2, y6);

  // Board
  m_tl_board = screen_coordinat(x3, y1);
  m_br_board = screen_coordinat(x4, y6);


  // Panel 2
  m_tl_units_2 = screen_coordinat(x5, y1);
  m_tl_controls_2 = screen_coordinat(x5, y3);
  m_tl_debug_2 = screen_coordinat(x5, y5);

  m_br_units_2 = screen_coordinat(x6, y2);
  m_br_controls_2 = screen_coordinat(x6, y4);
  m_br_debug_2 = screen_coordinat(x6, y6);
}

game_coordinat convert_to_game_coordinat(
  const screen_coordinat& coordinat,
  const game_view_layout& layout
)
{
  // How many pixels on the board
  const int screen_on_board_x{
    coordinat.get_x() - layout.get_tl_board().get_x()
  };
  const int screen_on_board_y{
    coordinat.get_y() - layout.get_tl_board().get_y()
  };
  // Fraction of the board
  const double f_x{
    static_cast<double>(screen_on_board_x) / static_cast<double>(layout.get_board_width())
  };
  const double f_y{
    static_cast<double>(screen_on_board_y) / static_cast<double>(layout.get_board_height())
  };
  return game_coordinat(
    8.0 * f_x,
    8.0 * f_y
  );
}

screen_coordinat convert_to_screen_coordinat(
  const game_coordinat& coordinat,
  const game_view_layout& layout
)
{
  const auto tl_board{layout.get_tl_board()};
  const auto br_board{layout.get_br_board()};
  const double square_width{
    static_cast<double>(br_board.get_x() - tl_board.get_x()) / 8.0
  };
  const double square_height{
    static_cast<double>(br_board.get_y() - tl_board.get_y()) / 8.0
  };
  return screen_coordinat(
    tl_board.get_x() + (square_width * coordinat.get_x()),
    tl_board.get_y() + (square_height * coordinat.get_y())
  );
}

double game_view_layout::get_square_height() const noexcept
{
  return static_cast<double>(get_board_height()) / 8.0;
}

double game_view_layout::get_square_width() const noexcept
{
  return static_cast<double>(get_board_width()) / 8.0;
}

void test_game_view_layout()
{
  #ifndef NDEBUG
  // Minimal tests
  {
    const game_view_layout layout;

    assert(layout.get_br_board().get_x() > 0.0);
    assert(layout.get_br_board().get_y() > 0.0);
    assert(layout.get_tl_board().get_x() > 0.0);
    assert(layout.get_tl_board().get_y() > 0.0);

    assert(layout.get_br_debug_2().get_x() > 0.0);
    assert(layout.get_br_debug_2().get_y() > 0.0);
    assert(layout.get_tl_units_2().get_x() > 0.0);
    assert(layout.get_tl_units_2().get_y() > 0.0);
    assert(layout.get_board_width() > 0.0);
    assert(layout.get_board_height() > 0.0);
    assert(layout.get_square_width() > 0.0);
    assert(layout.get_square_height() > 0.0);
  }
  // Coordinats of default size
  {
    const int margin_width = 10;
    const game_view_layout layout{screen_coordinat(1024, 576), margin_width};
    const int x1{margin_width};
    const int x2{x1 + layout.get_panel_width()};
    const int x3{x2 + margin_width};
    const int x4{x3 + layout.get_board_width()};
    const int x5{x4 + margin_width};
    const int x6{x5 + layout.get_panel_width()};

    const int y1{margin_width};
    const int y2{y1 + layout.get_panel_height()};
    const int y3{y2 + margin_width};
    const int y4{y3 + layout.get_panel_height()};
    const int y5{y4 + margin_width};
    const int y6{y5 + layout.get_panel_height()};

    assert(layout.get_tl_units_1().get_x() == x1);
    assert(layout.get_tl_units_1().get_y() == y1);
    assert(layout.get_br_units_1().get_x() == x2);
    assert(layout.get_br_units_1().get_y() == y2);

    assert(layout.get_tl_controls_1().get_x() == x1);
    assert(layout.get_tl_controls_1().get_y() == y3);
    assert(layout.get_br_controls_1().get_x() == x2);
    assert(layout.get_br_controls_1().get_y() == y4);

    assert(layout.get_tl_debug_1().get_x() == x1);
    assert(layout.get_tl_debug_1().get_y() == y5);
    assert(layout.get_br_debug_1().get_x() == x2);
    assert(layout.get_br_debug_1().get_y() == y6);

    assert(layout.get_tl_board().get_x() == x3);
    assert(layout.get_tl_board().get_y() == y1);
    assert(layout.get_br_board().get_x() == x4);
    assert(layout.get_br_board().get_y() == y6);

    assert(layout.get_tl_units_2().get_x() == x5);
    assert(layout.get_tl_units_2().get_y() == y1);
    assert(layout.get_br_units_2().get_x() == x6);
    assert(layout.get_br_units_2().get_y() == y2);

    assert(layout.get_tl_controls_2().get_x() == x5);
    assert(layout.get_tl_controls_2().get_y() == y3);
    assert(layout.get_br_controls_2().get_x() == x6);
    assert(layout.get_br_controls_2().get_y() == y4);

    assert(layout.get_tl_debug_2().get_x() == x5);
    assert(layout.get_tl_debug_2().get_y() == y5);
    assert(layout.get_br_debug_2().get_x() == x6);
    assert(layout.get_br_debug_2().get_y() == y6);
  }
  //--------------------------------------------------------------------------
  // game -> screen
  //--------------------------------------------------------------------------
  // in-game (0,0) must be top-left of screen board
  {
    const game_view_layout layout;
    const auto tl_board = convert_to_screen_coordinat(
      game_coordinat(0.0, 0.0),
      layout
    );
    assert(tl_board.get_x() == layout.get_tl_board().get_x());
    assert(tl_board.get_y() == layout.get_tl_board().get_y());
  }
  // in-game (8,8) must be bottom-right of screen board
  // (no piece can ever have its top-right at the bottom-right of the board)
  {
    const game_view_layout layout;
    const auto br_board = convert_to_screen_coordinat(
      game_coordinat(8.0, 8.0),
      layout
    );
    assert(br_board.get_x() == layout.get_br_board().get_x());
    assert(br_board.get_y() == layout.get_br_board().get_y());
  }
  //--------------------------------------------------------------------------
  // screen -> game
  //--------------------------------------------------------------------------
  // top-left of screen board must be in-game (0,0)
  // bottom-right of screen board must be in-game (8,8)
  {
    const game_view_layout layout;
    const auto br_board = convert_to_game_coordinat(
      layout.get_br_board(),
      layout
    );
    const auto tl_board = convert_to_game_coordinat(
      layout.get_tl_board(),
      layout
    );
    assert(tl_board.get_x() == 0.0);
    assert(tl_board.get_y() == 0.0);
    assert(br_board.get_x() == 8.0);
    assert(is_close(br_board.get_y(), 8.0, 0.03));
  }
  #endif
}
