#include "replay.h"

#include <algorithm>
#include <cassert>

replay::replay(const std::string& pgn_str)
{
  assert(!pgn_str.empty());
  assert(std::count(std::begin(pgn_str), std::end(pgn_str), '\n') == 0);
}


std::string get_replay_str() noexcept
{
  return "1. e4 e6 2. d4 b6 3. a3 Bb7 4. Nc3 Nh6 5. Bxh6 gxh6 6. Be2 Qg5 7. Bg4 h5 8. Nf3 Qg6 9. Nh4 Qg5 10. Bxh5 Qxh4 11. Qf3 Kd8 12. Qxf7 Nc6 13. Qe8# 1-0";
}

std::string get_scholars_mate_png_str() noexcept
{
  return "1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0";
}

void test_replay()
{
#ifndef NDEBUG
  // Can use scholars mate
  {
    const replay r(get_scholars_mate_png_str());
  }
  assert(1 == 2);
#endif // NDEBUG
}
