#include "helper.h"

#include <cassert>

// Allow Windows to find M_PI, thanks https://stackoverflow.com/a/26065595/3364162
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <sstream>
#include <vector>

std::string bool_to_str(const bool b) noexcept
{
  return b ? "true" : "false";
}

double calc_angle_degrees(const double dx, const double dy)
{
  return calc_angle_radians(dx, dy) * 360.0 / (2.0 * M_PI);
}

double calc_angle_radians(const double dx, const double dy)
{
  return std::atan2(dx,dy) - (0.5 * M_PI);
}

double calc_distance(const double dx, const double dy) noexcept
{
  return std::sqrt((dx * dx) + (dy * dy));
}


std::vector<int> make_sequence(
  const int from,
  const int to,
  const int increment
)
{
  assert(increment >= 1);
  assert((from - to) % increment == 0);
  std::vector<int> v;
  v.reserve((std::abs(to - from) / increment) + 1);
  if (to > from)
  {
    for (int i{from}; i != to; i+=increment)
    {
      v.push_back(i);
    }
  }
  else
  {
    for (int i{from}; i != to; i-=increment)
    {
      v.push_back(i);
    }
  }
  v.push_back(to);
  return v;
}

// https://github.com/richelbilderbeek/cpp/blob/master/content/CppFileToVector.md
std::vector<std::string> read_lines(const std::string& filename)
{
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  while(!in.eof())
  {
    std::string s;
    std::getline(in, s);
    v.push_back(s);
  }
  return v;
}

std::vector<std::string> split_str(
  const std::string& s,
  const char seperator
)
{
  std::istringstream is(s);
  std::vector<std::string> v;
  for (
    std::string sub;
    std::getline(is, sub, seperator);
    v.push_back(sub))
  {
    //Empty for loop
  }
  return v;
}

void test_helper()
{
#ifndef NDEBUG
  // bool_to_str
  {
    assert(bool_to_str(true) == "true");
    assert(bool_to_str(false) == "false");
  }
  // calc_angle_degreees
  {
    const double tolerance{0.1};
    {
      // 3 o'clock
      const double expected{0.0};
      const double created{calc_angle_degrees( 1.0,  0.0)};
      assert(is_close(expected, created, tolerance));
    }
    {
      // 6 o'clock
      const double expected{-90.0};
      const double created{calc_angle_degrees( 0.0,  1.0)};
      assert(is_close(expected, created, tolerance));
    }
    {
      // 9 o'clock
      const double expected{-180.0};
      const double created{calc_angle_degrees(-1.0,  0.0)};
      assert(is_close(expected, created, tolerance));
    }
    {
      // 12 o'clock
      const double expected{90.0};
      const double created{calc_angle_degrees( 0.0, -1.0)};
      assert(is_close(expected, created, tolerance));
    }
  }
  // calc_angle_radians
  {
    const double tolerance{0.1};
    {
      // 3 o'clock
      const double expected{0.0 * M_PI};
      const double created{calc_angle_radians( 1.0,  0.0)};
      assert(is_close(expected, created, tolerance));
    }
    {
      // 6 o'clock
      const double expected{(1.5 * M_PI) - (2.0 * M_PI)};
      const double created{calc_angle_radians( 0.0,  1.0)};
      assert(is_close(expected, created, tolerance));
    }
    {
      // 9 o'clock
      const double expected{(1.0 * M_PI) - (2.0 * M_PI)};
      const double created{calc_angle_radians(-1.0,  0.0)};
      assert(is_close(expected, created, tolerance));
    }
    {
      // 12 o'clock
      const double expected{(0.5 * M_PI) + (0.0 * M_PI)};
      const double created{calc_angle_radians( 0.0, -1.0)};
      assert(is_close(expected, created, tolerance));
    }
  }
  {
    std::vector<int> v = {1, 2, 3};
    remove_first(v);
    assert(v == std::vector<int>( {2, 3} ) );
    remove_first(v);
    assert(v == std::vector<int>( {3} ) );
  }
  // make_sequence, no intermediates
  {
    const auto v{make_sequence(42, 43)};
    assert(v.size() == 2);
    assert(v[0] == 42);
    assert(v[1] == 43);
  }
  // make_sequence, many intermediates
  {
    const auto v{make_sequence(314, 271)};
    assert(v.size() > 2);
    assert(v[0] == 314);
    assert(v[1] == 313);
    assert(v.back() == 271);
  }
  {
    const std::string s("A");
    const std::wstring expected(L"A");
    const std::wstring result{to_wstring(s)};
    assert(expected == result);
  }
  // read_lines
  {
    const std::string filename{"tmp_read_lines_test.txt"};
    std::ofstream f(filename);
    f << std::string("test text");
    f.close();
    assert(read_lines(filename).size() == 1);
  }
  // to_comma_seperated_str
  {
    std::vector<std::string> v1;
    assert(to_comma_seperated_str(v1) == "");
    std::vector<std::string> v2 = { "A" };
    assert(to_comma_seperated_str(v2) == "A");
    std::vector<std::string> v3 = { "A", "B" };
    assert(to_comma_seperated_str(v3) == "A,B");
    std::vector<std::string> v4 = { "A", "B", "C" };
    assert(to_comma_seperated_str(v4) == "A,B,C");
  }
#endif
}

template<class T>
std::string to_comma_seperated_str_impl(const std::vector<T>& v)
{
  std::stringstream s;
  for (const auto& t: v)
  {
    s << t << ',';
  }
  std::string t{s.str()};
  if (!t.empty()) t.pop_back();
  return t;
}

std::string to_comma_seperated_str(const std::vector<double>& v)
{
  return to_comma_seperated_str_impl(v);
}

std::string to_comma_seperated_str(const std::vector<std::string>& v)
{
  return to_comma_seperated_str_impl(v);
}

std::wstring to_wstring(const std::string s)
{
  std::wstring ws(std::begin(s), std::end(s));
  return ws;
}

