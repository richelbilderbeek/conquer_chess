#include "castling_type.h"

#include "../magic_enum/include/magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

#include <iterator>

std::vector<castling_type> get_all_castling_types() noexcept
{
  const auto a{magic_enum::enum_values<castling_type>()};
  std::vector<castling_type> v;
  v.reserve(a.size());
  std::copy(std::begin(a), std::end(a), std::back_inserter(v));
  assert(a.size() == v.size());
  return v;
}

void test_castling_type()
{
#ifndef NDEBUG
  // get_all_castling_types
  {
    assert(!get_all_castling_types().empty());
  }
  // to_str
  {
    assert(to_str(castling_type::king_side) == "king_side");
    assert(to_str(castling_type::queen_side) == "queen_side");
  }
#endif // NDEBUG
}

std::string to_str(const castling_type t) noexcept
{
  return std::string(magic_enum::enum_name(t));
}
