#include "chess_color.h"

chess_color get_other_color(const chess_color c)
{
  if (c == chess_color::white) return chess_color::black;
  return chess_color::white;
}
