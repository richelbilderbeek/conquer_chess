#include "game.h"

#include <cassert>

/// Test scenarios expressed in chess notation
void test_game_scenarios()
{
#ifndef NDEBUG // no tests in release
  // Move e2-e6
  {
    game g;
    assert(is_idle(g));
    const auto id{get_id(g, square("e2"))};
    assert(piece_with_id_is_at(g, id, square("e2")));
    assert(id.get() >= 0);
    do_select_and_move_keyboard_player_piece(g, square("e2"), square("e6"));
    assert(!is_idle(g));
    tick_until_idle(g);
    assert(is_idle(g));
    const auto piece{get_piece_with_id(g, id)};
    assert(piece_with_id_is_at(g, id, square("e6")));
  }
  // Ke1-e2 does not move king forward
  {
    game g;
    assert(square(find_pieces(g, piece_type::king, chess_color::white).at(0).get_coordinat()) == square("e1"));
    do_select_and_move_keyboard_player_piece(g, square("e1"), square("e2"));
    tick_until_idle(g);
    assert(find_pieces(g, piece_type::king, chess_color::white).at(0).get_messages().back() == message_type::cannot);
    assert(square(find_pieces(g, piece_type::king, chess_color::white).at(0).get_coordinat()) == square("e1"));
  }
  // e2-e6, then cannot move forward
  {
    game g;
    assert(is_idle(g));
    const auto id{get_id(g, square("e2"))};
    assert(piece_with_id_is_at(g, id, square("e2")));
    assert(id.get() >= 0);
    do_select_and_move_keyboard_player_piece(g, square("e2"), square("e6"));
    assert(!is_idle(g));
    tick_until_idle(g);
    assert(is_idle(g));
    assert(piece_with_id_is_at(g, id, square("e6")));
    do_select_and_move_keyboard_player_piece(g, square("e6"), square("e7"));
    tick_until_idle(g);
    assert(!piece_with_id_is_at(g, id, square("e7")));
    assert(piece_with_id_is_at(g, id, square("e6")));
  }
  #ifdef FIX_ISSUE_17
  // e2-e6, then can attack and capture d7
  {
    game g;
    assert(is_idle(g));
    const auto id{get_id(g, square("e2"))};
    assert(piece_with_id_is_at(g, id, square("e2")));
    assert(id.get() >= 0);
    do_select_and_move_keyboard_player_piece(g, square("e2"), square("e6"));
    assert(!is_idle(g));
    tick_until_idle(g);
    assert(is_idle(g));
    assert(piece_with_id_is_at(g, id, square("e6")));
    assert(get_piece_with_id(g, id).get_kill_count() == 0);
    do_select_and_start_attack_keyboard_player_piece(g, square("e6"), square("d7"));
    tick_until_idle(g);
    assert(get_piece_with_id(g, id).get_kill_count() == 1);
    assert(piece_with_id_is_at(g, id, square("d7"))); // Captured it!
    assert(1 == 2);
  }
  #endif // FIX_ISSUE_17
#endif // NDEBUG // no tests in release
}
