#include "game_controller.h"

#include "game.h"
#include "physical_controllers.h"

#include <cassert>

game_controller::game_controller(
  const std::vector<physical_controller>& physical_controllers
)
  : m_lhs_cursor_pos{0.5, 4.5},
    m_mouse_user_selector{},
    m_physical_controllers{physical_controllers},
    m_rhs_cursor_pos{7.5, 4.5}
{
  if (has_mouse_controller(m_physical_controllers))
  {
    m_mouse_user_selector = action_number(1);
  }

}

void game_controller::add_user_input(const user_input& a)
{
  // These will be processed in 'tick'
  m_user_inputs.add(a);
}

const game_coordinat& game_controller::get_cursor_pos(const side player) const noexcept
{
  if (player == side::lhs) return m_lhs_cursor_pos;
  assert(player == side::rhs);
  return m_rhs_cursor_pos;
}

side get_mouse_user_player_side(const game_controller& c)
{
  assert(has_mouse_controller(c));
  if (c.get_physical_controller(side::lhs).get_type() == physical_controller_type::mouse)
  {
    return side::lhs;
  }
  assert(c.get_physical_controller(side::rhs).get_type() == physical_controller_type::mouse);
  return side::rhs;
}

const physical_controller& game_controller::get_physical_controller(const side player) const noexcept
{
  assert(m_physical_controllers.size() == 2);
  if (player == side::lhs) return m_physical_controllers[0];
  assert(player == side::rhs);
  return m_physical_controllers[1];
}

const physical_controller& get_physical_controller(
  const game_controller& c,
  const side player
) noexcept
{
  return c.get_physical_controller(player);
}

bool has_mouse_controller(const game_controller& c)
{
  return
       c.get_physical_controller(side::lhs).get_type() == physical_controller_type::mouse
    || c.get_physical_controller(side::rhs).get_type() == physical_controller_type::mouse
  ;
}

void game_controller::set_mouse_user_selector(const action_number& number)
{
  assert(m_mouse_user_selector.has_value());
  m_mouse_user_selector = number;
}

void game_controller::set_player_pos(
  const game_coordinat& pos,
  const side player) noexcept
{
  if (player == side::lhs)
  {
    m_lhs_cursor_pos = pos;
  }
  else
  {
    assert(player == side::rhs);
    m_rhs_cursor_pos = pos;
  }
}

void test_game_controller() //!OCLINT tests may be many
{

#ifndef NDEBUG // no tests in release
  test_game_controller_keyboard_use();
  test_game_controller_mouse_use();
#endif // no tests in release
}

