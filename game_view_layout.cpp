#include "game_view_layout.h"

#include "helper.h"
#include "game_rect.h"
#include "game_coordinat.h"
#include "square.h"

#include <cassert>
#include <cmath>

game_view_layout::game_view_layout(
  const screen_coordinat& window_size,
  const int margin_width
) : m_window_size{window_size}
{
  const int unit_panel_height{100};
  const int control_panel_height{75};
  const int log_panel_height{
    (
      window_size.get_y()
      - (4 * margin_width)
      - unit_panel_height
      - control_panel_height
    )
    / 2
  };
  const int debug_panel_height{log_panel_height};
  const int panel_width = 300;

  const int max_board_width = m_window_size.get_x() - (2 * panel_width) - (4 * margin_width);
  const int max_board_height = m_window_size.get_y() - (2 * margin_width);
  const int board_width = std::min(max_board_height, max_board_width);
  const int board_height = board_width;
  assert(board_width == board_height);

  const int x1{margin_width};
  const int x2{x1 + panel_width};
  const int x3{x2 + margin_width};
  const int x4{x3 + board_width};
  const int x5{x4 + margin_width};
  const int x6{x5 + panel_width};

  const int y1{margin_width};
  const int y2{y1 + unit_panel_height};
  const int y3{y2 + control_panel_height};
  const int y4{y3 + margin_width};
  const int y5{y4 + log_panel_height};
  const int y6{y5 + margin_width};
  const int y7{y6 + debug_panel_height};


  // Panel 1
  m_units_1 = screen_rect(
    screen_coordinat(x1, y1),
    screen_coordinat(x2, y2)
  );
  m_controls_lhs = screen_rect(
    screen_coordinat(x1, y2),
    screen_coordinat(x2, y3)
  );
  m_log_1 = screen_rect(
    screen_coordinat(x1, y4),
    screen_coordinat(x2, y5)
  );
  m_debug_1 = screen_rect(
    screen_coordinat(x1, y6),
    screen_coordinat(x2, y7)
  );

  // Board
  m_board = screen_rect(
    screen_coordinat(x3, y1),
    screen_coordinat(x3 + board_width, y1 + board_height)
  );
  // Center board vertically
  const int empty_vertical_space{
    m_window_size.get_y()
    - (2 * margin_width)
    - get_height(m_board)
  };
  m_board += screen_coordinat(0, empty_vertical_space / 2);
  assert(get_width(m_board) == get_height(m_board));

  // Panel 2
  m_units_2 = screen_rect(
    screen_coordinat(x5, y1),
    screen_coordinat(x6, y2)
  );
  m_controls_rhs = screen_rect(
    screen_coordinat(x5, y2),
    screen_coordinat(x6, y3)
  );
  m_log_2 = screen_rect(
    screen_coordinat(x5, y4),
    screen_coordinat(x6, y5)
  );
  m_debug_2 = screen_rect(
    screen_coordinat(x5, y6),
    screen_coordinat(x6, y7)
  );

  assert(get_board_width(*this) == get_board_height(*this));
  assert(get_square_width(*this) == get_square_height(*this));

  {
    const int sz{75};
    m_controls_lhs_key_1 = screen_rect(
      m_controls_lhs.get_tl() + screen_coordinat(0 * sz, 0 * sz),
      m_controls_lhs.get_tl() + screen_coordinat(1 * sz, 1 * sz)
    );
    m_controls_lhs_key_2 = screen_rect(
      m_controls_lhs.get_tl() + screen_coordinat(1 * sz, 0 * sz),
      m_controls_lhs.get_tl() + screen_coordinat(2 * sz, 1 * sz)
    );
    m_controls_lhs_key_3 = screen_rect(
      m_controls_lhs.get_tl() + screen_coordinat(2 * sz, 0 * sz),
      m_controls_lhs.get_tl() + screen_coordinat(3 * sz, 1 * sz)
    );
    m_controls_lhs_key_4 = screen_rect(
      m_controls_lhs.get_tl() + screen_coordinat(3 * sz, 0 * sz),
      m_controls_lhs.get_tl() + screen_coordinat(4 * sz, 1 * sz)
    );

    m_controls_rhs_key_1 = screen_rect(
      m_controls_rhs.get_tl() + screen_coordinat(0 * sz, 0 * sz),
      m_controls_rhs.get_tl() + screen_coordinat(1 * sz, 1 * sz)
    );
    m_controls_rhs_key_2 = screen_rect(
      m_controls_rhs.get_tl() + screen_coordinat(1 * sz, 0 * sz),
      m_controls_rhs.get_tl() + screen_coordinat(2 * sz, 1 * sz)
    );
    m_controls_rhs_key_3 = screen_rect(
      m_controls_rhs.get_tl() + screen_coordinat(2 * sz, 0 * sz),
      m_controls_rhs.get_tl() + screen_coordinat(3 * sz, 1 * sz)
    );
    m_controls_rhs_key_4 = screen_rect(
      m_controls_rhs.get_tl() + screen_coordinat(3 * sz, 0 * sz),
      m_controls_rhs.get_tl() + screen_coordinat(4 * sz, 1 * sz)
    );
  }
}

