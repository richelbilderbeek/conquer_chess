#include "action_number.h"

#include <cassert>
#include <iostream>
#include <sstream>

action_number::action_number(const int number)
  : m_number{number}
{
  assert(m_number >= 1 && m_number <= 4);
}

std::vector<action_number> get_all_action_numbers() noexcept
{
  std::vector<action_number> v;
  v.reserve(4);
  for (int i{1}; i <= 4; ++i)
  {
    v.push_back(action_number(i));
  }
  assert(v.size() == 4);
  return v;
}

action_number get_next(const action_number& n) noexcept
{
  const int new_number{(n.get_number() % 4) + 1};
  assert(new_number >= 1 && new_number <= 4);
  return action_number(new_number);
}

action_number get_previous(const action_number& n) noexcept
{
  const int new_number{((n.get_number() + 2) % 4) + 1};
  assert(new_number >= 1 && new_number <= 4);
  return action_number(new_number);
}

void test_action_number()
{
#ifndef NDEBUG
  // Constructor
  {
    const action_number n(1);
    assert(n.get_number() == 1);
  }
  // get_all_action_numbers
  {
    assert(get_all_action_numbers().size() == 4);
  }
  // get_next
  {
    const action_number a(1);
    const action_number b(get_next(a));
    const action_number c(get_next(b));
    const action_number d(get_next(c));
    const action_number e(get_next(d));
    assert(b.get_number() == 2);
    assert(c.get_number() == 3);
    assert(d.get_number() == 4);
    assert(e.get_number() == 1);
  }
  // get_previous
  {
    const action_number a(3);
    const action_number b(get_previous(a));
    const action_number c(get_previous(b));
    const action_number d(get_previous(c));
    const action_number e(get_previous(d));
    assert(b.get_number() == 2);
    assert(c.get_number() == 1);
    assert(d.get_number() == 4);
    assert(e.get_number() == 3);
  }
  // operator==
  {
    const action_number a(1);
    const action_number b(1);
    const action_number c(2);
    assert(a == b);
    assert(!(a == c));
  }
  // operator<<
  {
    std::stringstream s;
    const action_number a(1);
    s << a;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const action_number& lhs, const action_number& rhs) noexcept
{
  return lhs.get_number() == rhs.get_number();
}

std::ostream& operator<<(std::ostream& os, const action_number& number) noexcept
{
  os << number.get_number();
  return os;
}
