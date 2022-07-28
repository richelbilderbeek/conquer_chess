#include "game.h"

#include "id.h"
#include "test_game.h"

#include <cassert>
#include <iostream>

/// Test the game class
void test_game_class()
{
#ifndef NDEBUG // no tests in release
  // Constructor
  {

  }
  ////////////////////////////////////////////////////////////////////////////
  // Member functions
  ////////////////////////////////////////////////////////////////////////////
  // game::get_layout, const
  {
    const auto g{get_default_game()};
    assert(get_width(g.get_layout().get_board()) > 0);
  }
  // game::get_options, const
  {
    const auto g{get_default_game()};
    assert(g.get_options().get_left_player_color() == chess_color::white);
  }
  // game::get_options, non-const
  {
    auto g{create_king_versus_king_game()};
    auto& options = g.get_options();
    assert(g.get_options().get_left_player_color() == chess_color::white);
    toggle_left_player_color(options);
    assert(g.get_options().get_left_player_color() == chess_color::black);
  }
  // game::get_player_1_pos
  {
    const auto g{create_king_versus_king_game()};
    assert(g.get_player_1_pos().get_x() >= 0.0);
    assert(g.get_player_1_pos().get_y() >= 0.0);
  }
  // game::get_player_2_pos
  {
    const auto g{create_king_versus_king_game()};
    assert(g.get_player_2_pos().get_x() >= 0.0);
    assert(g.get_player_2_pos().get_y() >= 0.0);
  }
  // game::get_time
  {
    const auto g{create_king_versus_king_game()};
    assert(g.get_time() == delta_t(0.0));
  }
  // game::tick
  {
    // A piece under attack must have decreasing health
    {
      game_options options{get_default_game_options()};
      options.set_starting_position(starting_position_type::bishop_and_knight_end_game);
      game g(options);
      const double health_before{get_piece_at(g, square("d2")).get_health()};
      // Let the white knight at c4 attack the black king at d2
      do_select_and_start_attack_keyboard_player_piece(
        g,
        square("c4"),
        square("d2")
      );
      g.tick(delta_t(0.1));
      const double health_after{get_piece_at(g, square("d2")).get_health()};
      assert(health_after < health_before);
    }
    #define FIX_ISSUE_15
    #ifdef FIX_ISSUE_15
    // When a piece is killed, the attacker moves to that square
    {
      game_options options{get_default_game_options()};
      options.set_starting_position(starting_position_type::before_scholars_mate);
      game g(options);
      do_select_and_start_attack_keyboard_player_piece(
        g,
        square("h5"),
        square("f7")
      );
      while (is_piece_at(g, square("f7")))
      {
        g.tick(delta_t(0.1));
      }
      // Start moving to capture
      g.tick(delta_t(0.1));
      const auto killer_queen{
        find_pieces(g, piece_type::queen, chess_color::white).at(0)
      };
      assert(killer_queen.get_actions().front().get_action_type() == piece_action_type::move);
    }
    #endif
  }
#endif // NDEBUG // no tests in release
}

