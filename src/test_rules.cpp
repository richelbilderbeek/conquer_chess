#include "test_rules.h"

#include "game_controller.h"

#include <cassert>

void test_rule_1_2()
{
  /// [1.2] The player with the light-coloured pieces (White) makes the first move,
  /// then the players move alternately,
  /// with the player with the dark-coloured pieces (Black) making the next move.
  ///
  /// In Conquer Chess, white can make multiple moves during the time of the first
  /// move. After waiting for the duration of one chess move, black can start
  /// doing its many moves.
  {
    game_controller c;

    move_cursor_to(c, "e2", side::lhs);
    move_cursor_to(c, "e7", side::rhs);

    // Can move already
    assert(!get_piece_actions(c, side::lhs).empty());
    // Cannot move yet
    assert(get_piece_actions(c, side::rhs).empty());

    c.tick(delta_t(1.0));

    // Could move already
    assert(!get_piece_actions(c, side::lhs).empty());
    // Can move now
    assert(!get_piece_actions(c, side::rhs).empty());
  }
}

void test_rule_1_4_1()
{
  /// [1.4.1]
  /// The player who achieves this goal is said to have ‘checkmated’
  /// the opponent’s king and to have won the game.
  /// Leaving one’s own king under attack,
  /// exposing one’s own king to attack and also
  /// 'capturing' the opponent’s king is not allowed.
  ///

  // [1.4.1a]
  // The player who achieves this goal is said to have ‘checkmated’
  //   the opponent’s king and to have won the game.
  {
    game_controller c{
      game(get_pieces_before_scholars_mate())
    };
    assert(is_piece_at(c, square("h5"))); // White queen
    assert(count_selected_units(c, chess_color::white) == 0);
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::white));
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::black));
    assert(!has_winner(c));

    // h5xf7#
    do_select(c, "h5", side::lhs); // White queen
    move_cursor_to(c, "f7", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    for (int i{0}; i!=5; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("h5")));
    assert(is_piece_at(c, square("f7")));
    assert(get_piece_at(c, square("f7")).get_type() == piece_type::queen);
    assert(is_checkmate(c.get_game().get_pieces(), chess_color::black));
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::white));
    assert(has_winner(c));
    assert(get_winner(c).value() == chess_color::white);
  }
  // [1.4.1b]
  // Leaving one’s own king under attack is not allowed.
  #ifdef FIX_1_4_1_B
  {
    game_controller c{
      game(create_game_from_fen_string(fen_string("4k3/8/4q3/8/8/8/8/4K3 w - - 0 1")))
    };

    // King at e1 wants to move to an attacked square
    do_select(c, "e1", side::lhs); // White king
    move_cursor_to(c, "e2", side::lhs); // Attacked square
    assert(count_selected_units(c, chess_color::white) == 1);

    assert(get_piece_actions(c, side::lhs).empty()); // Cannot
  }
  #endif // FIX_1_4_1_B
  #ifdef FIX_1_4_1_C
  // [1.4.1c]
  // Exposing one’s own king to attack is not allowed
  {
    game_controller c{
      game(create_game_from_fen_string(fen_string("4k3/8/4q3/8/8/4Q3/8/4K3 w - - 0 1")))
    };

    // Queen at e3 wants to move away from the pin
    do_select(c, "e3", side::lhs); // White king
    move_cursor_to(c, "a3", side::lhs); // Out of pin
    assert(count_selected_units(c, chess_color::white) == 1);

    assert(get_piece_actions(c, side::lhs).empty()); // Cannot
  }
  #endif // FIX_1_4_1_C
  // [1.4.1d]
  //
  // ’capturing’ the opponent’s king is not allowed in regular chess.
  //
  // In Conquer Chess, 'capturing' the opponent's king is possible
  // (i.e. reducing that king's health to zero)
  // and results in a win.
  {
    game_controller c{
      game(create_game_from_fen_string(fen_string("4k3/8/8/8/4Q3/8/8/4K3 w - - 0 1")))
    };
    assert(is_piece_at(c, square("e4"))); // White queen
    assert(count_selected_units(c, chess_color::white) == 0);
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::white));
    assert(!is_checkmate(c.get_game().get_pieces(), chess_color::black));
    assert(!has_winner(c));

    // e4 attacks king at e8
    do_select(c, "e4", side::lhs); // White queen
    move_cursor_to(c, "e8", side::lhs); // Black king
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    for (int i{0}; i!=5; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("e4"))); // Queen captured king
    assert(get_piece_at(c, square("e8")).get_type() == piece_type::queen); // White queen
    assert(has_winner(c));
    assert(get_winner(c).value() == chess_color::white);
  }
}

