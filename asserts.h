#ifndef ASSERTS_H
#define ASSERTS_H

#include <iostream>
#include <stdexcept>
#include <sstream>

template <class T>
void assert_eq(const T& lhs, const T& rhs)
{
  if (lhs != rhs)
  {
    std::stringstream s;
    s
      << "Assertion failed: 'lhs == rhs'\n"
      << "lhs: " << lhs << '\n'
      << "rhs: " << rhs
    ;
    std::cerr << s.str();
    throw std::logic_error(s.str());
  }
}


#endif // ASSERTS_H
