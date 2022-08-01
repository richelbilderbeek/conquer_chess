#ifndef HELPER_H
#define HELPER_H

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

/// Calculate the angle in degrees
/// dx | dy | o'clock | degrees
/// ---|----|---------|--------
///  1 |  0 |       3 | 0
///  0 |  1 |       6 | 270
/// -1 |  0 |       9 | 180
///  0 | -1 |      12 | 90
double calc_angle_degrees(const double dx, const double dy);

/// Calculate the angle in radians
/// dx | dy | o'clock | radians
/// ---|----|---------|---------
///  1 |  0 |       3 | 0.0 * pi
///  0 |  1 |       6 | 1.5 * pi
/// -1 |  0 |       9 | 1.0 * pi
///  0 | -1 |      12 | 0.5 * pi
double calc_angle_radians(const double dx, const double dy);

/// Calculate the Euclidean distance between two points
double calc_distance(const double dx, const double dy) noexcept;

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