void test_rule_1_5()
{
  /// [1.5]. If the position is such that neither player can possibly checkmate
  /// the opponent’s king, the game is drawn (see Article 5.2.2).

  // Position without checkmate possible
  {
    game_controller c{
      game(create_game_from_fen_string(fen_string("4k3/8/8/8/8/8/8/4K3 w - - 0 1")))
    };
    assert(!has_winner(c));
    assert(is_draw(c));

  }
  // Position with checkmate possible
  {
    game_controller c;
    assert(!has_winner(c));
    assert(!is_draw(c));
  }
}

void test_rule_2_1()
{
  /// `[2.1]` The chessboard is composed of an 8 x 8 grid of 64 equal squares
  /// alternately light (the ‘white’ squares) and dark (the ‘black’ squares).
  ///
  /// The chessboard is placed between the players in such a way
  /// that the near corner square to the right of the player is white.
  //
  assert(get_all_files().size() == 8);
  assert(get_all_ranks().size() == 8);
  assert(to_color(square("d1")) == chess_color::white);
  assert(to_color(square("e1")) == chess_color::black);
}

void test_rule_2_2()
{
  /// `[2.2]` At the beginning of the game White has 16 light-coloured pieces
  /// (the ‘white’ pieces); Black has 16 dark-coloured pieces (the ‘black’ pieces).

  const auto pieces = get_standard_starting_pieces();
  assert(pieces.size() == 32);
  std::map<chess_color, int> colors;
  colors[chess_color::white] = 0;
  colors[chess_color::black] = 0;
  for (const auto& p: pieces)
  {
   ++colors[p.get_color()];
  }
  assert(colors[chess_color::white] == 16);
  assert(colors[chess_color::black] == 16);
}

void test_rule_2_3()
{
  /// `[2.3]`. The initial position of the pieces on the chessboard is as follows:
  ///
  /// ```text
  /// rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
  /// ```

  const auto pieces = get_standard_starting_pieces();
  // Types
  assert(get_piece_at(pieces, square("a1")).get_type() == piece_type::rook);
  assert(get_piece_at(pieces, square("b1")).get_type() == piece_type::knight);
  assert(get_piece_at(pieces, square("c1")).get_type() == piece_type::bishop);
  assert(get_piece_at(pieces, square("d1")).get_type() == piece_type::queen);
  assert(get_piece_at(pieces, square("e1")).get_type() == piece_type::king);
  assert(get_piece_at(pieces, square("f1")).get_type() == piece_type::bishop);
  assert(get_piece_at(pieces, square("g1")).get_type() == piece_type::knight);
  assert(get_piece_at(pieces, square("h1")).get_type() == piece_type::rook);

  assert(get_piece_at(pieces, square("a2")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("b2")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("c2")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("d2")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("e2")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("f2")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("g2")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("h2")).get_type() == piece_type::pawn);

  assert(get_piece_at(pieces, square("a7")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("b7")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("c7")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("d7")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("e7")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("f7")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("g7")).get_type() == piece_type::pawn);
  assert(get_piece_at(pieces, square("h7")).get_type() == piece_type::pawn);

  assert(get_piece_at(pieces, square("a8")).get_type() == piece_type::rook);
  assert(get_piece_at(pieces, square("b8")).get_type() == piece_type::knight);
  assert(get_piece_at(pieces, square("c8")).get_type() == piece_type::bishop);
  assert(get_piece_at(pieces, square("d8")).get_type() == piece_type::queen);
  assert(get_piece_at(pieces, square("e8")).get_type() == piece_type::king);
  assert(get_piece_at(pieces, square("f8")).get_type() == piece_type::bishop);
  assert(get_piece_at(pieces, square("g8")).get_type() == piece_type::knight);
  assert(get_piece_at(pieces, square("h8")).get_type() == piece_type::rook);

  // Colors
  assert(get_piece_at(pieces, square("a1")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("b1")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("c1")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("d1")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("e1")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("f1")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("g1")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("h1")).get_color() == chess_color::white);

  assert(get_piece_at(pieces, square("a2")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("b2")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("c2")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("d2")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("e2")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("f2")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("g2")).get_color() == chess_color::white);
  assert(get_piece_at(pieces, square("h2")).get_color() == chess_color::white);

  assert(get_piece_at(pieces, square("a7")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("b7")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("c7")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("d7")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("e7")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("f7")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("g7")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("h7")).get_color() == chess_color::black);

  assert(get_piece_at(pieces, square("a8")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("b8")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("c8")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("d8")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("e8")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("f8")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("g8")).get_color() == chess_color::black);
  assert(get_piece_at(pieces, square("h8")).get_color() == chess_color::black);
}

