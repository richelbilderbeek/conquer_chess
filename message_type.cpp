#include "message_type.h"

#include <cassert>
#include <iostream>
#include <sstream>

std::vector<message_type> get_all_message_types() noexcept
{
  return
  {
    message_type::cannot,
    message_type::done,
    message_type::select,
    message_type::start_attack,
    message_type::start_move
  };
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
  switch (t)
  {
    case message_type::cannot: return "cannot";
    case message_type::done: return "done";
    case message_type::select: return "select";
    case message_type::start_attack: return "start_attack";
    default:
    case message_type::start_move:
      assert(t == message_type::start_move);
      return "start_move";
  }
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
