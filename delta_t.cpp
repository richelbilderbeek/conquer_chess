#include "delta_t.h"

#include <cassert>
#include <iostream>
#include <sstream>

delta_t::delta_t(const double dt)
  : m_delta_t{dt}
{

}

void test_delta_t()
{
#ifndef NDEBUG
  // get
  {
    const double dt{0.123};
    const delta_t t{dt};
    assert(t.get() == dt);
  }
  // operator==
  {
    const delta_t a(0.1);
    const delta_t b(0.1);
    const delta_t c(0.9876);
    assert(a == b);
    assert(!(a == c));
  }
  // operator<
  {
    const delta_t a(0.1);
    const delta_t b(0.2);
    assert(a < b);
  }
  // operator+
  {
    const double t1{0.1};
    const double t2{0.2};
    const delta_t d1{t1};
    const delta_t d2{t2};
    const delta_t d3{d1 + d2};
    assert(d3.get() == t1 + t2);
  }
  // operator-
  {
    const double t1{0.5};
    const double t2{0.3};
    const delta_t d1{t1};
    const delta_t d2{t2};
    const delta_t d3{d1 - d2};
    assert(d3.get() == t1 - t2);
  }
  // operator*
  {
    const double t1{0.1};
    const double t2{0.2};
    const delta_t d1{t1};
    const delta_t d2{t2};
    const delta_t d3{d1 * d2};
    assert(d3.get() == t1 * t2);
  }
  // operator+=
  {
    const double t1{0.1};
    const double t2{0.2};
    delta_t d1{t1};
    const delta_t d2{t2};
    d1 += d2;
    assert(d1.get() == t1 + t2);
  }
  // operator>
  {
    const delta_t low{0.0001};
    const delta_t high{0.11};
    assert(high > low);
  }
  // operator>=
  {
    const delta_t low{0.0001};
    const delta_t high{0.11};
    assert(high >= low);
  }
  // operator<<
  {
    std::stringstream s;
    s << delta_t(0.123);
    assert(!s.str().empty());
  }
#endif // DEBUG
}

bool operator==(const delta_t& lhs, const delta_t& rhs) noexcept
{
  return lhs.get() == rhs.get();
}

bool operator<(const delta_t& lhs, const delta_t& rhs) noexcept
{
  return lhs.get() < rhs.get();
}

delta_t& operator+=(delta_t& lhs, const delta_t& rhs) noexcept
{
  lhs = lhs + rhs;
  return lhs;
}

delta_t operator+(const delta_t& lhs, const delta_t& rhs) noexcept
{
  return delta_t(lhs.get() + rhs.get());
}

delta_t operator-(const delta_t& lhs, const delta_t& rhs) noexcept
{
  return delta_t(lhs.get() - rhs.get());
}

delta_t operator*(const delta_t& lhs, const delta_t& rhs) noexcept
{
  return delta_t(lhs.get() * rhs.get());
}

bool operator>(const delta_t& lhs, const delta_t& rhs) noexcept
{
  return lhs.get() > rhs.get();
}

bool operator>=(const delta_t& lhs, const delta_t& rhs) noexcept
{
  return lhs.get() >= rhs.get();
}

std::ostream& operator<<(std::ostream& os, const delta_t& dt) noexcept
{
  os << dt.get();
  return os;
}
