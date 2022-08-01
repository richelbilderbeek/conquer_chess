#include "screen_coordinat.h"

#include "helper.h"
#include <cassert>
#include <iostream>

screen_coordinat::screen_coordinat(const int x, const int y)
  : m_x{x}, m_y{y}
{

}

double calc_angle_degrees(const screen_coordinat& from, const screen_coordinat& to)
{
  return calc_angle_degrees(to - from);
}

double calc_angle_degrees(const screen_coordinat& delta)
{
  return calc_angle_degrees(
    delta.get_x(),
    delta.get_y()
  );
}


double calc_distance(const screen_coordinat& a, const screen_coordinat& b) noexcept
{
  const auto delta{b - a};
  return calc_distance(delta.get_x(), delta.get_y());
}

screen_coordinat get_default_screen_size() noexcept
{
  //return screen_coordinat(1024, 576);
  //return screen_coordinat(1024 + 400, 576);
  return screen_coordinat(1024 + 400, 576 + 400);
}

screen_coordinat get_default_main_menu_screen_size() noexcept
{
  return screen_coordinat(1100, 900);
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

screen_coordinat operator-(const screen_coordinat& lhs, const screen_coordinat& rhs) noexcept
{
  return screen_coordinat(
    lhs.get_x() - rhs.get_x(),
    lhs.get_y() - rhs.get_y()
  );
}

screen_coordinat operator*(const screen_coordinat& lhs, const double& factor) noexcept
{
  return screen_coordinat(
    static_cast<double>(lhs.get_x()) * factor,
    static_cast<double>(lhs.get_y()) * factor
  );
}

screen_coordinat operator/(const screen_coordinat& lhs, const double& factor)
{
  assert(factor != 0.0);
  return screen_coordinat(
    static_cast<double>(lhs.get_x()) / factor,
    static_cast<double>(lhs.get_y()) / factor
  );
}

screen_coordinat& operator+=(screen_coordinat& lhs, const screen_coordinat& rhs) noexcept
{
  lhs = lhs + rhs;
  return lhs;
}

bool operator==(const screen_coordinat& lhs, const screen_coordinat& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x()
    && lhs.get_y() == rhs.get_y()
  ;
}

void test_screen_coordinat()
{
  #ifndef NDEBUG
  // calc_angle_degrees, one screen_coordinat
  {
    const screen_coordinat c(1, 0);
    assert(calc_angle_degrees(c) == 0.0);
  }
  // calc_angle_degrees, two screen_coordinat
  {
    const screen_coordinat c(0, 0);
    const screen_coordinat d(1, 0);
    assert(calc_angle_degrees(c, d) == 0.0);
  }
  // calc_distance
  {
    const screen_coordinat c(0, 0);
    const screen_coordinat d(3, 4);
    assert(calc_distance(c, d) == 5.0);
  }
  // get_default_main_menu_screen_size
  {
    assert(get_default_main_menu_screen_size().get_x() > 0);
    assert(get_default_main_menu_screen_size().get_y() > 0);
  }
  // get_default_screen_size
  {
    assert(get_default_screen_size().get_x() > 0);
    assert(get_default_screen_size().get_y() > 0);
  }

  // operator+
  {
    const screen_coordinat c_1(1, 2);
    const screen_coordinat c_2(3, 4);
    const screen_coordinat c_3{c_1 + c_2};
    assert(c_1.get_x() + c_2.get_x() == c_3.get_x());
    assert(c_1.get_y() + c_2.get_y() == c_3.get_y());
  }
  // operator-
  {
    const screen_coordinat c_1(1, 2);
    const screen_coordinat c_2(3, 4);
    const screen_coordinat c_3{c_1 - c_2};
    assert(c_1.get_x() - c_2.get_x() == c_3.get_x());
    assert(c_1.get_y() - c_2.get_y() == c_3.get_y());
  }
  // operator*
  {
    const screen_coordinat c_1(100, 200);
    const double f{1.2};
    const screen_coordinat c_2{c_1 * f};
    assert(c_1.get_x() * f == c_2.get_x());
    assert(c_1.get_y() * f == c_2.get_y());
  }
  // operator/
  {
    const screen_coordinat c_1(100, 200);
    const double f{1.2};
    const screen_coordinat c_2{c_1 / f};
    const int expected_x{static_cast<int>(static_cast<double>(c_1.get_x()) / f)};
    const int expected_y{static_cast<int>(static_cast<double>(c_1.get_y()) / f)};
    assert(expected_x == c_2.get_x());
    assert(expected_y == c_2.get_y());
  }
  // operator+=
  {
    const screen_coordinat c_1(1, 2);
    const screen_coordinat c_2(3, 4);
    screen_coordinat c_3{c_1};
    c_3 += c_2;
    assert(c_1.get_x() + c_2.get_x() == c_3.get_x());
    assert(c_1.get_y() + c_2.get_y() == c_3.get_y());
  }
  // operator==
  {
    const screen_coordinat a(1, 2);
    const screen_coordinat b(1, 2);
    const screen_coordinat c(3, 4);
    const screen_coordinat d(1, 3);
    const screen_coordinat e(4, 2);
    assert(a == b);
    assert(!(a == c));
    assert(!(a == d));
    assert(!(a == e));
  }
  #endif
}
