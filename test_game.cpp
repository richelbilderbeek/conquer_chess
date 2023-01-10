#include "game.h"

#include "asserts.h"
#include "physical_controllers.h"
#include "helper.h"
#include "id.h"
#include "test_game.h"
#include "piece_actions.h"

#include <cassert>
#include <iostream>
#include <sstream>

#ifdef GAME_CONTROLLER_H
#error 'game' must know nothing about 'game_controller'
#endif

/// Test the game class
void test_game_class()
{
#ifndef NDEBUG // no tests in release
  // Constructor
  {

  }
  // game::get_time
  {
    const game g;
    assert(g.get_time() == delta_t(0.0));
  }
  // game::tick, moving
  {
    // #27: a2-a4 takes as long as b2-b3
    {
      game g;
      piece& first_pawn{get_piece_at(g, "a2")};
      piece& second_pawn{get_piece_at(g, "b2")};
      first_pawn.add_action(
        piece_action(
          chess_color::white,
          piece_type::pawn,
          piece_action_type::move,
          "a2",
          "a4"
        )
      );
      second_pawn.add_action(
        piece_action(
          chess_color::white,
          piece_type::pawn,
          piece_action_type::move,
          "b2",
          "b3"
        )
      );
      for (int i{0}; i!=5; ++i)
      {
        g.tick(delta_t(0.25));
        const bool at_start_1{first_pawn.get_current_square() == square("a2")};
        const bool at_start_2{second_pawn.get_current_square() == square("b2")};
        const bool at_goal_1{first_pawn.get_current_square() == square("a4")};
        const bool at_goal_2{second_pawn.get_current_square() == square("b3")};
        assert_eq(at_start_1, at_start_2);
        assert_eq(at_goal_1, at_goal_2);
      }
      assert(is_piece_at(g, square("a4")));
      assert(is_piece_at(g, square("b3")));
    }
  }
  // game::tick, misc behavior
  {
    // A piece under attack has decreasing health
    {
      game_options options{create_default_game_options()};
      options.set_starting_position(starting_position_type::bishop_and_knight_end_game);
      game g(options);
      // Let the white knight at c4
      // attack the black king at d2
      const square from{"c4"};
      const square to{"d2"};
      assert(get_piece_at(g, from).get_color() == chess_color::white);
      assert(get_piece_at(g, from).get_type() == piece_type::knight);
      assert(get_piece_at(g, to).get_color() == chess_color::black);
      assert(get_piece_at(g, to).get_type() == piece_type::king);
      const double health_before{get_piece_at(g, to).get_health()};
      piece& attacker{get_piece_at(g, from)};
      attacker.add_action(
        piece_action(
          chess_color::white,
          piece_type::knight,
          piece_action_type::attack,
          from,
          to
        )
      );
      g.tick(delta_t(0.5));
      const double health_after{get_piece_at(g, to).get_health()};
      assert(health_after < health_before);
    }
  }
  // game::tick, attack
  {
    // Cannot attack a piece of one's own color
    {
      game g;
      const square from{"e1"}; // White king
      const square to{"d1"};   // White queen
      const double health_before{get_piece_at(g, to).get_health()};
      piece& attacker{get_piece_at(g, from)};
      attacker.add_action(
        piece_action(
          chess_color::white,
          piece_type::king,
          piece_action_type::attack,
          from,
          to
        )
      );
      g.tick(delta_t(0.5));
      const double health_after{get_piece_at(g, to).get_health()};
      assert(health_after == health_before);
    }
    // Cannot attack an empty square
    {
      game g;
      const square from{"e1"}; // White king
      const square to{"e2"};   // Empty square
      piece& attacker{get_piece_at(g, from)};
      attacker.add_action(
        piece_action(
          chess_color::white,
          piece_type::king,
          piece_action_type::attack,
          from,
          to
        )
      );
      g.tick(delta_t(0.1));
      const auto messages{get_piece_at(g, from).get_messages()};
      assert(!messages.empty());
      assert(messages.back() == message_type::cannot);
    }
    // When a piece is killed, the queen attacker moves to that square
    {
      game_options options{create_default_game_options()};
      options.set_starting_position(starting_position_type::before_scholars_mate);
      game g(options);

      const square from{"h5"}; // White queen
      const square to{"f7"};   // Black pawn
      piece& attacker{get_piece_at(g, from)};
      attacker.add_action(
        piece_action(
          chess_color::white,
          piece_type::queen,
          piece_action_type::attack,
          from,
          to
        )
      );
      int cnt{0};
      while (is_piece_at(g, to)
        && get_piece_at(g, to).get_color() == chess_color::black
      )
      {
        g.tick(delta_t(0.1));
        ++cnt;
        assert(cnt < 1000);
      }
      // Must be captured
      assert(get_piece_at(g, square(to)).get_color() == chess_color::white);
    }
    // #20: A queen cannot attack over pieces
    {
      game g;
      const square from{"d1"}; // White queen
      const square to{"d8"};   // Black queen
      piece& attacker{get_piece_at(g, from)};
      attacker.add_action(
        piece_action(
          chess_color::white,
          piece_type::queen,
          piece_action_type::attack,
          from,
          to
        )
      );
      g.tick(delta_t(0.1));
      const auto messages{get_piece_at(g, from).get_messages()};
      assert(!messages.empty());
      assert(messages.back() == message_type::cannot);
    }
  }
#endif // NDEBUG // no tests in release
}

