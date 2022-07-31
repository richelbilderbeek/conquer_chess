#ifndef HELPER_H
#define HELPER_H

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

template <class T> bool is_close(const T& lhs, const T& rhs, const T& max)
{
  return std::abs(lhs - rhs) < max;
}

/// Make a sequence in an incluse way:
/// the first element will be 'from',
/// the last element will be 'to'
/// Assumes 'from' and 'to' are different
std::vector<int> make_sequence(
  const int from,
  const int to,
  const int increment = 1
);

template <class T>
void remove_first(T& v)
{
  assert(!v.empty());
  v.erase(v.begin());
}

/// Split a string, from https://www.richelbilderbeek.nl/CppSeperateString.htm
std::vector<std::string> split_str(
  const std::string& s,
  const char seperator = ' '
);

/// Test the help functions
void test_helper();

#endif // HELPER_H
