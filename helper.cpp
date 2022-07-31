#include "helper.h"

#include <cassert>
#include <cmath>
#include <sstream>
#include <vector>

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
#endif
}
