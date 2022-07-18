/// Use LOGIC_ONLY to be able to run on GHA

#include "game.h"
#include "game_resources.h"
#include "game_view.h"
#include "game_view_layout.h"
#include "menu_view.h"
#include "menu_view_layout.h"
#include "options_view_layout.h"
#include "screen_coordinat.h"

#include <SFML/Graphics.hpp>

#include <cassert>
#include <chrono>
#include <iostream>

/// All tests are called from here, only in debug mode
void test()
{
#ifndef NDEBUG
  test_piece_action_type();
  test_chess_color();
  test_game_coordinat();
  test_screen_coordinat();
  test_piece_action();
  test_game();
  test_game_view_layout();
  test_menu_view_layout();
  test_options_view_layout();
#ifndef LOGIC_ONLY
  test_game_resources();
  test_game_view();
#endif // LOGIC_ONLY
#endif
}

std::vector<std::string> collect_args(int argc, char **argv) {
  std::vector<std::string> v(argv, argv + argc);
  return v;
}

int main(int argc, char **argv) //!OCLINT tests may be long
{
  #ifndef NDEBUG
  test();
  #endif
  const auto args = collect_args(argc, argv);
  if (args.size() == 1)
  {
    #ifndef LOGIC_ONLY
    menu_view v;
    v.exec();
    #endif // LOGIC_ONLY
  }
  else
  {
    #ifndef LOGIC_ONLY
    game_view v;
    v.exec();
    #endif // LOGIC_ONLY
  }
}