void test_rule_3_1()
{
  /// `[3.1]` It is not permitted to move a piece to a square occupied
  /// by a piece of the same colour.
  {
    game_controller c;
    do_select(c, "d1", side::lhs); // White queen
    move_cursor_to(c, "d2", side::lhs); // White pawn
    assert(get_piece_actions(c, side::lhs).size() == 1);
    assert(get_piece_actions(c, side::lhs)[0] == piece_action_type::select);
  }
  /// `[3.1.1]` If a piece moves to a square occupied by an opponent's piece
  /// the latter is captured and removed from the chessboard
  /// as part of the same move.
  {
    game_controller c{
      game(get_pieces_before_scholars_mate())
    };

    // The attacker
    assert(is_piece_at(c, square("h5")));
    assert(get_piece_at(c.get_game(), square("h5")).get_type() == piece_type::queen);
    assert(get_piece_at(c.get_game(), square("h5")).get_color() == chess_color::white);

    // The piece under attack
    assert(is_piece_at(c, square("f7")));
    assert(get_piece_at(c.get_game(), square("f7")).get_type() == piece_type::pawn);
    assert(get_piece_at(c.get_game(), square("f7")).get_color() == chess_color::black);

    do_select(c, "h5", side::lhs);
    move_cursor_to(c, "f7", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs)); // Attack
    c.apply_user_inputs_to_game();

    c.tick(delta_t(1.00));
    // Must be captured
    assert(get_piece_at(c, square("f7")).get_color() == chess_color::white);
  }
  /// 3.1.1.CC.1
  /// If two pieces attack the same piece,
  /// the piece that started the attack first, will end up
  /// on the square of the captured piece.
  //
  #ifdef FIX_ISSUE_157
  // scenario 1: a4xb5, beats c4xb5
  {
    game_controller c{
      create_game_with_starting_position(starting_position_type::pawn_all_out_assault)
    };


    do_select(c, "a4", side::lhs);
    move_cursor_to(c, "b5", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs)); // Attack
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.5));

    do_select(c, "c4", side::lhs);
    move_cursor_to(c, "b5", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs)); // Attack
    c.apply_user_inputs_to_game();

    c.tick(delta_t(1.0));

    assert(!is_piece_at(c.get_game(), square("a4")));
    assert(is_piece_at(c.get_game(), square("c4")));
  }
  // scenario 2: c4xb5, beats a4xb5
  {
    game_controller c{
      create_game_with_starting_position(starting_position_type::pawn_all_out_assault)
    };

    do_select(c, "c4", side::lhs);
    move_cursor_to(c, "b5", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs)); // Attack
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.5));

    do_select(c, "a4", side::lhs);
    move_cursor_to(c, "b5", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs)); // Attack
    c.apply_user_inputs_to_game();

    c.tick(delta_t(1.0));

    assert(is_piece_at(c.get_game(), square("a4")));
    assert(!is_piece_at(c.get_game(), square("c4")));
  }
  #endif // FIX_ISSUE_157
  /// `[3.1.2]` A piece is said to attack an opponent's piece
  /// if the piece could make a capture on that square according
  /// to Articles 3.2 to 3.8.
  {
    game_controller c{
      game(get_pieces_before_scholars_mate())
    };

    // The attacker
    assert(is_piece_at(c, square("h5")));
    assert(get_piece_at(c.get_game(), square("h5")).get_type() == piece_type::queen);
    assert(get_piece_at(c.get_game(), square("h5")).get_color() == chess_color::white);

    // The piece under attack
    assert(is_piece_at(c, square("f7")));
    assert(get_piece_at(c.get_game(), square("f7")).get_type() == piece_type::pawn);
    assert(get_piece_at(c.get_game(), square("f7")).get_color() == chess_color::black);

    assert(is_square_attacked(c, square("f7"), chess_color::white));
  }
  /// `[3.1.3]` A piece is considered to attack a square even if this piece
  /// is constrained from moving to that square because it would then leave
  /// or place the king of its own colour under attack.
  {
    game_controller c{
      game(create_game_from_fen_string(fen_string("4k3/4q3/p7/8/8/8/4Q3/4K3 w - - 0 1")))
    };

    // The pinned attacker
    assert(is_piece_at(c, square("e2")));
    assert(get_piece_at(c.get_game(), square("e2")).get_type() == piece_type::queen);
    assert(get_piece_at(c.get_game(), square("e2")).get_color() == chess_color::white);

    // The piece under attack
    assert(is_piece_at(c, square("a6")));
    assert(get_piece_at(c.get_game(), square("a6")).get_type() == piece_type::pawn);
    assert(get_piece_at(c.get_game(), square("a6")).get_color() == chess_color::black);

    assert(is_square_attacked(c, square("a6"), chess_color::white));
  }

  //
}

