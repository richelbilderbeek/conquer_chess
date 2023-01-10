#include "lobby_view_layout.h"

#include <cassert>
#include <cmath>

lobby_view_layout::lobby_view_layout(
  const screen_coordinat& window_size,
  const int margin_width
) : m_font_size{64},
    m_window_size{window_size}
{
  const int n_vertical_units{5};
  const int n_vertical_margins{n_vertical_units + 1}; // margins are above, below and between panels
  const int panel_height{
    static_cast<int>(
      static_cast<double>(
        window_size.get_y() - (n_vertical_margins * margin_width))
        / static_cast<double>(n_vertical_units)
    )
  };
  const int n_horizontal_units{2};
  const int n_horizontal_margins{n_horizontal_units + 1}; // margins are above, below and between panels
  const int panel_width{
    static_cast<int>(
      static_cast<double>(
        window_size.get_x() - (n_horizontal_margins * margin_width))
        / static_cast<double>(n_horizontal_units)
    )
  };

  const int x1{margin_width};
  const int x2{x1 + panel_width};
  const int x3{x2 + margin_width};
  const int x4{x3 + panel_width};

  const int y1{margin_width};
  const int y2{y1 + panel_height + margin_width + panel_height};
  const int y3{y2 + margin_width};
  const int y4{y3 + panel_height};
  const int y5{y4 + margin_width};
  const int y6{y5 + panel_height};
  const int y7{y6 + margin_width};
  const int y8{y7 + panel_height};

  m_image = screen_rect(
    screen_coordinat(x1, y1),
    screen_coordinat(x4, y2)
  );

  m_lhs_color = screen_rect(
    screen_coordinat(x1, y3),
    screen_coordinat(x2, y4)
  );
  m_lhs_race = screen_rect(
    screen_coordinat(x1, y5),
    screen_coordinat(x2, y6)
  );
  m_lhs_start = screen_rect(
    screen_coordinat(x1, y7),
    screen_coordinat(x2, y8)
  );
  m_rhs_color = screen_rect(
    screen_coordinat(x3, y3),
    screen_coordinat(x4, y4)
  );
  m_rhs_race = screen_rect(
    screen_coordinat(x3, y5),
    screen_coordinat(x4, y6)
  );
  m_rhs_start = screen_rect(
    screen_coordinat(x3, y7),
    screen_coordinat(x4, y8)
  );
  m_font_size = std::min(
    panel_height / 2,
    panel_width / 6
  );
}


const screen_rect& lobby_view_layout::get_color(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_color;
  }
  assert(player_side == side::rhs);
  return m_rhs_color;
}

std::vector<screen_rect> get_panels(const lobby_view_layout& layout)
{
  return
  {
    layout.get_image(),
    layout.get_color(side::lhs),
    layout.get_race(side::lhs),
    layout.get_start(side::lhs),
    layout.get_color(side::rhs),
    layout.get_race(side::rhs),
    layout.get_start(side::rhs),
  };
}

const screen_rect& lobby_view_layout::get_race(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_race;
  }
  assert(player_side == side::rhs);
  return m_rhs_race;
}

const screen_rect& lobby_view_layout::get_start(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_start;
  }
  assert(player_side == side::rhs);
  return m_rhs_start;
}

const screen_rect& get_cursor_rect(
  const lobby_view_layout& layout,
  const lobby_view_item item,
  const side player_side
) noexcept
{
  switch (item)
  {
    case lobby_view_item::color: return layout.get_color(player_side);
    case lobby_view_item::race: return layout.get_race(player_side);
    default:
    case lobby_view_item::start:
      assert(item == lobby_view_item::start);
      return layout.get_start(player_side);
  }
}

void test_lobby_view_layout()
{
  #ifndef NDEBUG
  // get_panels
  {
    const lobby_view_layout layout;
    assert(!get_panels(layout).empty());
  }
  // get_cursor_rect
  {
    const lobby_view_layout layout;
    const auto r1{get_cursor_rect(layout, lobby_view_item::color, side::lhs)};
    const auto r2{get_cursor_rect(layout, lobby_view_item::race, side::lhs)};
    const auto r3{get_cursor_rect(layout, lobby_view_item::start, side::lhs)};
    const auto r4{get_cursor_rect(layout, lobby_view_item::color, side::rhs)};
    const auto r5{get_cursor_rect(layout, lobby_view_item::race, side::rhs)};
    const auto r6{get_cursor_rect(layout, lobby_view_item::start, side::rhs)};
    assert(r1 != r2);
    assert(r1 != r3);
    assert(r1 != r4);
    assert(r1 != r5);
    assert(r1 != r6);
    assert(r2 != r3);
    assert(r2 != r4);
    assert(r2 != r5);
    assert(r2 != r6);
    assert(r3 != r4);
    assert(r3 != r5);
    assert(r3 != r6);
    assert(r4 != r5);
    assert(r4 != r6);
    assert(r5 != r6);
  }
  #endif
}
