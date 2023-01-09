#include "read_only.h"
#include <cassert>
#include <vector>
#include <exception>

template <typename T>
bool operator==(const read_only<T>& lhs, const read_only<T>& rhs) noexcept
{
  return lhs.get_value() == rhs.get_value();
}

template <typename T>
bool operator!=(const read_only<T>& lhs, const read_only<T>& rhs) noexcept
{
  return lhs.get_value() != rhs.get_value();
}

template <typename T>
bool operator<(const read_only<T>& lhs, const read_only<T>& rhs) noexcept
{
  return lhs.get_value() < rhs.get_value();
}

template <typename T>
bool operator>(const read_only<T>& lhs, const read_only<T>& rhs) noexcept
{
  return lhs.get_value() > rhs.get_value();
}

template <typename T>
bool operator<=(const read_only<T>& lhs, const read_only<T>& rhs) noexcept
{
    return lhs.get_value() <= rhs.get_value();
}

template <typename T>
bool operator>=(const read_only<T>& lhs, const read_only<T>& rhs) noexcept
{
    return lhs.get_value() >= rhs.get_value();
}

void test_read_only()
{
#ifndef NDEBUG // no tests in release
  // basics
  {
    read_only<int> x(314); // Can only determine value at construction
    assert(x.get_value() == 314); // Can only get, not set
    read_only<int> x_again = x; // Can copy
    assert(x.get_value() == x_again.get_value());
    assert(x == x_again); // Can compare for equality
    read_only<int> x_other(271);
    assert(x != x_other); // Can compare for inequality
    assert(x > x_other);  // Can compare
    assert(x >= x_other); // Can compare
    assert(x_other < x);  // Can compare
    assert(x_other <= x); // Can compare
  }
  // double
  {
    read_only<double> y(3.14);
    assert(y.get_value() == 3.14);
  }
  // std::string
  {
    read_only<std::string> s("pi");
    assert(s.get_value() == "pi");
  }
  // no value
  {
    read_only<std::string> s;
    assert(!s.has_value());
    bool has_thrown{false};
    try
    {
      assert(s.get_value() == "irrelevant");
    }
    catch (const std::logic_error& e)
    {
      assert(e.what());
      has_thrown = true;
    }
    assert(has_thrown);
  }
#endif
}