/// Test individual functions
void test_game_functions()
{
#ifndef NDEBUG // no tests in release
  // can_player_select_piece_at_cursor_pos
  {
    game g;
    assert(get_mouse_user_player_color(g) == chess_color::black);
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    g.add_action(control_action(control_action_type::mouse_move, black_king.get_coordinat()));
    g.tick();
    assert(g.get_mouse_player_pos() == black_king.get_coordinat());
    assert(can_player_select_piece_at_cursor_pos(g, chess_color::black));

    assert(can_player_select_piece_at_cursor_pos(g, chess_color::white));
    g.add_action(create_press_left_action()); // cursor to d8
    g.tick();
    assert(!can_player_select_piece_at_cursor_pos(g, chess_color::white));

    g.add_action(create_press_left_action()); // cursor to d7
    g.add_action(create_press_left_action()); // cursor to d6
    g.tick();
    assert(!can_player_select_piece_at_cursor_pos(g, chess_color::white));
  }
  // clear_sound_effects
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(collect_messages(g).empty());
    // Add a sound by selecting a piece
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(!collect_messages(g).empty());
    clear_piece_messages(g);
    assert(collect_messages(g).empty());
  }
  // count_control_actions
  {
    const auto g{create_king_versus_king_game()};
    // No game actions at start
    assert(count_control_actions(g) == 0);
    // TODO: add action
  }
  // count_piece_actions: actions in pieces accumulate
  {
    game g = create_king_versus_king_game();
    get_pieces(g).at(0).add_action(
      piece_action(
        side::lhs,
        piece_type::king,
        piece_action_type::move,
        square("e1"),
        square("e2")
      )
    );
    // First actions is always 'go home'
    assert(count_piece_actions(g, chess_color::white) == 2);
    g.get_pieces().at(0).add_action(
      piece_action(
        side::lhs,
        piece_type::king,
        piece_action_type::move,
        square("e2"),
        square("e3")
      )
    );
    assert(count_piece_actions(g, chess_color::white) == 4);
  }
  // do_show_selected
  {
    const auto g{create_king_versus_king_game()};
    assert(do_show_selected(g) || !do_show_selected(g));
  }
  // get_cursor_pos
  {
    const game g;
    assert(get_cursor_pos(g, chess_color::white) != get_cursor_pos(g, chess_color::black));
  }
  // get_keyboard_player_pos, const, left == keyboard
  {
    game_options options = get_default_game_options();
    options.set_left_controller_type(controller_type::keyboard);
    const game g(options);
    assert(get_keyboard_player_pos(g) == g.get_player_1_pos());
  }
  // get_keyboard_player_pos, const, left == mouse
  {
    game_options options = get_default_game_options();
    options.set_left_controller_type(controller_type::mouse);
    const game g(options);
    assert(get_keyboard_player_pos(g) == g.get_player_2_pos());
  }
  // get_keyboard_player_pos, non-const, white == lhs == keyboard
  {
    game g;
    assert(get_keyboard_user_player_color(g) == chess_color::white);
    const auto pos_before{get_keyboard_player_pos(g)};
    auto& pos = get_keyboard_player_pos(g);
    pos += game_coordinat(0.1, 0.1);
    const auto pos_after{get_keyboard_player_pos(g)};
    assert(pos_before != pos_after);
  }
  // get_keyboard_player_pos, non-const, black == lhs == mouse
  {
    game_options options = get_default_game_options();
    options.set_left_player_color(chess_color::black);
    options.set_left_controller_type(controller_type::mouse);
    game g(options);
    assert(get_keyboard_user_player_color(g) == chess_color::white);
    const auto pos_before{get_keyboard_player_pos(g)};
    assert(pos_before == g.get_player_2_pos());
    auto& pos = get_keyboard_player_pos(g);
    pos += game_coordinat(0.1, 0.1);
    const auto pos_after{get_keyboard_player_pos(g)};
    assert(pos_before != pos_after);
  }
  // get_layout
  {
    const auto g{get_default_game()};
    assert(get_width(get_layout(g).get_board()) > 0);
  }
  // get_mouse_player_pos, const, left == keyboard
  {
    game_options options = get_default_game_options();
    options.set_left_controller_type(controller_type::keyboard);
    const game g(options);
    assert(get_mouse_player_pos(g) == g.get_player_2_pos());
  }
  // get_mouse_player_pos, const, left == mouse
  {
    game_options options = get_default_game_options();
    options.set_left_controller_type(controller_type::mouse);
    const game g(options);
    assert(get_mouse_player_pos(g) == g.get_player_1_pos());
  }
  // get_mouse_player_pos, non-const, left == keyboard
  {
    game_options options = get_default_game_options();
    options.set_left_controller_type(controller_type::keyboard);
    game g(options);
    assert(get_mouse_player_pos(g) == g.get_player_2_pos());
    get_mouse_player_pos(g) += game_coordinat(0.1, 0.1); // Only needs to compile
  }
  // get_mouse_player_pos, non-const, left == mouse
  {
    game_options options = get_default_game_options();
    options.set_left_controller_type(controller_type::mouse);
    game g(options);
    assert(get_mouse_player_pos(g) == g.get_player_1_pos());
    get_mouse_player_pos(g) += game_coordinat(0.1, 0.1); // Only needs to compile
  }
  // get_occupied_squares
  {
    const game g;
    assert(get_occupied_squares(g).size() == 32);
  }
  // get_piece_at, const
  {
    const game g;
    assert(get_piece_at(g, square("e1")).get_type() == piece_type::king);
  }
  // get_piece_at, non-const
  {
    game g;
    auto& piece{get_piece_at(g, square("e1"))};
    assert(piece.get_type() == piece_type::king);
    piece.set_selected(true); // Just needs to compile
  }
  // get_player_color
  {
    const game g;
    assert(get_player_color(g, side::lhs) == chess_color::white);
    assert(get_player_color(g, side::rhs) == chess_color::black);
  }
  // is_idle
  {
    game g;
    assert(is_idle(g));
  }
  // is_piece_at
  {
    const game g;
    assert(is_piece_at(g, square("e1")));
    assert(!is_piece_at(g, square("e4")));
  }
  // get_pieces
  {
    const game g;
    const auto pieces{get_pieces(g)};
    const auto pieces_again{g.get_pieces()};
    assert(pieces == pieces_again);
  }
  // get_mouse_player_pos
  {
    game g;
    const auto pos_before{get_mouse_player_pos(g)};
    auto& pos = g.get_mouse_player_pos();
    pos += game_coordinat(0.1, 0.1);
    const auto pos_after{get_mouse_player_pos(g)};
    assert(pos_before != pos_after);
  }
  // toggle_left_player_color
  {
    game g;
    const auto color_before{get_left_player_color(g.get_options())};
    toggle_left_player_color(g);
    const auto color_after{get_left_player_color(g.get_options())};
    assert(color_after != color_before);
  }