/// Test individual functions
void test_game_functions()
{
#ifndef NDEBUG // no tests in release
  // can_castle_kingside
  {
    const game g{
      get_game_with_starting_position(
        starting_position_type::ready_to_castle
      )
    };
    assert(can_castle_kingside(get_piece_at(g, "e1"), g));
    assert(can_castle_kingside(get_piece_at(g, "e8"), g));
  }
  // can_castle_queenside
  {
    const game g{
      get_game_with_starting_position(starting_position_type::ready_to_castle)
    };
    assert(can_castle_queenside(get_piece_at(g, "e1"), g));
    assert(can_castle_queenside(get_piece_at(g, "e8"), g));
  }
  // clear_sound_effects
  /*
  {
    game g;
    game_controller c;
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_action_1(side::rhs));
    assert(collect_messages(g).empty());
    c.get_user_inputs().apply_user_inputs_to_game(c, g); // TODO: fix
    g.tick();
    assert(!collect_messages(g).empty());
    clear_piece_messages(g);
    assert(collect_messages(g).empty());
  }
  */
  #ifdef FIX_ISSUE_78
  // Rewrite to game test, without using the game_controller
  // collect_action_history
  {
    game g;
    game_controller c;
    assert(!has_actions(collect_action_history(g)));
    do_select_and_move_keyboard_player_piece(g, c, "e2", "e4");
    assert(has_actions(collect_action_history(g)));
    g.tick(delta_t(0.25));
    g.tick(delta_t(0.25));
    g.tick(delta_t(0.25));
    do_select_and_move_keyboard_player_piece(g, c, "d2", "d4");
  }
  #endif // FIX_ISSUE_78
  // collect_all_piece_actions
  {
    // default start
    {
      const game g;
      const auto actions{collect_all_piece_actions(g)};
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
      const piece_action e7e5(
        chess_color::black,
        piece_type::pawn,
        piece_action_type::move,
        square("e7"),
        square("e5")
      );
      assert(is_in(e7e5, actions));
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
    // #28: pawns can attack diagonally
    {
      const game g{
        get_game_with_starting_position(starting_position_type::pawn_all_out_assault)
      };
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      const piece_action e4xf5(
        chess_color::white,
        piece_type::pawn,
        piece_action_type::attack,
        square("e4"),
        square("f5")
      );
      assert(is_in(e4xf5, actions));
      const piece_action e5xd4(
        chess_color::black,
        piece_type::pawn,
        piece_action_type::attack,
        square("e5"),
        square("d4")
      );
      assert(is_in(e5xd4, actions));
    }
    // kings only
    {
      const game g{
        get_game_with_starting_position(starting_position_type::kings_only)
      };
      const auto actions{collect_all_piece_actions(g)};
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
      const auto actions{collect_all_piece_actions(g)};
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
    // Kasparov versus Topalov
    {
      const game g{
        get_game_with_starting_position(starting_position_type::kasparov_vs_topalov)
      };
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      const piece_action ka4xa3(
        chess_color::black,
        piece_type::king,
        piece_action_type::attack,
        square("a4"),
        square("a3")
      );
      assert(is_in(ka4xa3, actions));
    }
    // pawn all-out assault
    {
      const game g{
        get_game_with_starting_position(starting_position_type::pawn_all_out_assault)
      };
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      const piece_action ra1a3(
        chess_color::white,
        piece_type::rook,
        piece_action_type::move,
        square("a1"),
        square("a3")
      );
      assert(is_in(ra1a3, actions));
      const piece_action rh8h6(
        chess_color::black,
        piece_type::rook,
        piece_action_type::move,
        square("h8"),
        square("h6")
      );
      assert(is_in(rh8h6, actions));
    }
    // pawns nearly near promotion
    {
      const game g{
        get_game_with_starting_position(starting_position_type::pawns_nearly_near_promotion)
      };
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      const piece_action a6a7(
        chess_color::white,
        piece_type::pawn,
        piece_action_type::move,
        square("a6"),
        square("a7")
      );
      assert(is_in(a6a7, actions));
      const piece_action h3h2(
        chess_color::black,
        piece_type::pawn,
        piece_action_type::move,
        square("h3"),
        square("h2")
      );
      assert(is_in(h3h2, actions));
    }
    // pawn at promotion
    {
      const game g{
        get_game_with_starting_position(starting_position_type::pawns_at_promotion)
      };
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      const piece_action a8isq(
        chess_color::white,
        piece_type::queen,
        piece_action_type::promote_to_queen,
        square("a8"),
        square("a8")
      );
      assert(is_in(a8isq, actions));
      const piece_action h1isr(
        chess_color::black,
        piece_type::rook,
        piece_action_type::promote_to_rook,
        square("h1"),
        square("h1")
      );
      assert(is_in(h1isr, actions));
    }
    // ready to castle
    {
      const game g{
        get_game_with_starting_position(starting_position_type::ready_to_castle)
      };
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      const piece_action wcks(
        chess_color::white,
        piece_type::king,
        piece_action_type::castle_kingside,
        square("e1"),
        square("g1")
      );
      assert(is_in(wcks, actions));
      const piece_action wcqs(
        chess_color::white,
        piece_type::king,
        piece_action_type::castle_queenside,
        square("e1"),
        square("c1")
      );
      assert(is_in(wcqs, actions));
      const piece_action bcks(
        chess_color::black,
        piece_type::king,
        piece_action_type::castle_kingside,
        square("e8"),
        square("g8")
      );
      assert(is_in(bcks, actions));
      const piece_action bcqs(
        chess_color::black,
        piece_type::king,
        piece_action_type::castle_queenside,
        square("e8"),
        square("c8")
      );
      assert(is_in(bcqs, actions));
    }
    // cannot castle through check
    {
      const game g{
        get_game_with_starting_position(starting_position_type::ready_to_not_castle)
      };
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      const piece_action wcks(
        chess_color::white,
        piece_type::king,
        piece_action_type::castle_kingside,
        square("e1"),
        square("g1")
      );
      assert(!is_in(wcks, actions));
      const piece_action wcqs(
        chess_color::white,
        piece_type::king,
        piece_action_type::castle_queenside,
        square("e1"),
        square("c1")
      );
      assert(!is_in(wcqs, actions));
      const piece_action bcks(
        chess_color::black,
        piece_type::king,
        piece_action_type::castle_kingside,
        square("e8"),
        square("g8")
      );
      assert(!is_in(bcks, actions));
      const piece_action bcqs(
        chess_color::black,
        piece_type::king,
        piece_action_type::castle_queenside,
        square("e8"),
        square("c8")
      );
      assert(!is_in(bcqs, actions));
    }
    // cannot move into check
    {
      const game g{
        get_game_with_starting_position(starting_position_type::ready_to_not_castle)
      };
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      const piece_action ke1d1(
        chess_color::white,
        piece_type::king,
        piece_action_type::move,
        square("e1"),
        square("d1")
      );
      assert(!is_in(ke1d1, actions));
      const piece_action ke8d8(
        chess_color::black,
        piece_type::king,
        piece_action_type::move,
        square("e8"),
        square("d8")
      );
      assert(!is_in(ke8d8, actions));
    }
    #ifdef FIX_ISSUE_78
    // Rewrite to game test, without using the game_controller
    // 21: can do en-passant after white g2-g4
    {
      game g{
        get_game_with_starting_position(starting_position_type::before_en_passant)
      };
      game_controller c;
      do_select_and_move_keyboard_player_piece(g, c, "g2", "g4");
      // It takes 1 time unit to move,
      // aim at halfway to window of opportunity for en-passant
      for (int i{0}; i!=6; ++i) g.tick(delta_t(0.25));
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      assert(has_action_of_type(actions, piece_action_type::en_passant));

      const piece_action h4xg3ep(
        chess_color::black,
        piece_type::pawn,
        piece_action_type::en_passant,
        square("h4"),
        square("g3")
      );
      assert(is_in(h4xg3ep, actions));

      const piece_action f4xg3ep(
        chess_color::black,
        piece_type::pawn,
        piece_action_type::en_passant,
        square("f4"),
        square("g3")
      );
      assert(is_in(f4xg3ep, actions));

      // After 1 move disappears
      g.tick(delta_t(1.0));
      const auto actions_again{collect_all_piece_actions(g)};
      assert(!is_in(h4xg3ep, actions_again));
      assert(!is_in(f4xg3ep, actions_again));
    }
    #endif // FIX_ISSUE_78
    // is_empty_between
    {
      const game g;
      assert(is_empty_between(g, "d3", "d5"));
      assert(!is_empty_between(g, "a1", "a8"));
      assert(is_empty_between(g, "d3", "d4"));
    }
    #ifdef FIX_ISSUE_78
    // Rewrite to game test, without using the game_controller
    #define FIX_ISSUE_DOUBLE_MOVE_BLACK
    #ifdef FIX_ISSUE_DOUBLE_MOVE_BLACK
    // do_select_and_move_piece e2-e3
    {
      game g;
      game_controller c;
      assert(is_piece_at(g, "e2"));
      do_select_and_move_piece(g, c, "e2", "e3", side::lhs);
      assert(!is_piece_at(g, "e2"));
    }
    // do_select_and_move_piece e2-e4
    {
      game g;
      game_controller c;
      assert(is_piece_at(g, "e2"));
      do_select_and_move_piece(g, c, "e2", "e4", side::lhs);
      assert(!is_piece_at(g, "e2"));
    }
    // do_select_and_move_piece: e7-e6
    {
      game g;
      game_controller c;
      assert(is_piece_at(g, "e7"));
      do_select_and_move_piece(g, c, "e7", "e6", side::rhs);
      assert(!is_piece_at(g, "e7"));
    }
    // do_select_and_move_piece: e7-e5
    {
      game g;
      game_controller c;
      assert(is_piece_at(g, "e7"));
      do_select_and_move_piece(g, c, "e7", "e5", side::rhs);
      assert(!is_piece_at(g, "e7"));
    }
    #endif // FIX_ISSUE_DOUBLE_MOVE_BLACK
    #endif // FIX_ISSUE_78
    // can_do: standard stup
    {
      const game g;
      // Pawns move forward
      assert(can_do(g, get_piece_at(g, "d2"), piece_action_type::move, "d4", side::lhs));
      assert(can_do(g, get_piece_at(g, "d7"), piece_action_type::move, "d5", side::rhs));
      // Pawns do not move backwards
      assert(!can_do(g, get_piece_at(g, "d2"), piece_action_type::move, "d1", side::lhs));
      assert(!can_do(g, get_piece_at(g, "d7"), piece_action_type::move, "d8", side::rhs));
      // Knights can jump
      assert(can_do(g, get_piece_at(g, "b1"), piece_action_type::move, "c3", side::lhs));
      assert(can_do(g, get_piece_at(g, "b8"), piece_action_type::move, "c6", side::rhs));
      // Bishops cannot move over other pieces
      assert(!can_do(g, get_piece_at(g, "c1"), piece_action_type::move, "a3", side::lhs));
      assert(!can_do(g, get_piece_at(g, "c8"), piece_action_type::move, "a6", side::rhs));
    }
    // can_do: attack
    {
      const game g{
        get_game_with_starting_position(starting_position_type::queen_end_game)
      };
      assert(!can_do(g, get_piece_at(g, "d1"), piece_action_type::move, "d8", side::lhs));
      assert(can_do(g, get_piece_at(g, "d1"), piece_action_type::attack, "d8", side::lhs));
      assert(can_do(g, get_piece_at(g, "d8"), piece_action_type::attack, "d1", side::rhs));
      assert(!can_do(g, get_piece_at(g, "d1"), piece_action_type::attack, "d7", side::lhs));
      assert(!can_do(g, get_piece_at(g, "d8"), piece_action_type::attack, "d2", side::rhs));
    }
    //#define FIX_ISSUE_21
    #ifdef FIX_ISSUE_21
    // can_do: en_passant
    {
      game g{
        get_game_with_starting_position(starting_position_type::before_en_passant)
      };
      do_select_and_move_piece(g, "b7", "b5", side::rhs);
      assert(is_piece_at(g, square("b5")));
      do_select_and_move_piece(g, "b7", "b5", side::rhs);
      assert(get_default_piece_action(g, c, side::lhs));
      assert(get_default_piece_action(g, c, side::lhs).value() != piece_action_type::move);
      assert(get_default_piece_action(g, c, side::lhs).value() != piece_action_type::attack);
      std::clog << g << '\n';
      assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::en_passant);
    }
    #endif // FIX_ISSUE_21
    // can_do: castling
    {
      const game g{
        get_game_with_starting_position(starting_position_type::ready_to_castle)
      };
      assert(can_do(g, get_piece_at(g, "e1"), piece_action_type::castle_kingside, "g1", side::lhs));
      assert(can_do(g, get_piece_at(g, "e8"), piece_action_type::castle_kingside, "g8", side::rhs));
      assert(can_do(g, get_piece_at(g, "e1"), piece_action_type::castle_queenside, "c1", side::lhs));
      assert(can_do(g, get_piece_at(g, "e8"), piece_action_type::castle_queenside, "c8", side::rhs));
    }
    // can_do: promote
    {
      const game g{
        get_game_with_starting_position(starting_position_type::pawns_at_promotion)
      };
      assert(can_do(g, get_piece_at(g, "a8"), piece_action_type::promote_to_queen, "a8", side::lhs));
      assert(can_do(g, get_piece_at(g, "a8"), piece_action_type::promote_to_rook, "a8", side::lhs));
      assert(can_do(g, get_piece_at(g, "a8"), piece_action_type::promote_to_bishop, "a8", side::lhs));
      assert(can_do(g, get_piece_at(g, "a8"), piece_action_type::promote_to_knight, "a8", side::lhs));
      assert(can_do(g, get_piece_at(g, "h1"), piece_action_type::promote_to_queen, "h1", side::rhs));
      assert(can_do(g, get_piece_at(g, "h1"), piece_action_type::promote_to_rook, "h1", side::rhs));
      assert(can_do(g, get_piece_at(g, "h1"), piece_action_type::promote_to_bishop, "h1", side::rhs));
      assert(can_do(g, get_piece_at(g, "h1"), piece_action_type::promote_to_knight, "h1", side::rhs));
    }
    //#define FIX_ISSUE_21
    #ifdef FIX_ISSUE_21
    // 53: Piece selected, opponent at target square -> en_passant
    {
      game g{
        get_game_with_starting_position(starting_position_type::before_en_passant)
      };
      // Black opens up en-passant
      do_select_and_move_piece(g, "b7", "b5", side::rhs);
      do_select(g, "a5", side::lhs);
      move_cursor_to(g, "b6", side::lhs);
      assert(get_default_piece_action(g, c, side::lhs));
      assert(get_default_piece_action(g, c, side::lhs).value() != piece_action_type::move);
      assert(get_default_piece_action(g, c, side::lhs).value() != piece_action_type::attack);
      assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::en_passant);
    }
    #endif // FIX_ISSUE_21

    //#define FIX_ISSUE_21
    #ifdef FIX_ISSUE_21
    // 21: can do en-passant after black b7-b5
    {
      game g{
        game_options(
          get_default_screen_size(),
          create_mouse_keyboard_controllers(),
          starting_position_type::before_en_passant,
          get_default_game_speed(),
          get_default_margin_width()
        )
      };
      std::clog << "==================================\n";
      std::clog << "= START                          =\n";
      std::clog << "==================================\n";
      std::clog << g << '\n';
      assert(is_piece_at(g, square("b7")));
      assert(!is_piece_at(g, square("b5")));
      do_select_and_move_keyboard_player_piece(g, "b7", "b5");
      // It takes 1 time unit to move,
      // aim at halfway to window of opportunity for en-passant
      for (int i{0}; i!=6; ++i) g.tick(delta_t(0.25));
      assert(!is_piece_at(g, square("b7")));
      std::clog << "==================================\n";
      std::clog << "= AFTER                          =\n";
      std::clog << "==================================\n";
      std::clog << g << '\n';
      assert(!is_piece_at(g, square("b7")));
      assert(is_piece_at(g, square("b5")));
      const auto actions{collect_all_piece_actions(g)};
      assert(!actions.empty());
      assert(has_action_of_type(actions, piece_action_type::en_passant));

      const piece_action a5xb6ep(
        chess_color::white,
        piece_type::pawn,
        piece_action_type::en_passant,
        square("a5"),
        square("b6")
      );
      assert(is_in(a5xb6ep, actions));

      const piece_action c5xb6ep(
        chess_color::white,
        piece_type::pawn,
        piece_action_type::en_passant,
        square("c5"),
        square("b6")
      );
      assert(is_in(c5xb6ep, actions));

      // After 1 move disappears
      g.tick(delta_t(1.0));
      const auto actions_again{collect_all_piece_actions(g)};
      assert(!is_in(a5xb6ep, actions_again));
      assert(!is_in(c5xb6ep, actions_again));
    }
    assert(!"Progress #21");
    #endif // FIX_ISSUE_21
  }
  // collect_all_user_inputses
  {
    //#define FIX_ISSUE_34
    #ifdef FIX_ISSUE_34
    // default start
    {
      const game g;
      const auto user_inputs{collect_all_user_inputses(g)};
      assert(!user_inputs.empty());
      const auto piece_actions{collect_all_piece_actions(g)};
      assert(user_inputs.size() == piece_actions.size());
    }
    // default start
    {
      const game g;
      const auto user_inputs{collect_all_user_inputses(g)};
      const auto e2e4{create_user_inputs("e2e4", chess_color::white, g)};
      assert(is_present_in(e2e4, user_inputs));
    }
    #endif // FIX_ISSUE_34
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
    assert(get_options(g) == g.get_game_options());
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
  // get_player_side
  {
    const game_options go{create_default_game_options()};
    lobby_options lo = create_default_lobby_options();
    // default
    {
      const game g(go, lo);
      assert(get_player_side(g, chess_color::white) == side::lhs);
      assert(get_player_side(g, chess_color::black) == side::rhs);
    }
    lo.set_color(chess_color::black, side::lhs);
    {
      const game g(go, lo);
      assert(get_player_side(g, chess_color::white) == side::rhs);
      assert(get_player_side(g, chess_color::black) == side::lhs);
    }
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
  // operator<<
  {
    game g;
    std::stringstream s;
    s << g;
    assert(!s.str().empty());
  }
#endif // NDEBUG // no tests in release
}

void test_game() //!OCLINT tests may be many
{

#ifndef NDEBUG // no tests in release
  test_game_class();
  test_game_functions();
  test_game_scenarios();
#endif // no tests in release
}
