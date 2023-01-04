#include "game_controller.h"

#include "asserts.h"
#include "game.h"
#include "physical_controllers.h"

#include <cassert>

game_controller::game_controller(
  const physical_controllers& physical_controllers
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
  m_user_inputs.add(a);
}

void add_user_input(game_controller& c, const user_input& input)
{
  c.add_user_input(input);
}

void add_user_inputs(game_controller& c, const user_inputs& inputs)
{
  for (const auto i: inputs.get_user_inputs())
  {
    add_user_input(c, i);
  }
}

void game_controller::apply_user_inputs_to_game(
  game& g
)
{
  for (const auto& action: m_user_inputs.get_user_inputs())
  {
    if (action.get_user_input_type() == user_input_type::press_action_1)
    {
      process_press_action_1(g, *this, action);
    }
    else if (action.get_user_input_type() == user_input_type::press_action_2)
    {
      process_press_action_2(g, *this, action);
    }
    else if (action.get_user_input_type() == user_input_type::press_action_3)
    {
      process_press_action_3(g, *this, action);
    }
    else if (action.get_user_input_type() == user_input_type::press_action_4)
    {
      process_press_action_4(g, *this, action);
    }
    else if (action.get_user_input_type() == user_input_type::press_down)
    {
      const auto pos{get_cursor_pos(action.get_player())};
      set_cursor_pos(get_below(pos), action.get_player());
    }
    else if (action.get_user_input_type() == user_input_type::press_left)
    {
      const auto pos{get_cursor_pos(action.get_player())};
      set_cursor_pos(get_left(pos), action.get_player());
    }
    else if (action.get_user_input_type() == user_input_type::press_right)
    {
      const auto pos{get_cursor_pos(action.get_player())};
      set_cursor_pos(get_right(pos), action.get_player());
    }
    else if (action.get_user_input_type() == user_input_type::press_up)
    {
      const auto pos{get_cursor_pos(action.get_player())};
      set_cursor_pos(get_above(pos), action.get_player());
    }
    else if (action.get_user_input_type() == user_input_type::mouse_move)
    {
      if (has_mouse_controller(*this))
      {
        assert(action.get_coordinat());
        set_cursor_pos(action.get_coordinat().value(), action.get_player());
        assert_eq(get_cursor_pos(action.get_player()), action.get_coordinat().value());
        assert_eq(square(get_cursor_pos(action.get_player())), square(action.get_coordinat().value()));
      }
    }
    #ifdef FIX_ISSUE_46
    // Below you see that LMB selects and RMB moves
    // Instead, the mouse controller has an active action
    // triggered by LMB, and RMB goes to the next
    #endif // FIX_ISSUE_46
    else if (action.get_user_input_type() == user_input_type::lmb_down)
    {
      if (!has_mouse_controller(*this)) return;
      process_press_action_1_or_lmb_down(g, *this, action);

    }
    else if (action.get_user_input_type() == user_input_type::rmb_down)
    {
      const auto maybe_index{
        get_mouse_user_selector()
      };
      assert(maybe_index);
      set_mouse_user_selector(
        get_next(maybe_index.value())
      );
    }
  }
  m_user_inputs = std::vector<user_input>();
}

int count_user_inputs(const game_controller& c) noexcept
{
  return count_user_inputs(c.get_user_inputs());
}

void do_move_keyboard_player_piece(
  game& g,
  game_controller& c,
  const square& s
)
{
  assert(has_keyboard_controller(c));
  assert(count_selected_units(g, get_keyboard_user_player_color(g, c)) == 1);
  set_keyboard_player_pos(c, s);

  assert(s
    == square(get_cursor_pos(c, get_keyboard_user_player_side(c)))
  );

  add_user_input(
    c,
    create_press_action_1(
      get_keyboard_user_player_side(c)
    )
  );
  assert(count_user_inputs(c) == 1);
  c.apply_user_inputs_to_game(g);
  g.tick(delta_t(0.0));
  assert(count_user_inputs(c) == 0);
}