/// Test keyboard use
void test_game_controller_keyboard_use()
{
#ifndef NDEBUG // no tests in release
  // has_mouse_controller
  {
    const game_controller g(
      create_two_keyboard_controllers()
    );
    assert(!has_mouse_controller(g));
  }
  // Keyboard: select white king
  {
    game g;
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    set_player_pos(g, to_coordinat(white_king.get_current_square()), side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // 60: selectedness is transferred, for white
  {
    game g;
    move_cursor_to(g, "e1", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::white) == 1);
    move_cursor_to(g, "d1", side::lhs);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::white) != 2);
    assert(count_selected_units(g, chess_color::white) != 0);
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // 60: selectedness is transferred, for black
  {
    game g{
      get_game_with_controllers(create_two_keyboard_controllers())
    };
    move_cursor_to(g, "e8", side::rhs);
    assert(count_selected_units(g, chess_color::black) == 0);
    add_user_input(g, create_press_action_1(side::rhs));
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::black) == 1);
    move_cursor_to(g, "d8", side::rhs);
    add_user_input(g, create_press_action_1(side::rhs));
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::black) != 2);
    assert(count_selected_units(g, chess_color::black) != 0);
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Selecting a unit twice with action 1 selects and unselects it
  {
    game g;
    assert(count_selected_units(g, chess_color::white) == 0);
    move_cursor_to(g, "e1", side::lhs);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick();
    assert(count_selected_units(g, chess_color::white) != 2);
    assert(count_selected_units(g, chess_color::white) != 0);
    assert(count_selected_units(g, chess_color::white) == 1);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 0);
  }
  // Keyboard: can move pawn forward
  {
    game g;
    move_cursor_to(g, "e2", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(g, "e4", side::lhs);
    add_user_input(g, create_press_action_1(side::lhs));
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
    game g = get_game_with_starting_position(starting_position_type::pawn_all_out_assault);
    move_cursor_to(g, "e4", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(g, "e3", side::lhs);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick(delta_t(0.01)); // Ignores invalid action, adds sound effect
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e4")).get_type() == piece_type::pawn);
    assert(collect_messages(g).at(1).get_message_type() == message_type::cannot);
  }
  // 3: white castles kingside
  {
    game g = get_game_with_starting_position(starting_position_type::ready_to_castle);
    move_cursor_to(g, "e1", side::lhs);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick(delta_t(0.0));
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(g, "g1", side::lhs);
    add_user_input(g, create_press_action_1(side::lhs));
    g.tick(delta_t(0.0));
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e4")).get_type() == piece_type::pawn);
    #define FIX_ISSUE_3
    #ifdef FIX_ISSUE_3
    assert(collect_messages(g).at(1).get_message_type() == message_type::start_castling_kingside);
    #endif // FIX_ISSUE_3
  }
  // 47: set_keyboard_player_pos for RHS player
  {
    game g;
    assert(get_keyboard_user_player_color(g) == chess_color::white);
    assert(get_keyboard_user_player_side(g) == side::lhs);
    const square s("a1");
    set_keyboard_player_pos(g, s);
    assert(s == square(get_cursor_pos(g, get_keyboard_user_player_side(g))));
  }
  // 47: set_keyboard_player_pos for RHS player
  {
    game g{
      create_default_game_options_with_controllers(
        create_mouse_keyboard_controllers()
      )
    };
    assert(get_keyboard_user_player_color(g) == chess_color::black);
    assert(get_keyboard_user_player_side(g) == side::rhs);
    const square s("a1");
    set_keyboard_player_pos(g, s);
    assert(s == square(get_cursor_pos(g, get_keyboard_user_player_side(g))));
  }
#endif // NDEBUG // no tests in release
}

/// Test mouse use
void test_game_controller_mouse_use()
{
#ifndef NDEBUG // no tests in release
  // has_mouse_controller
  {
    const game_controller g(
      create_mouse_keyboard_controllers()
    );
    assert(has_mouse_controller(g));
  }
  // get_mouse_user_player_side, mouse at LHS
  {
    const game_controller g(
      create_mouse_keyboard_controllers()
    );
    assert(get_mouse_user_player_side(g) == side::lhs);
  }
  // get_mouse_user_player_side, mouse at RHS
  {
    const game_controller g(
      create_keyboard_mouse_controllers()
    );
    assert(get_mouse_user_player_side(g) == side::rhs);
  }
  // Clicking a unit once with LMB selects it
  {
    game g;
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(g, "e8", side::rhs);
    add_user_input(g, create_press_lmb_action(side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Clicking a unit twice with LMB selects and unselects it
  {
    game g;
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(g, "e8", side::rhs);
    add_user_input(g, create_press_lmb_action(side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    add_user_input(g, create_press_lmb_action(side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 0);
  }
  // 60: selectedness is transferred
  // Clicking a unit with LMB,
  // then another unit with LMB, only the last unit is selected
  {
    game g;
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(g, "d8", side::rhs);
    add_user_input(g, create_press_lmb_action(side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    move_cursor_to(g, "e8", side::rhs);
    add_user_input(g, create_press_lmb_action(side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) != 2);
    assert(count_selected_units(g, chess_color::black) != 0);
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Ke8e7 works by LMB, LMB
  {
    game g = get_kings_only_game();
    move_cursor_to(g, "e8", side::rhs);
    add_user_input(g, create_press_lmb_action(side::rhs));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 0);
    move_cursor_to(g, "e7", side::rhs);
    add_user_input(g, create_press_lmb_action(side::rhs));
    g.tick(delta_t(0.01));
    assert(count_piece_actions(g, chess_color::black) >= 1);
  }
#endif // NDEBUG // no tests in release
}

std::ostream& operator<<(std::ostream& os, const game_controller& g) noexcept
{

  os
    << "LHS cursor position: " << g.get_cursor_pos(side::lhs) << '\n'
    << "RHS cursor position: " << g.get_cursor_pos(side::rhs) << '\n'
    << "LHS player physical controller: " << get_physical_controller(g, side::lhs) << '\n'
    << "RHS player physical controller: " << get_physical_controller(g, side::rhs) << '\n'
  ;
  if (g.get_mouse_user_selector())
  {
    os << "Mouse user selector: " << g.get_mouse_user_selector().value() << '\n';
  }
  else
  {
    os << "Mouse user selector: " << "{}" << '\n';
  }
  os
    << "User inputs: " << g.get_user_inputs() << '\n'
  ;
  return os;
}
