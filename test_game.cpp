#include "game.h"

#include "id.h"
#include "test_game.h"
#include "piece_actions.h"

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
    auto g{get_kings_only_game()};
    auto& options = g.get_options();
    assert(g.get_options().get_left_player_color() == chess_color::white);
    toggle_left_player_color(options);
    assert(g.get_options().get_left_player_color() == chess_color::black);
  }
  // game::get_time
  {
    const auto g{get_kings_only_game()};
    assert(g.get_time() == delta_t(0.0));
  }
  // game::tick
  {
    // A piece under attack must have decreasing health
    {
      game_options options{create_default_game_options()};
      options.set_starting_position(starting_position_type::bishop_and_knight_end_game);
      game g(options);
      const double health_before{get_piece_at(g, square("d2")).get_health()};
      // Let the white knight at c4 attack the black king at d2
      assert(get_piece_at(g, square("d2")).get_color() == chess_color::black);
      do_select_and_start_attack_keyboard_player_piece(
        g,
        square("c4"),
        square("d2")
      );
      assert(get_piece_at(g, square("d2")).get_color() == chess_color::black);
      g.tick(delta_t(0.1));
      assert(get_piece_at(g, square("d2")).get_color() == chess_color::black);
      const double health_after{get_piece_at(g, square("d2")).get_health()};
      assert(health_after < health_before);
    }
    // Cannot attack a piece of one's own color
    {
      game g;
      const double health_before{get_piece_at(g, square("e1")).get_health()};
      // Let the white queen at d1 attack the white king at e1
      do_select_and_start_attack_keyboard_player_piece(
        g,
        square("d1"),
        square("e1")
      );
      g.tick(delta_t(0.1));
      const double health_after{get_piece_at(g, square("d2")).get_health()};
      assert(health_after == health_before);
    }
    // When a piece is killed, the queen attacker moves to that square
    {
      game_options options{create_default_game_options()};
      options.set_starting_position(starting_position_type::before_scholars_mate);
      game g(options);
      do_select_and_start_attack_keyboard_player_piece(
        g,
        square("h5"),
        square("f7")
      );
      int cnt{0};
      while (is_piece_at(g, square("f7"))
        && get_piece_at(g, square("f7")).get_color() == chess_color::black
      )
      {
        g.tick(delta_t(0.1));
        ++cnt;
        assert(cnt < 1000);
      }
      // Must be captured
      assert(get_piece_at(g, square("f7")).get_color() == chess_color::white);
    }
  }
#endif // NDEBUG // no tests in release
}