void do_move_mouse_player_piece(
  game& g,
  game_controller& c,
  const square& s

)
{
  assert(has_mouse_controller(c));
  assert(count_selected_units(g, get_mouse_user_player_color(g, c)) == 1);
  set_mouse_player_pos(c, s);
  assert(square(get_cursor_pos(c, get_mouse_user_player_side(c))) == s);

  add_user_input(
    c,
    create_press_lmb_action(
      get_mouse_user_player_side(c)
    )
  );
  assert(count_user_inputs(c) == 1);
  g.tick(delta_t(0.0));
  assert(count_user_inputs(c) == 0);
}

void do_select_and_move_keyboard_player_piece(
  game& g,
  game_controller& c,
  const square& from,
  const square& to
)
{
  do_select_for_keyboard_player(g, c, from);
  assert(count_selected_units(g) > 0);
  do_move_keyboard_player_piece(g, c, to);
  assert(count_user_inputs(c) == 0);
}

void do_select_and_move_keyboard_player_piece(
  game& g,
  game_controller& c,
  const std::string& from_str,
  const std::string& to_str
)
{
  return do_select_and_move_keyboard_player_piece(
    g,
    c,
    square(from_str),
    square(to_str)
  );
}

void do_select_and_move_mouse_player_piece(
  game& g,
  game_controller& c,
  const square& from,
  const square& to
)
{
  do_select_for_mouse_player(g, c, from);
  assert(count_selected_units(g) > 0);
  do_move_mouse_player_piece(g, c, to);
  assert(count_user_inputs(c) == 0);
}

void do_select_and_move_mouse_player_piece(
  game& g,
  game_controller& c,
  const std::string& from_str,
  const std::string& to_str
)
{
  return do_select_and_move_mouse_player_piece(
    g,
    c,
    square(from_str),
    square(to_str)
  );
}

void do_select_and_promote_keyboard_player_piece(
  game& g,
  game_controller& c,
  const square& pawn_location,
  const piece_type promote_to
)
{
  do_select_for_keyboard_player(g, c, pawn_location);
  do_promote_keyboard_player_piece(g, c, pawn_location, promote_to);
  assert(count_user_inputs(c) == 0);
}

void do_select_and_start_attack_keyboard_player_piece(
  game& g,
  game_controller& c,
  const square& from,
  const square& to
)
{
  do_select_for_keyboard_player(g, c, from);
  do_start_attack_keyboard_player_piece(g, c, to);
  assert(count_user_inputs(c) == 0);
}

void do_select_for_keyboard_player(
  game& g,
  game_controller& c,
  const square& s
)
{
  return do_select(g, c, s, get_keyboard_user_player_color(g, c));
}

void do_select_for_mouse_player(
  game& g,
  game_controller& c,
  const square& s
)
{
  assert(has_mouse_controller(c));
  assert(is_piece_at(g, s));
  assert(!get_piece_at(g, s).is_selected());
  set_mouse_player_pos(c, s);
  assert(square(get_cursor_pos(c, get_mouse_user_player_side(c))) == s);
  add_user_input(
    c,
    create_press_lmb_action(
      get_mouse_user_player_side(c)
    )
  );
  g.tick(delta_t(0.0));
  assert(count_user_inputs(c) == 0);
  assert(get_piece_at(g, s).is_selected());
}

const game_coordinat& game_controller::get_cursor_pos(const side player) const noexcept
{
  if (player == side::lhs) return m_lhs_cursor_pos;
  assert(player == side::rhs);
  return m_rhs_cursor_pos;
}

const game_coordinat& get_cursor_pos(const game_controller&c, const side player_side) noexcept
{
  return c.get_cursor_pos(player_side);
}

