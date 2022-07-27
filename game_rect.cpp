#include "game_rect.h"

#include <cassert>
#include <iostream>
#include <sstream>

game_rect::game_rect(
  const game_coordinat& top_left,
  const game_coordinat& bottom_right
) : m_top_left{top_left},
    m_bottom_right{bottom_right}
{

}

game_coordinat get_center(const game_rect& r) noexcept
{
  return game_coordinat(
    (r.get_tl().get_x() + r.get_br().get_x()) / 2,
    (r.get_tl().get_y() + r.get_br().get_y()) / 2
  );
}

int get_height(const game_rect& r) noexcept
{
  return r.get_br().get_y() - r.get_tl().get_y();
}
int get_width(const game_rect& r) noexcept
{
  return r.get_br().get_x() - r.get_tl().get_x();
}

bool is_in(const game_coordinat& pos, const game_rect& r) noexcept
{
  return pos.get_x() >= r.get_tl().get_x()
    && pos.get_x() <= r.get_br().get_x()
    && pos.get_y() >= r.get_tl().get_y()
    && pos.get_y() <= r.get_br().get_y()
  ;
}

void test_game_rect()
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

    const game_rect r(game_coordinat(2, 1), game_coordinat(10, 5));
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

    const game_rect r(game_coordinat(2, 1), game_coordinat(10, 5));
    const auto c{get_center(r)};
    assert(c.get_x() == 6);
    assert(c.get_y() == 3);
  }
  // is_in
  {
    const game_rect r(
      game_coordinat(10, 20),
      game_coordinat(100, 200)
    );
    assert(is_in(game_coordinat(50, 60), r));
    assert(!is_in(game_coordinat(0, 60), r));
    assert(!is_in(game_coordinat(1000, 60), r));
    assert(!is_in(game_coordinat(50, 0), r));
    assert(!is_in(game_coordinat(60, 600), r));
  }
  // operator==
  {
    const game_rect a;
    const game_rect b;
    const game_rect c(game_coordinat(1.1, 2.2), game_coordinat(3.3, 4.4));
    assert(a == b);
    assert(!(a == c));
  }
  // operator<<
  {
    std::stringstream s;
    s << game_rect();
    assert(!s.str().empty());
  }
#endif // DEBUG
}

bool operator==(const game_rect& lhs, const game_rect& rhs) noexcept
{
  return lhs.get_tl() == rhs.get_tl()
    && lhs.get_br() == rhs.get_br()
  ;
}

std::ostream& operator<<(std::ostream& os, const game_rect& r) noexcept
{
  os << "(" << r.get_tl() << "-" << r.get_br() << ")";
  return os;
}
