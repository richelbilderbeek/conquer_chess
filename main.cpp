#include "game.h"
#include "game_resources.h"
#include "game_view.h"

#include <SFML/Graphics.hpp>

#include <cassert>
#include <chrono>
#include <iostream>

/// All tests are called from here, only in debug mode
void test()
{
#ifndef NDEBUG
  test_game();

#ifndef LOGIC_ONLY
  test_game_view();
  test_game_resources();
#endif // LOGIC_ONLY
#endif
}

int main() //!OCLINT tests may be long
{
  game_view gv;
}