#endif // NDEBUG // no tests in release
}

/// Test keyboard use
void test_game_keyboard_use()
{
#ifndef NDEBUG // no tests in release
  // Keyboard: select white king
  {
    game g;
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    g.get_keyboard_player_pos() = game_coordinat(white_king.get_coordinat());
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_select_action());
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // Keyboard: can move pawn forward
  {
    game g;
    g.get_keyboard_player_pos() = to_coordinat("e2");
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_select_action());
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    g.get_keyboard_player_pos() = to_coordinat("e4");
    g.add_action(create_press_move_action());
    g.tick(); // Moves it to e3, unselects piece
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e3")).get_type() == piece_type::pawn);
    assert(collect_messages(g).at(1).get_message_type() == message_type::start_move);
  }
  // Keyboard: cannot move pawn backward
  {
    game g;
    g.get_keyboard_player_pos() = to_coordinat("e2");
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_select_action());
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    g.get_keyboard_player_pos() = to_coordinat("e1");
    g.add_action(create_press_move_action());
    g.tick(); // Ignores invalid action, adds sound effect
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e2")).get_type() == piece_type::pawn);
    assert(collect_messages(g).at(1).get_message_type() == message_type::cannot);
  }
#endif // NDEBUG // no tests in release
}

/// Test mouse use
void test_game_mouse_use()
{
#ifndef NDEBUG // no tests in release
  // Clicking a unit once with LMB selects it
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Clicking a unit twice with LMB selects and unselects it
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 0);
  }
  // Clicking a unit with LMB, then another unit with LMB, only the last unit is selected
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    const auto black_queen{find_pieces(g, piece_type::queen, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    g.add_action(create_press_lmb_action(black_queen.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // LMB then RMB makes a unit move
  {
    game g = create_king_versus_king_game();
    g.add_action(create_press_lmb_action(to_coordinat("e8")));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 0);
    g.add_action(create_press_rmb_action(to_coordinat("e6")));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) >= 1);
  }
  // 2x LMB then RMB makes a unit move 1 stretch (not 2)
  {
    game g;
    g.add_action(create_press_lmb_action(to_coordinat("e8")));
    g.add_action(create_press_rmb_action(to_coordinat("e7")));
    g.tick(delta_t(0.001));
    assert(count_piece_actions(g, chess_color::black) == 1);
    g.add_action(create_press_lmb_action(to_coordinat("e8")));
    g.add_action(create_press_rmb_action(to_coordinat("e7")));
    g.tick(delta_t(0.001));
    assert(count_piece_actions(g, chess_color::black) == 1);
  }
#endif // NDEBUG // no tests in release
}

void test_game() //!OCLINT tests may be many
{

#ifndef NDEBUG // no tests in release
  test_game_class();
  test_game_functions();
  test_game_keyboard_use();
  test_game_mouse_use();
  test_game_scenarios();
#endif // no tests in release
}