void test_rule_3_2()
{
  /// - `[3.2]` The bishop may move to any square along a diagonal on which it stands.
  assert(can_move_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("d3")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("d4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("d5")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("e3")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("e5")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("f3")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("f4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("f5")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::bishop, square("e4"), square("f6")));

}

void test_rule_3_3()
{
  /// - `[3.3]` The rook may move to any square along the file or the rank on which it stands.
  assert(!can_move_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("d3")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("d4")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("d5")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("e3")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("e5")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("f3")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("f4")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("f5")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::rook, square("e4"), square("f6")));

}

void test_rule_3_4()
{
  /// `[3.4]` The queen may move to any square along the file, the rank or a diagonal on which it stands.
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("d3")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("d4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("d5")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e3")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e5")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("f3")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("f4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("f5")));

  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e1")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e2")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e3")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e4"))); // Can move home
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e5")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e6")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e7")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e8")));

  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("a4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("b4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("c4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("d4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("e4"))); // Can move home
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("f4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("g4")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("h4")));

  assert(!can_move_on_empty_board(chess_color::white, piece_type::queen, square("e4"), square("f6")));

}

void test_rule_3_5()
{
  /// `[3.5]` When making these moves, the bishop, rook or queen may not move over
  /// any intervening pieces.
  // TODO
}

void test_rule_3_6()
{
  /// `[3.6]` The knight may move to one of the squares nearest to that
  /// on which it stands but not on the same rank, file or diagonal.
  assert(!can_move_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("d3")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("d4")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("d5")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("e3")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("e5")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("f3")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("f4")));
  assert(!can_move_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("f5")));
  assert(can_move_on_empty_board(chess_color::white, piece_type::knight, square("e4"), square("f6")));
}


void test_rule_3_7()
{
  /// `[3.7]` The pawn:
  /// - `[3.7.1]` The pawn may move forward to the square immediately in front of it
  /// on the same file, provided that this square is unoccupied, or
  {
    assert(can_move_on_empty_board(chess_color::white, piece_type::pawn, square("e4"), square("e5")));
  }
  /// `[3.7.2]` on its first move the pawn may move as in 3.7.1 or
  ///  alternatively it may advance two squares along the same file,
  ///  provided that both squares are unoccupied, or
  {
    // TOOD
  }
  /// `[3.7.3]` the pawn may move to a square occupied by an opponent's piece
  ///  diagonally in front of it on an adjacent file, capturing that piece.
  {
    // TOOD
  }

  /// `[3.7.3.1]` A pawn occupying a square on the same rank as and
  ///  on an adjacent file to an opponent's pawn which has just advanced
  ///  two squares in one move from its original square may capture
  ///  this opponent's pawn as though the latter had been moved only one square.
  {
    // TOOD
  }

  /// `[3.7.3.2]` This capture is only legal on the move following
  ///  this advance and is called an ‘en passant' capture.
  {
    // TOOD
  }

  /// `[3.7.3.3]` When a player, having the move, plays a pawn to
  ///  the rank furthest from its starting position,
  ///  he/she must exchange that pawn as part of the same move
  ///  for a new queen, rook, bishop or knight of the same colour
  ///  on the intended square of arrival.
  ///  This is called the square of ‘promotion'.
  {
    // TOOD
  }

  /// `[3.7.3.4]` The player's choice is not restricted to pieces
  ///  that have been captured previously.
  {
    // TOOD
  }

  /// `[3.7.3.5]` This exchange of a pawn for another piece is
  ///  called promotion, and the effect of the new piece is immediate.
  {
    // TOOD
  }
}


void test_rules()
{
#ifndef NDEBUG // no tests in release
  test_rule_1_2();
  test_rule_1_4_1();
  test_rule_1_5();
  test_rule_2_1();
  test_rule_2_2();
  test_rule_2_3();
  test_rule_3_1();
  test_rule_3_2();
  test_rule_3_3();
  test_rule_3_4();
  test_rule_3_5();
  test_rule_3_6();
  test_rule_3_7();
#endif // NDEBUG // no tests in release
}

