#include "helper.h"

#include <cassert>
#include <vector>

void test_helper()
{
#ifndef NDEBUG
  {
    std::vector<int> v = {1, 2, 3};
    remove_first(v);
    assert(v == std::vector<int>( {2, 3} ) );
    remove_first(v);
    assert(v == std::vector<int>( {3} ) );
  }
#endif
}
