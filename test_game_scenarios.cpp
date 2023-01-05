#include "game.h"

#include <algorithm>
#include <cassert>
#include <iostream>

/// Test scenarios expressed in chess notation
void test_game_scenarios()
{
#ifndef NDEBUG // no tests in release
  #ifdef FIX_ISSUE_78
  // Rewrite to game test, without using the game_controller
  // Move e2-e6
  {
    game g;
    game_controller c;
    assert(is_idle(g));
    const auto id{get_id(g, square("e2"))};
    assert(piece_with_id_is_at(g, id, square("e2")));
    assert(id.get() >= 0);
    do_select_and_move_keyboard_player_piece(g, c, square("e2"), square("e6"));
    assert(!is_idle(g));
    tick_until_idle(g);
    assert(is_idle(g));
    const auto piece{get_piece_with_id(g, id)};
    assert(piece_with_id_is_at(g, id, square("e6")));
  }
  // Ke1-e2 does not move king forward, it shifts selectedness
  {
    game g;
    game_controller c;
    assert(find_pieces(g, piece_type::king, chess_color::white).at(0).get_current_square() == square("e1"));
    do_select_and_move_keyboard_player_piece(g, c, square("e1"), square("e2"));
    tick_until_idle(g);
    const auto messages{find_pieces(g, piece_type::king, chess_color::white).at(0).get_messages()};
    assert(std::count(std::begin(messages), std::end(messages), message_type::select) == 1);
    assert(find_pieces(g, piece_type::king, chess_color::white).at(0).get_current_square() == square("e1"));
  }
  // e2-e6, then cannot move forward
  {
    game g;
    game_controller c;
    assert(is_idle(g));
    const auto id{get_id(g, square("e2"))};
    assert(piece_with_id_is_at(g, id, square("e2")));
    assert(id.get() >= 0);
    do_select_and_move_keyboard_player_piece(g, c, square("e2"), square("e6"));
    assert(!is_idle(g));
    tick_until_idle(g);
    assert(is_idle(g));
    assert(piece_with_id_is_at(g, id, square("e6")));
    do_select_and_move_keyboard_player_piece(g, c, square("e6"), square("e7"));
    tick_until_idle(g);
    assert(!piece_with_id_is_at(g, id, square("e7")));
    assert(piece_with_id_is_at(g, id, square("e6")));
  }
  // d2-d3, then queen does not attack attack d3
  {
    game g;
    game_controller c;
    assert(is_idle(g));
    const auto id{get_id(g, square("d2"))};
    assert(piece_with_id_is_at(g, id, square("d2")));
    assert(id.get() >= 0);
    do_select_and_move_keyboard_player_piece(g, c, square("d2"), square("d3"));
    assert(!is_idle(g));
    tick_until_idle(g);
    assert(is_idle(g));
    const auto piece{get_piece_with_id(g, id)};
    assert(piece.get_current_square() == square("d3"));
    assert(piece_with_id_is_at(g, id, square("d3")));
    assert(piece_with_id_is_at(g, id, square("d3")));
    assert(get_f_health(get_piece_with_id(g, id)) == 1.0);
    do_select_and_start_attack_keyboard_player_piece(
      g, c, square("d1"), square("d3"));
    tick_until_idle(g);
    assert(get_f_health(get_piece_with_id(g, id)) == 1.0);
  }
  // in a queen end game, then queen can move from d1 to d4
  {
    game g = get_game_with_starting_position(starting_position_type::queen_end_game);
    game_controller c;
    assert(is_idle(g));
    const auto id{get_id(g, square("d1"))};
    assert(piece_with_id_is_at(g, id, square("d1")));
    assert(get_piece_with_id(g, id).get_type() == piece_type::queen);
    assert(id.get() >= 0);
    do_select_and_move_keyboard_player_piece(g, c, square("d1"), square("d4"));
    assert(!is_idle(g));
    tick_until_idle(g);
    assert(is_idle(g));
    assert(piece_with_id_is_at(g, id, square("d4")));
  }
  // before scholars mate, then queen can move from
  {
    game g = get_game_with_starting_position(starting_position_type::queen_end_game);
    game_controller c;
    assert(is_idle(g));
    const auto id{get_id(g, square("d1"))};
    assert(piece_with_id_is_at(g, id, square("d1")));
    assert(get_piece_with_id(g, id).get_type() == piece_type::queen);
    assert(id.get() >= 0);
    do_select_and_move_keyboard_player_piece(g, c, square("d1"), square("b1"));
    assert(!is_idle(g));
    tick_until_idle(g);
    assert(is_idle(g));
    assert(piece_with_id_is_at(g, id, square("b1")));
  }
  // a pawn can promote to queen
  {
    game g = get_game_with_starting_position(starting_position_type::pawns_near_promotion);
    game_controller c;
    const auto id{get_id(g, square("a7"))};
    do_select_and_move_keyboard_player_piece(g, c, square("a7"), square("a8"));
    tick_until_idle(g);
    do_select_and_promote_keyboard_player_piece(g, c, square("a8"), piece_type::queen);
    g.tick(delta_t(0.01));
    assert(piece_with_id_is_at(g, id, square("a8")));
    assert(get_piece_with_id(g, id).get_type() == piece_type::queen);
  }
  // a pawn can promote to rook
  {
    game g = get_game_with_starting_position(starting_position_type::pawns_near_promotion);
    game_controller c;
    const auto id{get_id(g, square("a7"))};
    do_select_and_move_keyboard_player_piece(g, c, square("a7"), square("a8"));
    tick_until_idle(g);
    do_select_and_promote_keyboard_player_piece(g, c, square("a8"), piece_type::rook);
    g.tick(delta_t(0.01));
    assert(piece_with_id_is_at(g, id, square("a8")));
    assert(get_piece_with_id(g, id).get_type() == piece_type::rook);
  }
  // a pawn can promote to bishop
  {
    game g = get_game_with_starting_position(starting_position_type::pawns_near_promotion);
    game_controller c;
    const auto id{get_id(g, square("a7"))};
    do_select_and_move_keyboard_player_piece(g, c, square("a7"), square("a8"));
    tick_until_idle(g);
    do_select_and_promote_keyboard_player_piece(g, c, square("a8"), piece_type::bishop);
    g.tick(delta_t(0.01));
    assert(piece_with_id_is_at(g, id, square("a8")));
    assert(get_piece_with_id(g, id).get_type() == piece_type::bishop);
  }
  // a pawn can promote to knight
  {
    game g = get_game_with_starting_position(starting_position_type::pawns_near_promotion);
    game_controller c;
    const auto id{get_id(g, square("a7"))};
    do_select_and_move_keyboard_player_piece(g, c, square("a7"), square("a8"));
    tick_until_idle(g);
    do_select_and_promote_keyboard_player_piece(g, c, square("a8"), piece_type::knight);
    g.tick(delta_t(0.01));
    assert(piece_with_id_is_at(g, id, square("a8")));
    assert(get_piece_with_id(g, id).get_type() == piece_type::knight);
  }
  #endif // FIX_ISSUE_78

#endif // NDEBUG // no tests in release
}
