#include "when_to_make_a_move_law.h"


#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

#include <cassert>
#include <iterator>
#include <iterator>

std::vector<when_to_make_a_move_law> get_all_when_to_make_a_move_laws() noexcept
{
  const auto a{magic_enum::enum_values<when_to_make_a_move_law>()};
  std::vector<when_to_make_a_move_law> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

void test_when_to_make_a_move_law()
{
#ifndef NDEBUG
  // get_all_when_to_make_a_move_laws
  {
    assert(!get_all_when_to_make_a_move_laws().empty());
  }
  // to_str
  {
    assert(to_str(when_to_make_a_move_law::classic) == "classic");
  }
#endif // NDEBUG
}

std::string to_str(const when_to_make_a_move_law t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}
