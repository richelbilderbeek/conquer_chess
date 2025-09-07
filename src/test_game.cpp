#include "test_game.h"

#include "game.h"
#include "piece.h"
#include "piece_actions.h"
#include "lobby_options.h"
#include "game_options.h"

#include <cassert>
#include <sstream>

#ifdef GAME_CONTROLLER_H
#error 'game' must know nothing about 'game_controller'
#endif

void test_game() //!OCLINT tests may be many
{
#ifndef NDEBUG // no tests in release
  test_game_class();
  test_game_functions();
  test_game_scenarios();
#endif // no tests in release
}

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
  // game::get_time
  {
    game g{create_game_with_standard_starting_position()};
    assert(g.get_in_game_time() == in_game_time(0.0));
  }
  // game::tick
  {
    // Pieces have the same clock
    {
      game g{create_game_with_standard_starting_position()};
      assert(g.get_in_game_time() == in_game_time(0.0));
      for (const piece& p: g.get_pieces())
      {
        assert(p.get_in_game_time() == g.get_in_game_time());
      }
      g.tick(delta_t(0.1));
      for (const piece& p: g.get_pieces())
      {
        assert(p.get_in_game_time() == g.get_in_game_time());
      }
    }
    // a2-a4 makes a pawn en-passantable
    {
      game g{create_game_with_standard_starting_position()};
      piece& p{get_piece_at(g, "a2")};
      p.add_action(
        piece_action(
          chess_color::white,
          piece_type::pawn,
          piece_action_type::move,
          "a2",
          "a4"
        )
      );
      assert(!p.is_enpassantable(g.get_in_game_time()));
      tick(g, delta_t(0.5));
      tick(g, delta_t(0.5));
      assert(p.is_enpassantable(g.get_in_game_time()));
    }
    // a2-a4 makes a pawn en-passantable for one second
    {
      game g{create_game_with_standard_starting_position()};
      piece& p{get_piece_at(g, "a2")};
      p.add_action(
        piece_action(
          chess_color::white,
          piece_type::pawn,
          piece_action_type::move,
          "a2",
          "a4"
        )
      );
      assert(!p.is_enpassantable(g.get_in_game_time()));
      tick(g, delta_t(0.5));
      assert(!p.is_enpassantable(g.get_in_game_time()));
      tick(g, delta_t(0.5));
      assert(p.is_enpassantable(g.get_in_game_time()));
      tick(g, delta_t(0.49));
      assert(p.is_enpassantable(g.get_in_game_time()));
      tick(g, delta_t(0.49));
      assert(p.is_enpassantable(g.get_in_game_time()));
      g.tick(delta_t(0.1));
      assert(!p.is_enpassantable(g.get_in_game_time()));
    }
    // #27: a2-a4 takes as long as b2-b3
    {
      game g{create_game_with_standard_starting_position()};
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
        assert(at_start_1 == at_start_2);
        assert(at_goal_1 == at_goal_2);
      }
      assert(is_piece_at(g, square("a4")));
      assert(is_piece_at(g, square("b3")));
    }
    // A piece under attack has decreasing health
    {
      game g(get_pieces_bishop_and_knight_end_game());
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
      tick(g, delta_t(0.5));
      const double health_after{get_piece_at(g, to).get_health()};
      assert(health_after < health_before);
    }
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
    // When a piece is killed, the queen attacker moves to that square
    {
      game g(get_pieces_before_scholars_mate());
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
      game g{create_game_with_standard_starting_position()};
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
    const game g(get_pieces_ready_to_castle());
    assert(can_castle_kingside(get_piece_at(g, "e1"), g));
    assert(can_castle_kingside(get_piece_at(g, "e8"), g));
  }
  // can_castle_queenside
  {
    const game g(get_pieces_ready_to_castle());
    assert(can_castle_queenside(get_piece_at(g, "e1"), g));
    assert(can_castle_queenside(get_piece_at(g, "e8"), g));
  }
  // can_do_attack: Qd1 cannot attack c8, even not on an empty board
  {
    game g{create_game_with_standard_starting_position()};
    auto& white_queen{get_piece_at(g, "d1")};
    //white_queen.set_selected(true);
    // Wrong file
    assert(!can_do_attack(g, white_queen, square("c8"), lobby_options().get_color(side::lhs)));
  }
  // can_do_attack: Qd1 cannot attack d2, as this is a friendly piece
  {
    game g{create_game_with_standard_starting_position()};
    auto& white_queen{get_piece_at(g, "d1")};
    //white_queen.set_selected(true);
    // Cannot attack own pieces
    assert(!can_do_attack(g, white_queen, square("d2"), lobby_options().get_color(side::lhs)));
  }
  // can_do_promote: a pawn can promote at last rank
  {
    game g(get_pieces_pawns_at_promotion());
    auto& white_pawn{get_piece_at(g, "a8")};
    //white_pawn.set_selected(true);
    assert(can_do_promote(white_pawn, chess_color::white));
  }
  // can_do_castle_kingside
  {
    game g(get_pieces_ready_to_castle());
    auto& white_king{get_piece_at(g, "e1")};
    //white_king.set_selected(true);
    assert(can_do_castle_kingside(g, white_king, chess_color::white));
  }
  // can_do_castle_queenside
  {
    game g(get_pieces_ready_to_castle());
    auto& white_king{get_piece_at(g, "e1")};
    //white_king.set_selected(true);
    assert(can_do_castle_queenside(g, white_king, chess_color::white));
  }

  // collect_all_piece_actions
  {
    // default start
    {
      const game g{create_game_with_standard_starting_position()};
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
    // pawns can attack diagonally
    {
      const game g(get_pieces_pawn_all_out_assault());
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
      const game g(get_pieces_kings_only());
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
      const game g(get_pieces_queen_endgame());
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
      const game g(get_pieces_kasparov_vs_topalov());
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
      const game g(get_pieces_pawn_all_out_assault());
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
      const game g(get_pieces_pawns_nearly_near_promotion());
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
      const game g(get_pieces_pawns_at_promotion());
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
      const game g(get_pieces_ready_to_castle());
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
      const game g(get_pieces_ready_to_not_castle());
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
      const game g(get_pieces_ready_to_not_castle());
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
    // is_empty_between
    {
      const game g;
      assert(is_empty_between(g, "d3", "d5"));
      assert(!is_empty_between(g, "a1", "a8"));
      assert(is_empty_between(g, "d3", "d4"));
    }
    // clear_piece_messages
    {
      game g{create_game_with_standard_starting_position()};
      assert(collect_messages(g).empty());
      clear_piece_messages(g);
      assert(collect_messages(g).empty());
    }
    // collect_action_history
    {
      const game g{create_game_with_standard_starting_position()};
      assert(collect_action_history(g).get().empty());
    }
    // collect_all_piece_actions, for a color
    {
      const game g{create_game_with_standard_starting_position()};
      assert(!collect_all_piece_actions(g, chess_color::white).empty());
      assert(!collect_all_piece_actions(g, chess_color::black).empty());
    }
  }
  // count_piece_actions: actions in pieces accumulate
  {
    game g(get_pieces_kings_only());
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
  // get_occupied_squares
  {
    const game g{create_game_with_standard_starting_position()};
    assert(get_unique_occupied_squares(g).size() == 32);
  }
  // get_piece_at, const
  {
    const game g{create_game_with_standard_starting_position()};
    assert(get_piece_at(g, square("e1")).get_type() == piece_type::king);
  }
  // get_piece_at, non-const
  {
    game g{create_game_with_standard_starting_position()};
    auto& piece{get_piece_at(g, square("e1"))};
    assert(piece.get_type() == piece_type::king);
    //piece.set_selected(true); // Just needs to compile
  }
  // get_piece_with_id, const
  {
    const game g{create_game_with_standard_starting_position()};
    const piece_id id{create_new_id()};
    bool has_thrown{false};
    try
    {
      get_piece_with_id(g, id);
    }
    catch (std::logic_error& )
    {
      has_thrown = true;
    }
    assert(has_thrown);
  }
  // get_piece_with_id, non const
  {
    game g{create_game_with_standard_starting_position()};
    const piece_id id{create_new_id()};
    bool has_thrown{false};
    try
    {
      get_piece_with_id(g, id);
    }
    catch (std::logic_error& )
    {
      has_thrown = true;
    }
    assert(has_thrown);
  }
  // is_empty
  {
    const game g{create_game_with_standard_starting_position()};
    assert(is_empty(g, square("e4")));
    assert(!is_empty(g, square("d1")));
  }
  // is_idle
  {
    const game g{create_game_with_standard_starting_position()};
    assert(is_idle(g));
  }
  // is_piece_at
  {
    const game g{create_game_with_standard_starting_position()};
    assert(is_piece_at(g, square("e1")));
    assert(!is_piece_at(g, square("e4")));
  }
  // get_pieces
  {
    const game g{create_game_with_standard_starting_position()};
    const auto pieces{get_pieces(g)};
    const auto pieces_again{g.get_pieces()};
    assert(pieces == pieces_again);
  }
  // get_time
  {
    const game g{create_game_with_standard_starting_position()};
    const auto t{get_time(g)};
    assert(t.get() == 0.0);
  }
  // tick_until_idle
  {
    game g{create_game_with_standard_starting_position()};
    tick_until_idle(g);
    assert(is_idle(g));
  }
  // to_pgn
  {
    game g{create_game_with_standard_starting_position()};
    assert(to_pgn(g).empty());
    // Only a game cannot do much,
    // see 'test_game_controller' for a test where this does create
    // a proper history
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

