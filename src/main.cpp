/// @mainpage Conquer Chess
///
/// Welcome to Conquer Chess
///
/// - [Conquer Chess homepage](https://richelbilderbeek.github.io/conquer_chess/)
///
/// The main window is called \link{main_window}.
///
/// The class for the game logic is called \link{game}.
///
/// This game has some singletons:
///
/// - \link{get_render_window}: to get the one-and-only SFML render window
/// - \link{game_resources::get}: to get the one-and-only \link{game_resources}
/// - \link{game_options::get}: to get the one-and-only \link{game options}
/// - \link{physical_controllers::get}: to get the one-and-only \link{physical_controllers}
/// - \link{lobby_options::get}: to get the one-and-only \link{lobby_options}

// Use LOGIC_ONLY to be able to run on GHA

#include "about.h"
#include "about_view_layout.h"
#include "action_history.h"
#include "board_layout.h"
#include "board_to_text_options.h"
#include "castling_type.h"
#include "chess_move.h"
#include "cc_cli_options.h"
#include "controls_view_item.h"
#include "controls_view_layout.h"
#include "diagnostics_file.h"
#include "fps_clock.h"
#include "game.h"
#include "game_controller.h"
#include "game_statistics_widget_layout.h"
#include "game_statistic_type.h"
#include "game_log.h"
#include "game_options.h"
#include "game_statistics.h"
#include "game_rect.h"
#include "game_view_layout.h"
#include "game_statistics_in_time.h"
#include "game_statistics_output_file.h"
#include "helper.h"
#include "fen_string.h"
#include "in_game_time.h"
#include "game_statistics_view_layout.h"
#include "in_game_controls_layout.h"
#include "key_bindings.h"
#include "laws.h"
#include "lobby_options.h"
#include "lobby_view_item.h"
#include "lobby_view_layout.h"
#include "menu_view_item.h"
#include "menu_view_layout.h"
#include "navigation_controls_layout.h"
#include "options_view_layout.h"
#include "pgn_move_string.h"
#include "pgn_game_string.h"
#include "physical_controller.h"
#include "physical_controllers.h"
#include "piece_actions.h"
#include "piece_id.h"
#include "square_layout.h"
#include "pieces.h"
#include "replay_view_layout.h"
#include "program_state.h"
#include "race.h"
#include "read_only.h"
#include "replay.h"
#include "screen_coordinate.h"
#include "sfml_helper.h"
#include "sleep_scheduler.h"
#include "test_game.h"
#include "test_rules.h"
#include "when_to_make_a_move_law.h"
#ifndef LOGIC_ONLY
#include "loading_view.h"
#include "main_window.h"
#endif // LOGIC_ONLY


#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp>

#include <cassert>
#include <csignal>
#include <cstdio>
#include <filesystem>

/// All tests are called from here, only in debug mode
void test()
{
#ifndef NDEBUG
  //test_rules();
  //test_game_controller();
  //assert(1 == 2);

  test_about();
  test_about_view_layout();
  test_action_history();
  test_action_number();
  test_board_layout();
  test_board_to_text_options();
  test_castling_type();
  test_chess_color();
  test_chess_move();
  test_control_action_type();
  test_controller();
  test_controls_view_item();
  test_controls_view_layout();
  test_cli_options();
  test_delta_t();
  test_fen_string();
  test_fps_clock();
  test_game();
  test_game_controller();
  test_game_coordinate();
  test_game_info_layout();
  test_game_options();
  test_game_rect();
  test_game_speed();
  test_game_statistic_type();
  test_game_statistics();
  test_game_statistics_in_time();
  test_game_statistics_output_file();
  test_game_statistics_view_layout();
  test_game_view_layout();
  test_helper();
  test_id();
  test_in_game_controls_layout();
  test_in_game_time();
  test_key_bindings();
  test_laws();
  test_lobby_options();
  test_lobby_view_item();
  test_lobby_view_layout();
  test_log();
  test_menu_view_item();
  test_menu_view_layout();
  test_message();
  test_message_type();
  test_mouse_bindings();
  test_navigation_controls_layout();
  test_options_view_item();
  test_options_view_layout();
  test_pgn_game_string();
  test_pgn_move_string();
  test_physical_controller_type();
  test_physical_controllers();
  test_piece();
  test_piece_action();
  test_piece_action_type();
  test_piece_actions();
  test_piece_layout();
  test_piece_type();
  test_pieces();
  test_replay_view_layout();
  test_program_state();
  test_race();
  test_read_only();
  test_replay();
  test_rules();
  test_screen_coordinate();
  test_screen_rect();
  test_sfml_helper();
  test_side();
  test_sleep_scheduler();
  test_sleep_scheduler();
  test_square();
  test_starting_position_type();
  test_user_input();
  test_user_inputs();
  test_volume();
  test_when_to_make_a_move_law();

#ifndef LOGIC_ONLY
  test_loading_view();
  test_resource_loader();
#endif // LOGIC_ONLY

#endif // NDEBUG
}

