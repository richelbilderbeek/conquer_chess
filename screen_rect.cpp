#include "screen_rect.h"

#include <cassert>
#include <iostream>
#include <sstream>

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

void test_screen_rect()
{
#ifndef NDEBUG
  // get_width and get_height
  {
    //   012345678901
    // 0 ............
    // 1 ..+---|---+.
    // 2 ..|   |   |.
    // 3 ..----X----.
    // 4 ..|   |   |.
    // 5 ..+---|---+.
    // 6 ............

    const screen_rect r(screen_coordinat(2, 1), screen_coordinat(10, 5));
    assert(get_width(r) == 8); // The bottom-left is excluded?
    assert(get_height(r) == 4); // The bottom-left is excluded?
  }
  // get_center
  {
    //   012345678901
    // 0 ............
    // 1 ..+---|---+.
    // 2 ..|   |   |.
    // 3 ..----X----.
    // 4 ..|   |   |.
    // 5 ..+---|---+.
    // 6 ............

    const screen_rect r(screen_coordinat(2, 1), screen_coordinat(10, 5));
    const auto c{get_center(r)};
    assert(c.get_x() == 6);
    assert(c.get_y() == 3);
  }
  // operator<<
  {
    std::stringstream s;
    s << screen_rect();
    assert(!s.str().empty());
  }
#endif // DEBUG
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
