#include "played_game_view_layout.h"

#include <cassert>
#include <cmath>

played_game_view_layout::played_game_view_layout(
  const screen_coordinat& window_size,
  const int margin_width
) : m_font_size{64},
    m_window_size{window_size}
{
  const int panel_height{window_size.get_y() - (2 * margin_width)};
  const int panel_width{window_size.get_x() - (2 * margin_width)};
  const int x1{margin_width};
  const int x2{x1 + panel_width};

  const int y1{margin_width};
  const int y2{y1 + panel_height};

  m_text = screen_rect(
    screen_coordinat(x1, y1),
    screen_coordinat(x2, y2)
  );
  m_font_size = std::min(
    panel_height / 20,
    panel_width / 10
  );
}

std::vector<screen_rect> get_panels(const played_game_view_layout& layout)
{
  return
  {
    layout.get_text()
  };
}

void test_played_game_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const played_game_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  #endif
}
