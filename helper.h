#ifndef HELPER_H
#define HELPER_H

#include <cmath>

template <class T> bool is_close(const T& lhs, const T& rhs, const T& max)
{
  return std::abs(lhs - rhs) < max;
}


#endif // HELPER_H
