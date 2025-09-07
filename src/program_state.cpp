#include "program_state.h"

#include <sstream>
#include <iterator>
#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

std::vector<program_state> get_all_program_states() noexcept
{
  const auto a{magic_enum::enum_values<program_state>()};
  std::vector<program_state> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

void test_program_state()
{
#ifndef NDEBUG
  // get_all_program_states
  {
    assert(!get_all_program_states().empty());
  }
  // to_str
  {
    assert(to_str(program_state::about) == "about");
    assert(to_str(program_state::game) == "game");
  }
  // operator<<
  {
    std::stringstream s;
    s << program_state::about;
    assert(s.str() == "about");
  }
#endif // NDEBUG
}

std::string to_str(const program_state t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}

std::ostream& operator<<(std::ostream& os, const program_state t) noexcept
{
  os << to_str(t);
  return os;
}
