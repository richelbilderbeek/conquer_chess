#include "when_to_make_a_move_law.h"


#include "magic_enum/magic_enum.hpp" // https://github.com/Neargye/magic_enum

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

when_to_make_a_move_law get_next(const when_to_make_a_move_law speed) noexcept
{
  const auto v{get_all_when_to_make_a_move_laws()};
  auto there{std::find(std::begin(v), std::end(v), speed)};
  assert(there != std::end(v));
  if (there == std::end(v) - 1)
  {
    assert(!v.empty());
    const auto t{v.front()};
    return t;
  }
  return *(++there);
}

when_to_make_a_move_law get_previous(const when_to_make_a_move_law speed) noexcept
{
  const auto v{get_all_when_to_make_a_move_laws()};
  auto there{std::find(std::begin(v), std::end(v), speed)};
  assert(there != std::end(v));
  if (there == std::begin(v))
  {
    assert(!v.empty());
    const auto t{v.back()};
    return t;
  }
  return *(--there);
}

void test_when_to_make_a_move_law()
{
#ifndef NDEBUG
  // get_all_when_to_make_a_move_laws
  {
    assert(!get_all_when_to_make_a_move_laws().empty());
  }
  // get_next
  {
    for (const auto law: get_all_when_to_make_a_move_laws())
    {
      const auto next{get_next(law)};
      assert(next != law);
    }
  }
  // get_previous
  {
    for (const auto law: get_all_when_to_make_a_move_laws())
    {
      const auto next{get_next(law)};
      const auto previous{get_previous(next)};
      assert(previous == law);
    }
  }
  // to_str
  {
    assert(to_str(when_to_make_a_move_law::classic) == "classic");
  }
  // to_str
  {
    assert(to_human_str(when_to_make_a_move_law::classic) == "Classic");
    assert(to_human_str(when_to_make_a_move_law::rts) == "RTS");
    assert(to_human_str(when_to_make_a_move_law::white_first) == "White first");
  }
#endif // NDEBUG
}

std::string to_human_str(const when_to_make_a_move_law law) noexcept
{
  switch (law)
  {
    case when_to_make_a_move_law::classic: return "Classic";
    case when_to_make_a_move_law::rts: return "RTS";
    default:
    case when_to_make_a_move_law::white_first:
      assert(law == when_to_make_a_move_law::white_first);
      return "White first";
  }
}

std::string to_str(const when_to_make_a_move_law law) noexcept
{
  return std::string(magic_enum::enum_name(law));
}
