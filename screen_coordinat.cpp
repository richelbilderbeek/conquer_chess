#include "screen_coordinat.h"

#include <cassert>
#include <iostream>

screen_coordinat::screen_coordinat(const int x, const int y)
  : m_x{x}, m_y{y}
{

}

screen_coordinat get_default_screen_size() noexcept
{
  return screen_coordinat(1024, 576);
  //return screen_coordinat(1024 + 800, 576);
}

std::ostream& operator<<(std::ostream& os, const screen_coordinat& coordinat)
{
  os << "(" << coordinat.get_x() << ","  << coordinat.get_y() << ")";
  return os;
}

screen_coordinat operator+(const screen_coordinat& lhs, const screen_coordinat& rhs) noexcept
{
  return screen_coordinat(
    lhs.get_x() + rhs.get_x(),
    lhs.get_y() + rhs.get_y()
  );
}

screen_coordinat& operator+=(screen_coordinat& lhs, const screen_coordinat& rhs) noexcept
{
  lhs = lhs + rhs;
  return lhs;
}

void test_screen_coordinat()
{
  #ifndef NDEBUG
  {
    const screen_coordinat c_1(1, 2);
    const screen_coordinat c_2(3, 4);
    const screen_coordinat c_3{c_1 + c_2};
    assert(c_1.get_x() + c_2.get_x() == c_3.get_x());
    assert(c_1.get_y() + c_2.get_y() == c_3.get_y());
  }
  {
    const screen_coordinat c_1(1, 2);
    const screen_coordinat c_2(3, 4);
    screen_coordinat c_3{c_1};
    c_3 += c_2;
    assert(c_1.get_x() + c_2.get_x() == c_3.get_x());
    assert(c_1.get_y() + c_2.get_y() == c_3.get_y());
  }
  #endif
}
