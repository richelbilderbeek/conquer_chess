#include "about_view_layout.h"

#include <cassert>
#include <cmath>

about_view_layout::about_view_layout(
  const screen_coordinat& window_size,
  const int margin_width
) : m_font_size{64},
    m_window_size{window_size}
{
  const int panel_height{
    static_cast<int>(
      static_cast<double>(window_size.get_y() - (4 * margin_width)) / 7.0
    )
  };
  const int panel_width{window_size.get_x() - (2 * margin_width)};
  const int x1{margin_width};
  const int x2{x1 + panel_width};

  const int y1{margin_width};
  const int y2{y1 + (2 * panel_height)};
  const int y3{y2 + margin_width};
  const int y4{y3 + (1 * panel_height)};
  const int y5{y4 + margin_width};
  const int y6{y5 + (4 * panel_height)};

  m_title = screen_rect(
    screen_coordinat(x1, y1),
    screen_coordinat(x2, y2)
  );
  m_subtitle = screen_rect(
    screen_coordinat(x1, y3),
    screen_coordinat(x2, y4)
  );
  m_text = screen_rect(
    screen_coordinat(x1, y5),
    screen_coordinat(x2, y6)
  );
  m_font_size = std::min(
    panel_height / 4,
    panel_width / 9
  );
}

std::vector<screen_rect> get_panels(const about_view_layout& layout)
{
  return
  {
    layout.get_title(),
    layout.get_subtitle(),
    layout.get_text()
  };
}

void test_about_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const about_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  #endif
}