square get_cursor_square(
  const game_controller& c,
  const side player_side
)
{
  const game_coordinat cursor_pos{get_cursor_pos(c, player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  return square(cursor_pos);
}

std::optional<piece_action_type> get_default_piece_action(
  const game& g,
  const game_controller& c,
  const side player_side
) noexcept
{

  if (has_selected_pieces(g, player_side))
  {
    // Has selected pieces
    const game_coordinat cursor_pos{get_cursor_pos(c, player_side)};
    if (!is_coordinat_on_board(cursor_pos))
    {
      return {};
    }
    const auto cursor_square{square(cursor_pos)};
    const chess_color player_color{get_player_color(g, player_side)};

    if (
      is_piece_at(g, cursor_square)
      && get_piece_at(g, cursor_square).get_color() == player_color
    )
    {
      const piece& p{get_piece_at(g, cursor_square)};
      if (p.is_selected())
      {
        if (can_promote(p))
        {
          return piece_action_type::promote_to_queen;
        }
        else
        {
          return piece_action_type::unselect;
        }
      }
    }
    else
    {
      // No own piece at cursor, maybe can move/castle/en-passant there?
      assert(get_selected_pieces(g, player_side).size() == 1);
      const auto selected_piece{get_selected_pieces(g, player_side)[0]};
      if (can_do(g, selected_piece, piece_action_type::en_passant, cursor_square, player_side))
      {
        return piece_action_type::en_passant;
      }
      if (can_do(g, selected_piece, piece_action_type::castle_kingside, cursor_square, player_side))
      {
        return piece_action_type::castle_kingside;
      }
      if (can_do(g, selected_piece, piece_action_type::castle_queenside, cursor_square, player_side))
      {
        return piece_action_type::castle_queenside;
      }
      if (can_do(g, selected_piece, piece_action_type::move, cursor_square, player_side))
      {
        return piece_action_type::move;
      }
      if (can_do(g, selected_piece, piece_action_type::attack, cursor_square, player_side))
      {
        return piece_action_type::attack;
      }
    }
  }
  else
  {
    // Has no selected pieces
    const auto cursor_pos{get_cursor_pos(c, player_side)};
    if (is_coordinat_on_board(cursor_pos) && is_piece_at(g, square(cursor_pos)))
    {
      const piece& p{get_piece_at(g, square(cursor_pos))};
      const auto player_color{get_player_color(g, player_side)};
      if (p.get_color() == player_color)
      {
        if (!p.is_selected()) return piece_action_type::select;
      }
    }
  }
  return std::optional<piece_action_type>();
}

side get_keyboard_user_player_side(const game_controller& c)
{
  if (c.get_physical_controller(side::lhs).get_type() == physical_controller_type::keyboard)
  {
    // 1 keyboard max
    assert(c.get_physical_controller(side::rhs).get_type() != physical_controller_type::keyboard);
    return side::lhs;
  }
  assert(c.get_physical_controller(side::rhs).get_type() == physical_controller_type::keyboard);
  return side::rhs;
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

const physical_controller& game_controller::get_physical_controller(const side player_side) const noexcept
{
  return m_physical_controllers.get_controller(player_side);
}

const physical_controller& get_physical_controller(
  const game_controller& c,
  const side player
) noexcept
{
  return c.get_physical_controller(player);
}

physical_controller_type get_physical_controller_type(
  const game_controller& c,
  const side player
) noexcept
{
  return get_physical_controller(c, player).get_type();
}


user_input get_user_input_to_do_action_1(
  const game_controller& c,
  const side player_side
)
{
  if (get_physical_controller_type(c, player_side) == physical_controller_type::keyboard)
  {
    return create_press_action_1(player_side);
  }
  else
  {
    return create_press_lmb_action(player_side);
  }
}

const user_inputs& get_user_inputs(const game_controller& c) noexcept
{
 return c.get_user_inputs();
}

user_inputs get_user_inputs_to_move_cursor_from_to(
  const game_controller& c,
  const square& from,
  const square& to,
  const side player_side
)
{
  if (get_physical_controller_type(c, player_side) == physical_controller_type::mouse)
  {
    // A mouse user 'just' moves its mouse at the correct position,
    // regardless of the current cursors' position
    return get_user_inputs_to_move_cursor_to(c, to, player_side);
  }
  else
  {
    assert(
      get_physical_controller_type(c, player_side)
      == physical_controller_type::keyboard
    );
    const int n_right{(to.get_x() - from.get_x() + 8) % 8};
    const int n_down{(to.get_y() - from.get_y() + 8) % 8};
    std::vector<user_input> inputs;
    for (int i{0}; i!=n_right; ++i)
    {
      inputs.push_back(create_press_right_action(player_side));
    }
    for (int i{0}; i!=n_down; ++i)
    {
      inputs.push_back(create_press_down_action(player_side));
    }
    return inputs;
  }
}

user_inputs get_user_inputs_to_move_cursor_to(
  const game_controller& c,
  const square& to,
  const side player_side
)
{
  if (get_physical_controller_type(c, player_side) == physical_controller_type::mouse)
  {
    // A mouse user 'just' moves its mouse at the correct position,
    // regardless of the current cursors' position
    return user_inputs(
      {
        create_mouse_move_action(
          to_coordinat(to),
          player_side
        )
      }
    );
  }
  else
  {
    assert(get_physical_controller_type(c, player_side) == physical_controller_type::keyboard);
    // A keyboard user must move the cursor from its existing position
    const square from{get_cursor_pos(c, player_side)};
    return get_user_inputs_to_move_cursor_from_to(
      c,
      from,
      to,
      player_side
    );
  }
}

user_input get_user_input_to_select(
  const game_controller& c,
  const side player_side
)
{
  if (get_physical_controller_type(c, player_side) == physical_controller_type::keyboard)
  {
    return create_press_action_1(player_side);
  }
  else
  {
    return create_press_lmb_action(player_side);
  }
}

bool has_keyboard_controller(const game_controller& c)
{
  return
       c.get_physical_controller(side::lhs).get_type() == physical_controller_type::keyboard
    || c.get_physical_controller(side::rhs).get_type() == physical_controller_type::keyboard
  ;
}

bool has_mouse_controller(const game_controller& c)
{
  return
       c.get_physical_controller(side::lhs).get_type() == physical_controller_type::mouse
    || c.get_physical_controller(side::rhs).get_type() == physical_controller_type::mouse
  ;
}


bool is_mouse_user(const game_controller& c, const side player_side) noexcept
{
  return c.get_physical_controller(player_side).get_type()
    == physical_controller_type::mouse
  ;
}


void move_cursor_to(
  game_controller& c,
  const std::string& square_str,
  const side player_side
)
{
  move_cursor_to(c, square(square_str), player_side);
}

void move_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
)
{
  c.set_cursor_pos(to_coordinat(s), player_side);
}

void move_keyboard_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
)
{
  assert(
    get_physical_controller_type(c, player_side)
    == physical_controller_type::keyboard
  );
  c.set_cursor_pos(to_coordinat(s), player_side);
  assert(s
    == square(get_cursor_pos(c, player_side))
  );
}

void move_mouse_cursor_to(
  game_controller& c,
  const square& s,
  const side player_side
)
{
  assert(is_mouse_user(c, player_side));
  set_mouse_player_pos(c, s); // Processes the action
  assert(square(get_cursor_square(c, get_mouse_user_player_side(c))) == s);
}

void game_controller::set_mouse_user_selector(const action_number& number)
{
  assert(m_mouse_user_selector.has_value());
  m_mouse_user_selector = number;
}

void game_controller::set_cursor_pos(
  const game_coordinat& pos,
  const side player_side) noexcept
{
  if (player_side == side::lhs)
  {
    m_lhs_cursor_pos = pos;
  }
  else
  {
    assert(player_side == side::rhs);
    m_rhs_cursor_pos = pos;
  }
}

void set_keyboard_player_pos(
  game_controller& c,
  const square& s
)
{
  assert(has_keyboard_controller(c));
  const auto player_side{get_keyboard_user_player_side(c)};
  move_cursor_to(c, s, player_side);

  assert(s
    == square(get_cursor_pos(c, get_keyboard_user_player_side(c)))
  );
  assert(count_user_inputs(c) == 0);
}

void set_mouse_player_pos(
  game_controller& c,
  const square& s
)
{
  assert(has_mouse_controller(c));
  const side player_side{get_mouse_user_player_side(c)};
  set_cursor_pos(c, s, player_side);
  /*
  add_user_input(
    c,
    create_mouse_move_action(
      to_coordinat(s),
      get_mouse_user_player_side(c)
    )
  );
  assert(count_user_inputs(c) == 0);
  */
  assert_eq(square(get_cursor_square(c, get_mouse_user_player_side(c))), s);
  assert(square(get_cursor_square(c, get_mouse_user_player_side(c))) == s);
}

void test_game_controller() //!OCLINT tests may be many
{

#ifndef NDEBUG // no tests in release
  // game::add_user_input
  {
    game_controller c;
    assert(is_empty(get_user_inputs(c)));
    add_user_input(c, create_press_action_1(side::lhs));
    assert(!is_empty(get_user_inputs(c)));
  }
  // has_mouse_controller
  {
    const game_controller g(
      create_two_keyboard_controllers()
    );
    assert(!has_mouse_controller(g));
  }
  // 55: move_cursor_to
  {
    game_controller c;
    move_cursor_to(c, "d1", side::lhs);
    assert(get_cursor_pos(c, side::lhs) == to_coordinat(square("d1")));
    move_cursor_to(c, "f6", side::rhs);
    assert(get_cursor_pos(c, side::rhs) == to_coordinat(square("f6")));
  }
  // Keyboard: select white king
  {
    game g;
    game_controller c;
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    set_cursor_pos(c, to_coordinat(white_king.get_current_square()), side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // 60: selectedness is transferred, for white
  {
    game g;
    game_controller c;
    move_cursor_to(c, "e1", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::white) == 1);
    move_cursor_to(c, "d1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::white) != 2);
    assert(count_selected_units(g, chess_color::white) != 0);
    assert(count_selected_units(g, chess_color::white) == 1);
  }
  // 60: selectedness is transferred, for black
  {
    game g;
    game_controller c{
      create_two_keyboard_controllers()
    };
    move_cursor_to(c, "e8", side::rhs);
    assert(get_cursor_square(c, side::rhs) == square("e8"));
    assert(count_selected_units(g, chess_color::black) == 0);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::black) == 1);
    assert(get_physical_controller_type(c, side::rhs) == physical_controller_type::keyboard);
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::black) != 2);
    assert(count_selected_units(g, chess_color::black) != 0);
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Selecting a unit twice with action 1 selects and unselects it
  {
    game g;
    game_controller c;
    assert(count_selected_units(g, chess_color::white) == 0);
    move_cursor_to(c, "e1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::white) != 2);
    assert(count_selected_units(g, chess_color::white) != 0);
    assert(count_selected_units(g, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 0);
  }
  // Keyboard: can move pawn forward
  {
    game g;
    game_controller c;
    move_cursor_to(c, "e2", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "e4", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
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
    game_controller c;
    move_cursor_to(c, "e4", side::lhs);
    assert(count_selected_units(g, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01));
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "e3", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.01)); // Ignores invalid action, adds sound effect
    assert(count_selected_units(g, chess_color::white) == 0);
    assert(get_closest_piece_to(g, to_coordinat("e4")).get_type() == piece_type::pawn);
    assert(collect_messages(g).at(1).get_message_type() == message_type::cannot);
  }
  // 3: white castles kingside
  {
    game g = get_game_with_starting_position(starting_position_type::ready_to_castle);
    game_controller c;
    move_cursor_to(c, "e1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));
    assert(count_selected_units(g, chess_color::white) == 1);
    assert(collect_messages(g).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "g1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(g);
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
    game_controller c;
    assert(get_keyboard_user_player_side(c) == side::lhs);
    const square s("a1");
    set_keyboard_player_pos(c, s);
    assert(s == square(get_cursor_pos(c, get_keyboard_user_player_side(c))));
  }
  // 47: set_keyboard_player_pos for RHS player
  {
    game_controller c{
      create_mouse_keyboard_controllers()
    };
    assert(get_keyboard_user_player_side(c) == side::rhs);
    const square s("a1");
    set_keyboard_player_pos(c, s);
    assert(s == square(get_cursor_pos(c, get_keyboard_user_player_side(c))));
  }
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
    game_controller c;
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Clicking a unit twice with LMB selects and unselects it
  {
    game g;
    game_controller c;
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 0);
  }
  // 60: selectedness is transferred
  // Clicking a unit with LMB,
  // then another unit with LMB, only the last unit is selected
  {
    game g;
    game_controller c;
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    g.tick();
    assert(count_selected_units(g, chess_color::black) != 2);
    assert(count_selected_units(g, chess_color::black) != 0);
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Ke8e7 works by LMB, LMB
  {
    game g{get_kings_only_game()};
    game_controller c;
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 0);
    move_cursor_to(c, "e7", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
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
