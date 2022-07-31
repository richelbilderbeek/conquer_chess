#include "id.h"

#include <cassert>
#include <iostream>
#include <sstream>

int id::sm_next_value{0};

id::id()
  : m_value{sm_next_value++}
{

}

id create_new_id() noexcept
{
  return id();
}

void test_id()
{
#ifndef NDEBUG
  {
    const auto a{create_new_id()};
    const auto b{create_new_id()};
    assert(a == a);
    assert(!(a == b));
    assert(a != b);
  }
  // operator<<
  {
    const id i = create_new_id();
    std::stringstream s;
    s << i;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const id& lhs, const id& rhs) noexcept
{
  return lhs.get() == rhs.get();
}

bool operator!=(const id& lhs, const id& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const id& i) noexcept
{
  os << i.get();
  return os;
}
