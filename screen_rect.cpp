#include "screen_rect.h"

#include <iostream>

screen_rect::screen_rect(
  const screen_coordinat& top_left,
  const screen_coordinat& bottom_right
) : m_top_left{top_left},
    m_bottom_right{bottom_right}
{

}

screen_coordinat get_center(const screen_rect& r) noexcept
{
  return screen_coordinat(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
}

int get_height(const screen_rect& r) noexcept
{
  return r.get_br().get_y() - r.get_tl().get_y();
}
int get_width(const screen_rect& r) noexcept
{
  return r.get_br().get_x() - r.get_tl().get_x();
}

screen_rect& operator+=(screen_rect& rect, const screen_coordinat& delta) noexcept
{
  rect = screen_rect(
    rect.get_tl() + delta,
    rect.get_br() + delta
  );
  return rect;
}

std::ostream& operator<<(std::ostream& os, const screen_rect& r) noexcept
{
  os << "(" << r.get_tl() << "-" << r.get_br() << ")";
  return os;
}