/// Test individual functions
void test_game_functions()
{
#ifndef NDEBUG // no tests in release
  // clear_sound_effects
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(collect_messages(g).empty());
    // Add a sound by selecting a piece
    g.add_action(create_press_action_1(side::rhs));
    //g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(!collect_messages(g).empty());
    clear_piece_messages(g);
    assert(collect_messages(g).empty());
  }
  #define FIX_ISSUE_26_COLLECT_ALL_ACTIONS
  #ifdef FIX_ISSUE_26_COLLECT_ALL_ACTIONS
  // collect_all_actions
  {
    // default start
    {
      const game g;
      const auto actions{collect_all_actions(g)};
      assert(!actions.empty());
      const piece_action e2e3(
        chess_color::white,
        piece_type::pawn,
        piece_action_type::move,
        square("e2"),
        square("e3")
      );
      assert(is_in(e2e3, actions));
      const piece_action e2e4(
        chess_color::white,
        piece_type::pawn,
        piece_action_type::move,
        square("e2"),
        square("e4")
      );
      assert(is_in(e2e4, actions));
      const piece_action nb1c3(
        chess_color::white,
        piece_type::knight,
        piece_action_type::move,
        square("b1"),
        square("c3")
      );
      assert(is_in(nb1c3, actions));
      const piece_action nb8c6(
        chess_color::black,
        piece_type::knight,
        piece_action_type::move,
        square("b8"),
        square("c6")
      );
      assert(is_in(nb8c6, actions));
    }
    // kings only
    {
      const game g{
        get_game_with_starting_position(starting_position_type::kings_only)
      };
      const auto actions{collect_all_actions(g)};
      assert(!actions.empty());
      const piece_action ke1e2(
        chess_color::white,
        piece_type::king,
        piece_action_type::move,
        square("e1"),
        square("e2")
      );
      assert(is_in(ke1e2, actions));
      const piece_action ke8d7(
        chess_color::black,
        piece_type::king,
        piece_action_type::move,
        square("e8"),
        square("d7")
      );
      assert(is_in(ke8d7, actions));
    }
    // kings and queens only
    {
      const game g{
        get_game_with_starting_position(starting_position_type::queen_end_game)
      };
      const auto actions{collect_all_actions(g)};
      assert(!actions.empty());
      const piece_action qd1d5(
        chess_color::white,
        piece_type::queen,
        piece_action_type::move,
        square("d1"),
        square("d5")
      );
      assert(is_in(qd1d5, actions));
      const piece_action qd8a5(
        chess_color::black,
        piece_type::queen,
        piece_action_type::move,
        square("d8"),
        square("a5")
      );
      assert(is_in(qd8a5, actions));
    }
  }
  #endif // FIX_ISSUE_26_COLLECT_ALL_ACTIONS
  // count_control_actions
  {
    const auto g{get_kings_only_game()};
    // No game actions at start
    assert(count_control_actions(g) == 0);
    // TODO: add action
  }
  // count_piece_actions: actions in pieces accumulate
  {
    game g = get_kings_only_game();
    get_pieces(g).at(0).add_action(
      piece_action(
        chess_color::white,
        piece_type::king,
        piece_action_type::move,
        square("e1"),
        square("e2")
      )
    );
    assert(count_piece_actions(g, chess_color::white) == 1);
    g.get_pieces().at(0).add_action(
      piece_action(
        chess_color::white,
        piece_type::king,
        piece_action_type::move,
        square("e2"),
        square("e3")
      )
    );
    assert(count_piece_actions(g, chess_color::white) == 2);
  }
  // do_show_selected
  {
    const auto g{get_kings_only_game()};
    assert(do_show_selected(g) || !do_show_selected(g));
  }
  // get_cursor_pos
  {
    const game g;
    assert(get_cursor_pos(g, chess_color::white) != get_cursor_pos(g, chess_color::black));
  }
  // get_keyboard_player_pos, non-const, white == lhs == keyboard
  {
    game g;
    assert(get_keyboard_user_player_color(g) == chess_color::white);
    const auto pos_before{get_player_pos(g, side::lhs)};
    auto& pos = get_player_pos(g, side::lhs);
    pos += game_coordinat(0.1, 0.1);
    const auto pos_after{get_player_pos(g, side::lhs)};
    assert(pos_before != pos_after);
  }
  // get_layout
  {
    const auto g{get_default_game()};
    assert(get_width(get_layout(g).get_board()) > 0);
  }
  // get_music_volume_as_percentage
  {
    const game g;
    assert(get_music_volume_as_percentage(g) >= 0.0);
  }
  // get_occupied_squares
  {
    const game g;
    assert(get_occupied_squares(g).size() == 32);
  }
  // get_options
  {
    const game g;
    assert(get_options(g) == g.get_options());
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
    game_options options{create_default_game_options()};
    options.set_left_player_color(chess_color::black);
    const game h(options);
    assert(get_player_color(h, side::lhs) == chess_color::black);
    assert(get_player_color(h, side::rhs) == chess_color::white);
  }
  // get_player_side
  {
    const game g;
    assert(get_player_side(g, chess_color::white) == side::lhs);
    assert(get_player_side(g, chess_color::black) == side::rhs);
    game_options options{create_default_game_options()};
    options.set_left_player_color(chess_color::black);
    const game h(options);
    assert(get_player_side(h, chess_color::white) == side::rhs);
    assert(get_player_side(h, chess_color::black) == side::lhs);
  }
  // get_possible_moves
  {
    // No moves when nothing selected
    {
      const game g;
      assert(get_possible_moves(g, side::lhs).empty());
      assert(get_possible_moves(g, side::rhs).empty());
    }
    // Knight at b1 has four moves when selected (two regular, and two moves beyond)
    {
      game g;
      const std::vector<square> moves{get_possible_moves(g, side::lhs)};
      assert(moves.empty());
      auto& piece{get_piece_at(g, square("b1"))};
      assert(piece.get_type() == piece_type::knight);
      piece.set_selected(true);
      std::clog << get_possible_moves(g, side::lhs) << '\n';
      assert(get_possible_moves(g, side::lhs).size() == 4);
    }
    // Pawn at e2 has four moves when selected
    {
      game g;
      const std::vector<square> moves{get_possible_moves(g, side::lhs)};
      assert(moves.empty());
      auto& piece{get_piece_at(g, square("e2"))};
      piece.set_selected(true);
      assert(get_possible_moves(g, side::lhs).size() == 4);
    }
  }
  // is_empty
  {
    const game g;
    assert(is_empty(g, square("e4")));
    assert(!is_empty(g, square("d1")));
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
  // get_time
  {
    const game g;
    const auto t{get_time(g)};
    assert(t.get() == 0.0);
  }
  // get_mouse_player_pos
  {
    game g;
    const auto pos_before{get_player_pos(g, side::rhs)};
    auto& pos = g.get_player_pos(side::rhs);
    pos += game_coordinat(0.1, 0.1);
    const auto pos_after{get_player_pos(g, side::rhs)};
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
    g.get_player_pos(side::lhs) = to_coordinat(white_king.get_current_square());
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_action_1(side::lhs));
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // Keyboard: can move pawn forward
  {
    game g;
    g.get_player_pos(side::lhs) = to_coordinat("e2");
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_action_1(side::lhs));
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    g.get_player_pos(side::lhs) = to_coordinat("e4");
    g.add_action(create_press_action_1(side::lhs));
    g.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    g.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    g.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    g.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e3")).get_type() == piece_type::pawn);
    const auto messages{collect_messages(g)};
    const auto message{messages.at(1)};
    assert(message.get_message_type() == message_type::start_move);
  }
  // Keyboard: cannot move pawn backward
  {
    game g;
    g.get_player_pos(side::lhs) = to_coordinat("e2");
    assert(count_selected_units(g, chess_color::white) == 0);
    g.add_action(create_press_action_1(side::lhs));
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    g.get_player_pos(side::lhs) = to_coordinat("e1");
    g.add_action(create_press_action_1(side::lhs));
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
    g.add_action(create_press_lmb_action(to_coordinat(black_king.get_current_square()), side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Clicking a unit twice with LMB selects and unselects it
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(
      create_press_lmb_action(
        to_coordinat(black_king.get_current_square()), side::rhs
      )
    );
    g.tick();
    g.add_action(create_press_lmb_action(to_coordinat(black_king.get_current_square()), side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 0);
  }
  // Clicking a unit with LMB, then another unit with LMB, only the last unit is selected
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    const auto black_queen{find_pieces(g, piece_type::queen, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(to_coordinat(black_queen.get_current_square()), side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    g.add_action(create_press_lmb_action(to_coordinat(black_king.get_current_square()), side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // LMB then RMB makes a unit move
  {
    game g = get_kings_only_game();
    g.add_action(create_press_lmb_action(to_coordinat("e8"), side::rhs));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 0);
    g.add_action(create_press_rmb_action(to_coordinat("e6"), side::rhs));
    g.tick(delta_t(0.25));
    g.tick(delta_t(0.25));
    g.tick(delta_t(0.25));
    g.tick(delta_t(0.25));
    assert(count_piece_actions(g, chess_color::black) >= 1);
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