/// Handle the abort signal, as triggered by a failing assert.
/// Note that, for now, this is mostly cosmetic:
/// in main, the stderr is redirected
/// to a file.
/// @note From \url{https://www.geeksforgeeks.org/cpp/how-to-handle-sigabrt-signal-in-cpp/}
void handle_abort_signal(int /* signal */)
{
    std::cout
      << "ERROR!\n"
      << "\n"
      << "I am sorry. This should never have happened. \n"
      << "\n"
      << "Error is written to '"
        << std::filesystem::current_path() << "/conquer_chess_error.txt"
        << "'. \n"
      << "\n"
      << "It would be helpful to share this file with me. \n"
      << "You will be added as a contributor \n"
      << "if you create a proper bug report :-) \n"
      << "\n"
      << "Sorry, the Conquer Chess developer\n"
      << "\n"
    ;
}


void play_standard_random_game()
{
  try
  {
    const int n_turns{8123};
    const auto g = play_random_game(n_turns, 42);
    std::clog
      << "Final board:\n"
      << to_board_str(g.get_pieces()) << '\n'
      << "As FEN:\n"
      << to_fen_string(g).get() << '\n'
    ;
    std::clog << "Winner:\n";
    if (g.get_winner().has_value())
    {
      std::clog << to_str(g.get_winner().value()) << '\n';
    }
    else
    {
      std::clog << "None" << '\n';
    }
  }
  catch (std::exception& e)
  {
    std::cerr
      << "ERROR! Exception thrown: \n"
      << e.what() << '\n'
    ;
  }
}

void get_runtime_speed_profile()
{
  #ifdef NDEBUG
  // Do the profile here
  play_standard_random_game();
  #else
  std::cerr << "Do not profile in debug mode\n";
  assert(!"Do not profile in debug mode");
  std::exit(42);
  #endif
}

int main(int argc, char **argv) //!OCLINT tests may be long
{

  const auto options{cc_cli_options(collect_args(argc, argv))};

  if (options.get_do_show_help())
  {
    show_help(std::cout);
    return 0;
  }


  diagnostics_file file;

  // Compile date, current time
  file.add_header();

  file.add_cli_options(options);

  if (options.get_do_assert_to_log())
  {
    // Set up the signal handler for SIGABRT
    // From https://www.geeksforgeeks.org/cpp/how-to-handle-sigabrt-signal-in-cpp/
    signal(SIGABRT, handle_abort_signal);

    // Redirect errors to the error log filename:
    // errors will be appended
    std::freopen(get_default_diagnostics_filename().c_str(), "a", stderr);
  }


  if (options.get_do_profile())
  {
    std::clog << "Start profiling\n";
    get_runtime_speed_profile();
  }
  if (options.get_do_play_standard_random_game())
  {
    std::clog << "Start playing a standard random game\n";
    play_standard_random_game();
  }
  if (options.get_do_test())
  {
    std::clog << "Start tests\n";
    test();
    std::clog << "All tests passed. Testing done!\n";
  }

  #ifndef LOGIC_ONLY
  std::clog << "Starting game\n";
  main_window v(options);
  v.exec();
  #endif // LOGIC_ONLY

  // Game ended successfully
  file.add_footer();

}
