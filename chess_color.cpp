#include "chess_color.h"

#include <cassert>
#include <iostream>

chess_color get_other_color(const chess_color c) noexcept
{
  if (c == chess_color::white) return chess_color::black;
  assert(c == chess_color::black);
  return chess_color::white;
}

void test_chess_color()
{
#ifndef NDEBUG
  // to_str
  {
    assert(to_str(chess_color::white) == "white");
    assert(to_str(chess_color::black) == "black");
  }
  // get_other_color
  {
    assert(get_other_color(chess_color::white) == chess_color::black);
    assert(get_other_color(chess_color::black) == chess_color::white);
  }
#endif // DEBUG
}

std::string to_str(const chess_color c) noexcept
{
  if (c == chess_color::white) return "white";
  assert(c == chess_color::black);
  return "black";
}

std::ostream& operator<<(std::ostream& os, const chess_color c) noexcept
{
  os << to_str(c);
  return os;
}
