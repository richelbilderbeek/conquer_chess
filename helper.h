#ifndef HELPER_H
#define HELPER_H

#include <cassert>
#include <cmath>

template <class T> bool is_close(const T& lhs, const T& rhs, const T& max)
{
  return std::abs(lhs - rhs) < max;
}

template <class T>
void remove_first(T& v)
{
  assert(!v.empty());
  v.erase(v.begin());
}

/// Test the help functions
void test_helper();

#endif // HELPER_H
