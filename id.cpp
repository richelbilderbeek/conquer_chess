#include "id.h"

#include <cassert>

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
