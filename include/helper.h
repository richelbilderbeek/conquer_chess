#ifndef HELPER_H
#define HELPER_H

/// @file Helper functions

#include <algorithm>
#include <cassert>
#include <cmath>
#include <string>
#include <vector>

/// Convert 'true' to 'true' and 'false' to 'false'
std::string bool_to_str(const bool b) noexcept;

/// Calculate the angle in degrees.
///
/// Calculate the angle in degrees.
///
/// dx | dy | o'clock | degrees
/// ---|----|---------|--------
///  1 |  0 |       3 | 0
///  0 |  1 |       6 | 270
/// -1 |  0 |       9 | 180
///  0 | -1 |      12 | 90
double calc_angle_degrees(const double dx, const double dy);

/// Calculate the angle in radians.
///
/// Calculate the angle in radians.
///
/// dx | dy | o'clock | radians
/// ---|----|---------|---------
///  1 |  0 |       3 | 0.0 * pi
///  0 |  1 |       6 | 1.5 * pi
/// -1 |  0 |       9 | 1.0 * pi
///  0 | -1 |      12 | 0.5 * pi
double calc_angle_radians(const double dx, const double dy);

/// Calculate the Euclidean distance between two points.
///
/// Calculate the Euclidean distance between two points.
double calc_distance(const double dx, const double dy) noexcept;

/// Get pi
///
/// Cannot use `M_PI`, as I have no bright ideas how
/// to get it from MSVC's STL.
///
/// I copied the value from the GCC STL.
constexpr double get_pi() noexcept { return 3.14159265358979323846; }

/// Determine if the difference between two values is less
/// than the maximum tolerated value.
template <class T> bool is_close(const T& lhs, const T& rhs, const T& max)
{
  return std::abs(lhs - rhs) < max;
}

/// Determine if a value if part of a collection
template <class T> bool is_present_in(
  const T& element,
  const std::vector<T>& collection
)
{
  return std::find(
    std::begin(collection),
    std::end(collection),
    element
  ) != std::end(collection);
}

/// Make a sequence in an inclusive way.
///
/// Make a sequence in an inclusive way:
/// the first element will be 'from',
/// the last element will be 'to'
/// Assumes 'from' and 'to' are different
std::vector<int> make_sequence(
  const int from,
  const int to,
  const int increment = 1
);

/// Convert a file's content to a collection of strings.
/// @note Adapted from \url{https://github.com/richelbilderbeek/cpp/blob/master/content/CppFileToVector.md}
std::vector<std::string> read_lines(const std::string& filename);

/// Remove the first element from a collection.
///
/// Remove the first element from a collection.
template <class T>
void remove_first(T& v)
{
  assert(!v.empty());
  v.erase(v.begin());
}

/// Split a string.
///
/// @see From https://www.richelbilderbeek.nl/CppSeperateString.htm
std::vector<std::string> split_str(
  const std::string& s,
  const char seperator = ' '
);

/// Test the help functions.
///
/// Test the help functions.
void test_helper();

/// Convert a vector to one comma seperated string
// No templates, to reduce compile-time
std::string to_comma_seperated_str(const std::vector<double>& v);

/// Convert a vector to one comma seperated string
// No templates, to reduce compile-time
std::string to_comma_seperated_str(const std::vector<std::string>& v);


/// Convert a string to a wide string
std::wstring to_wstring(const std::string s);

#endif // HELPER_H
