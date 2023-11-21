#include "message_type.h"

#include <magic_enum/magic_enum.hpp> // https://github.com/Neargye/magic_enum

#include <cassert>
#include <iostream>
#include <sstream>

std::vector<message_type> get_all_message_types() noexcept
{
  const auto a{magic_enum::enum_values<message_type>()};
  std::vector<message_type> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

void test_message_type()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(message_type::cannot) == "cannot");
    assert(to_str(message_type::done) == "done");
    assert(to_str(message_type::select) == "select");
    assert(to_str(message_type::start_attack) == "start_attack");
    assert(to_str(message_type::start_move) == "start_move");
  }
  // to_str on std::vector
  {
    assert(!to_str(get_all_message_types()).empty());
  }
  // operator<<
  {
    std::stringstream s;
    s << message_type::cannot;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

std::string to_str(const message_type t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}

std::string to_str(const std::vector<message_type>& message_types) noexcept
{
  std::stringstream s;
  for (const auto message_type: message_types)
  {
    s << message_type << '\n';
  }
  std::string t{s.str()};
  if (t.empty()) return t;
  t.pop_back();
  return t;
}

std::ostream& operator<<(std::ostream& os, const message_type t) noexcept
{
  os << to_str(t);
  return os;
}
