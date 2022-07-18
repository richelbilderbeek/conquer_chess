#include "chess_color.h"

#include <cassert>

chess_color get_other_color(const chess_color c)
{
  if (c == chess_color::white) return chess_color::black;
  return chess_color::white;
}

void test_chess_color()
{
#ifndef NDEBUG
  {
    assert(get_other_color(chess_color::white) == chess_color::black);
    assert(get_other_color(chess_color::black) == chess_color::white);
  }
#endif // DEBUG
}