game_coordinat convert_to_game_coordinat(
  const screen_coordinat& coordinat,
  const game_view_layout& layout
)
{
  // How many pixels on the board
  const int screen_on_board_x{
    coordinat.get_x() - layout.get_board().get_tl().get_x()
  };
  const int screen_on_board_y{
    coordinat.get_y() - layout.get_board().get_tl().get_y()
  };
  // Fraction of the board
  const double f_x{
    static_cast<double>(screen_on_board_x) / static_cast<double>(get_board_width(layout))
  };
  const double f_y{
    static_cast<double>(screen_on_board_y) / static_cast<double>(get_board_height(layout))
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
  const auto tl_board{layout.get_board().get_tl()};
  //const auto br_board{layout.get_board().get_br()};

  const double square_width{
    get_square_width(layout)
  };
  const double square_height{
    get_square_height(layout)
  };
  return screen_coordinat(
    tl_board.get_x() + (square_width * coordinat.get_x()),
    tl_board.get_y() + (square_height * coordinat.get_y())
  );
}

screen_rect convert_to_screen_rect(
  const game_rect& r,
  const game_view_layout& layout
)
{
  const auto& tl{convert_to_screen_coordinat(r.get_tl(), layout)};
  const auto& br{convert_to_screen_coordinat(r.get_br(), layout)};
  return screen_rect(tl, br);
}

screen_rect convert_to_screen_rect(
  const square& s,
  const game_view_layout& layout
)
{
  const game_coordinat tl(s.get_x(), s.get_y());
  const game_coordinat br(s.get_x() + 1, s.get_y() + 1);
  const game_rect rect(tl, br);
  return convert_to_screen_rect(rect, layout);
}

int get_board_height(const game_view_layout& layout) noexcept
{
  return get_height(layout.get_board());
}

int get_board_width(const game_view_layout& layout) noexcept
{
  return get_width(layout.get_board());
}


const screen_rect& game_view_layout::get_controls(const side player) const noexcept
{
  if (player == side::lhs) return m_controls_lhs;
  assert(player == side::rhs);
  return m_controls_rhs;
}

const screen_rect& game_view_layout::get_controls_key(
  const side player,
  const int key
) const noexcept
{
  assert(key >= 1); // Human based counting
  assert(key <= 4); // Human based counting
  if (player == side::lhs)
  {
    switch (key)
    {
      case 1: return m_controls_lhs_key_1;
      case 2: return m_controls_lhs_key_2;
      case 3: return m_controls_lhs_key_3;
      case 4:
        default:
        assert(key == 4);
        return m_controls_lhs_key_4;
    }
  }
  assert(player == side::rhs);
  switch (key)
  {
    case 1: return m_controls_rhs_key_1;
    case 2: return m_controls_rhs_key_2;
    case 3: return m_controls_rhs_key_3;
    case 4:
      default:
      assert(key == 4);
      return m_controls_rhs_key_4;
  }
}



const screen_rect& game_view_layout::get_log(const side player) const noexcept
{
  if (player == side::lhs) return m_log_1;
  assert(player == side::rhs);
  return m_log_2;
}

std::vector<screen_rect> get_panels(const game_view_layout& layout)
{
  return
  {
    layout.get_board(),
    layout.get_controls(side::lhs),
    layout.get_controls(side::rhs),
    layout.get_debug_1(),
    layout.get_debug_2(),
    layout.get_log(side::lhs),
    layout.get_log(side::rhs),
    layout.get_units_1(),
    layout.get_units_2()
  };
}

int game_view_layout::get_panel_height() const noexcept
{
  return get_height(m_units_1);
}

int game_view_layout::get_panel_width() const noexcept
{
  return get_width(m_units_1);
}

double get_square_height(const game_view_layout& layout) noexcept
{
  return static_cast<double>(get_board_height(layout)) / 8.0;
}

double get_square_width(const game_view_layout& layout) noexcept
{
  return static_cast<double>(get_board_width(layout)) / 8.0;
}

void test_game_view_layout()
{
  #ifndef NDEBUG
  {
    const game_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  {
    const game_view_layout layout;
    assert(layout.get_board().get_br().get_x() > 0.0);
    assert(layout.get_board().get_br().get_y() > 0.0);
    assert(layout.get_board().get_tl().get_x() > 0.0);
    assert(layout.get_board().get_tl().get_y() > 0.0);

    assert(layout.get_debug_2().get_br().get_x() > 0.0);
    assert(layout.get_debug_2().get_br().get_y() > 0.0);
    assert(layout.get_units_2().get_tl().get_x() > 0.0);
    assert(layout.get_units_2().get_tl().get_y() > 0.0);
    assert(get_board_width(layout) > 0.0);
    assert(get_board_height(layout) > 0.0);
    assert(get_square_width(layout) > 0.0);
    assert(get_square_height(layout) > 0.0);
    assert(get_board_width(layout) == get_board_height(layout));
    assert(get_square_width(layout) == get_square_height(layout));
  }
  // get_controls_key
  {
    const game_view_layout layout;
    for (const auto player: get_all_sides())
    {
      for (int key{1}; key<=4; ++key)
      {
        assert(layout.get_controls_key(player, key).get_br().get_x() >= 0);
      }
    }
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
    assert(tl_board.get_x() == layout.get_board().get_tl().get_x());
    assert(tl_board.get_y() == layout.get_board().get_tl().get_y());
  }
  // convert_to_screen_rect
  {
    const game_view_layout layout;
    const auto board_rect = convert_to_screen_rect(
      game_rect(
        game_coordinat(0.0, 0.0),
        game_coordinat(8.0, 8.0)
      ),
      layout
    );
    assert(board_rect == layout.get_board());
  }
  // in-game (8,8) must be bottom-right of screen board
  // (no piece can ever have its top-right at the bottom-right of the board)
  {
    const game_view_layout layout;
    const auto br_board = convert_to_screen_coordinat(
      game_coordinat(8.0, 8.0),
      layout
    );
    assert(br_board.get_x() == layout.get_board().get_br().get_x());
    assert(br_board.get_y() == layout.get_board().get_br().get_y());
  }
  //--------------------------------------------------------------------------
  // screen -> game
  //--------------------------------------------------------------------------
  // top-left of screen board must be in-game (0,0)
  // bottom-right of screen board must be in-game (8,8)
  {
    const game_view_layout layout;
    const auto br_board = convert_to_game_coordinat(
      layout.get_board().get_br(),
      layout
    );
    const auto tl_board = convert_to_game_coordinat(
      layout.get_board().get_tl(),
      layout
    );
    assert(tl_board.get_x() == 0.0);
    assert(tl_board.get_y() == 0.0);
    assert(br_board.get_x() == 8.0);
    assert(is_close(br_board.get_y(), 8.0, 0.03));
  }
  // convert_to_screen_rect
  {
    const game_view_layout layout;
    const square a1("a1");
    const square h8("h8");
    const screen_rect a1_rect{convert_to_screen_rect(a1, layout)};
    const screen_rect h8_rect{convert_to_screen_rect(h8, layout)};
    assert(a1_rect.get_tl() == layout.get_board().get_tl());
    assert(h8_rect.get_br() == layout.get_board().get_br());
  }
  #endif
}
