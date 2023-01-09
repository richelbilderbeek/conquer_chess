#include "asserts.h"

#include <cassert>

void test_asserts()
{
#ifndef NDEBUG
  // assert_eq: OK
  {
    const std::string s{"something irrelevant"};
    const std::string t{s};
    assert_eq(s, t);
  }
  // assert_eq: different
  {
    const std::string s{"A"};
    const std::string t{"B"};
    bool has_thrown{false};
    try
    {
      assert_eq(s, t);
    }
    catch (const std::logic_error& e)
    {
      assert(e.what());
      has_thrown = true;
    }
    assert(has_thrown);
  }
#endif // NDEBUG
}

