/// Use LOGIC_ONLY to be able to run on GHA

#include "controller.h"
#include "game.h"
#include "game_rect.h"
#include "game_resources.h"
#include "game_view.h"
#include "game_view_layout.h"
#include "helper.h"
#include "id.h"
#include "fps_clock.h"
#include "game_log.h"
#include "menu_view.h"
#include "menu_view_item.h"
#include "menu_view_layout.h"
#include "chess_move.h"
#include "options_view_layout.h"
#include "replay.h"
#include "screen_coordinat.h"
#include "test_game.h"
#include <SFML/Graphics.hpp>

#include <cassert>
#include <chrono>
#include <iostream>

/// All tests are called from here, only in debug mode
void test()
{
#ifndef NDEBUG
  test_helper();

  test_chess_color();
  test_chess_move();
  test_control_action();
  test_control_actions();
  test_controller();
  test_controller_type();
  test_delta_t();
  test_fps_clock();
  test_game();
  test_game_coordinat();
  test_game_options();
  test_game_rect();
  test_game_speed();
  test_game_view_layout();
  test_helper();
  test_id();
  test_log();
  test_menu_view_item();
  test_menu_view_layout();
  test_message();
  test_message_type();
  test_options_view_item();
  test_options_view_layout();
  test_piece();
  test_piece_action();
  test_piece_action_type();
  test_piece_type();
  test_pieces();
  test_replay();
  test_replayer();
  test_screen_coordinat();
  test_screen_rect();
  test_side();
  test_square();
  test_starting_position_type();
  test_volume();
#ifndef LOGIC_ONLY
  test_game_resources();
  test_game_view();
  test_sound_effects();
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
