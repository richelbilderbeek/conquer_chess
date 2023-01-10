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

bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const game_controller& c,
  const chess_color cursor_color
)
{
  const auto& cursor_pos{
    get_cursor_pos(
      c,
      get_player_side(g, cursor_color)
    )
  };
  if (
    !is_piece_at(
      g,
      cursor_pos,
      g.get_game_options().get_click_distance()
    )
  )
  {
    return false;
  }
  const auto& piece{get_closest_piece_to(g, cursor_pos)};
  return !piece.is_selected() && piece.get_color() == cursor_color;
}

std::vector<user_inputs> collect_all_user_inputses(
  const game& g,
  const game_controller& c
)
{
  std::vector<user_inputs> user_inputs;

  const auto piece_actions{collect_all_piece_actions(g)};
  user_inputs.reserve(piece_actions.size());
  for (const auto& piece_action: piece_actions) {
    user_inputs.push_back(to_user_inputs(piece_action, g, c));
  }
  return user_inputs;
}

user_inputs convert_move_to_user_inputs(
  const game& g,
  const game_controller& c,
  const chess_move& m
)
{
  const auto player_side{get_player_side(g, m.get_color())};
  const square from{get_from(g, m)};

  user_inputs inputs;
  // Move the cursor to piece's square
  {
    const auto v{get_user_inputs_to_move_cursor_to(c, from, player_side)};
    add(inputs, v);
  }
  // Select the piece
  {
    const auto i{get_user_input_to_select(c, player_side)};
    inputs.add(i);
  }
  // Move the cursor to target's square
  {
    assert(m.get_to().has_value());
    const auto v{
      get_user_inputs_to_move_cursor_from_to(
        c,
        from,
        m.get_to().value(),
        player_side
      )
    };
    add(inputs, v);
  }
  // Do the action
  {
    const auto i{get_user_input_to_do_action_1(c, player_side)};
    inputs.add(i);
  }
  return inputs;
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

void do_promote_keyboard_player_piece(
  game& g,
  game_controller& c,
  const square& pawn_location,
  const piece_type promote_to
)
{
  assert(has_keyboard_controller(c));
  assert(count_selected_units(g, get_keyboard_user_player_color(g, c)) == 1);
  set_keyboard_player_pos(c, pawn_location);
  assert(square(get_cursor_pos(c, side::lhs)) == pawn_location);
  assert(get_piece_at(g, pawn_location).get_type() == piece_type::pawn);
  switch (promote_to)
  {
    case piece_type::bishop:
      add_user_input(c, create_press_action_3(get_keyboard_user_player_side(c)));
      break;
    case piece_type::knight:
      add_user_input(c, create_press_action_4(get_keyboard_user_player_side(c)));
      break;
    case piece_type::king:
    case piece_type::pawn:
    case piece_type::queen:
      assert(promote_to == piece_type::queen);
      add_user_input(
        c,
        create_press_action_1(
          get_keyboard_user_player_side(c)
        )
      );
      break;
    case piece_type::rook:
      add_user_input(c, create_press_action_2(get_keyboard_user_player_side(c)));
      break;
  }
  c.apply_user_inputs_to_game(g);
  g.tick(delta_t(0.0));
  assert(count_user_inputs(c) == 0);
}

void do_start_attack_keyboard_player_piece(
  game& g,
  game_controller& c,
  const square& s
)
{
  assert(has_keyboard_controller(c));
  assert(count_selected_units(g, get_keyboard_user_player_color(g, c)) == 1);
  set_keyboard_player_pos(c, s);
  assert(square(get_cursor_pos(c, side::lhs)) == s);
  add_user_input(c, create_press_action_2(get_keyboard_user_player_side(c)));
  assert(count_user_inputs(c) != 0);
  c.apply_user_inputs_to_game(g);
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

const game_coordinat& get_cursor_pos(
  const game_controller&c,
  const side player_side
) noexcept
{
  return c.get_cursor_pos(player_side);
}

const game_coordinat& get_cursor_pos(
  const game& g,
  const game_controller& c,
  const chess_color cursor_color
)
{
  return get_cursor_pos(c, get_player_side(g, cursor_color));
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

chess_color get_keyboard_user_player_color(
  const game& g,
  const game_controller& c
)
{
  return get_player_color(g, get_keyboard_user_player_side(c));
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

chess_color get_mouse_user_player_color(
  const game& g,
  const game_controller& c
)
{
  return get_player_color(g, get_mouse_user_player_side(c));
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

void play_game_with_controller(
  const physical_controllers& pcs,
  const replay& r,
  const bool verbose
)
{
  game g;
  game_controller c{pcs};
  const auto& moves{r.get_moves()};
  const int n_moves = moves.size();
  for (int i{0}; i!=n_moves; ++i)
  {
    const auto& m{moves[i]};
    if (verbose) std::clog << i << ": " << m << '\n';
    const auto user_inputs{convert_move_to_user_inputs(g, c, m)};
    if (verbose) std::clog << i << ": " << user_inputs << '\n';
    add_user_inputs(c, user_inputs);
    c.apply_user_inputs_to_game(g);
    for (int j{0}; j!=4; ++j) g.tick(delta_t(0.25));
    if (verbose) std::clog << to_board_str(
      g.get_pieces(),
      board_to_text_options(true, true)
    ) << '\n';
    const auto messages{collect_messages(g)};
    const long n_cannot{
      std::count_if(
        std::begin(messages),
        std::end(messages),
        [](const auto& msg)
        {
          return msg.get_message_type() == message_type::cannot;
        }
      )
    };
    assert(n_cannot == 0);
  }

}

void set_cursor_pos(
  game_controller& c,
  const game_coordinat& pos,
  const side player_side
) noexcept
{
  c.set_cursor_pos(pos, player_side);
}

void set_cursor_pos(
  game_controller& c,
  const square& s,
  const side player_side
) noexcept
{
  set_cursor_pos(c, to_coordinat(s), player_side);
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
  // Clicking nothing with LMB makes nothing happen
  {
    game g;
    game_controller c;
    assert(count_selected_units(g, chess_color::black) == 0);
    move_cursor_to(c, "e4", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 0);
  }
  // Clicking RMB makes selector move
  {
    game g;
    game_controller c;
    assert(c.get_mouse_user_selector().value() == action_number(1));
    add_user_input(c, create_press_rmb_action(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick();
    assert(c.get_mouse_user_selector().value() == action_number(2));
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
  // 53: nothing selected, cursor at empty square -> no action
  {
    game g;
    game_controller c;
    move_cursor_to(c, "d4", side::lhs);
    move_cursor_to(c, "d5", side::rhs);
    assert(!get_default_piece_action(g, c, side::lhs));
    assert(!get_default_piece_action(g, c, side::rhs));
  }
  // 53: nothing selected, cursor at square with opponent piece -> no action
  {
    game g;
    game_controller c;
    move_cursor_to(c, "d8", side::lhs);
    move_cursor_to(c, "d1", side::rhs);
    assert(!get_default_piece_action(g, c, side::lhs));
    assert(!get_default_piece_action(g, c, side::rhs));
  }
  // 53: nothing selected, cursor at square of own color -> select
  {
    game g;
    game_controller c;
    move_cursor_to(c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::rhs);
    assert(get_default_piece_action(g, c, side::lhs));
    assert(get_default_piece_action(g, c, side::rhs));
    assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::select);
    assert(get_default_piece_action(g, c, side::rhs).value() == piece_action_type::select);
  }
  // 53: selected piece, cursor still there -> unselect
  {
    game g;
    game_controller c;
    move_cursor_to(c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_action_1(side::lhs));
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game(g);
    g.tick(delta_t(0.0));
    assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::unselect);
    assert(get_default_piece_action(g, c, side::rhs).value() == piece_action_type::unselect);
  }
  // 53: Piece selected, cursor at valid target square -> move
  {
    game g;
    game_controller c;
    do_select(g, c, "d2", side::lhs);
    do_select(g, c, "d7", side::rhs);
    move_cursor_to(c, "d3", side::lhs);
    move_cursor_to(c, "d5", side::rhs);
    assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::move);
    assert(get_default_piece_action(g, c, side::rhs).value() == piece_action_type::move);
  }
  // 53: Piece selected, opponent at target square -> attack
  {
    game g{
      get_game_with_starting_position(starting_position_type::queen_end_game)
    };
    game_controller c;
    do_select(g, c, "d1", side::lhs);
    do_select(g, c, "d8", side::rhs);
    move_cursor_to(c, "d8", side::lhs);
    move_cursor_to(c, "d1", side::rhs);
    assert(get_default_piece_action(g, c, side::lhs).value() != piece_action_type::move);
    assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::attack);
    assert(get_default_piece_action(g, c, side::rhs).value() == piece_action_type::attack);
  }
  // 53: King selected, cursor at valid king-side castling square -> king-side castle
  {
    game g{
      get_game_with_starting_position(starting_position_type::ready_to_castle)
    };
    game_controller c;
    do_select(g, c, "e1", side::lhs);
    do_select(g, c, "e8", side::rhs);
    move_cursor_to(c, "g1", side::lhs);
    move_cursor_to(c, "g8", side::rhs);
    assert(get_default_piece_action(g, c, side::lhs).value() != piece_action_type::move);
    assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::castle_kingside);
    assert(get_default_piece_action(g, c, side::rhs).value() == piece_action_type::castle_kingside);
  }
  // 53: King selected, cursor at valid queen-side castling square -> queen-side castle
  {
    game g{
      get_game_with_starting_position(starting_position_type::ready_to_castle)
    };
    game_controller c;
    do_select(g, c, "e1", side::lhs);
    do_select(g, c, "e8", side::rhs);
    move_cursor_to(c, "c1", side::lhs);
    move_cursor_to(c, "c8", side::rhs);
    assert(get_default_piece_action(g, c, side::lhs).value() != piece_action_type::move);
    assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::castle_queenside);
    assert(get_default_piece_action(g, c, side::rhs).value() == piece_action_type::castle_queenside);
  }
  // 53: Pawns move to the square where they are promoted -> move
  {
    game g{
      get_game_with_starting_position(starting_position_type::pawns_near_promotion)
    };
    game_controller c;
    do_select(g, c, "a7", side::lhs);
    do_select(g, c, "h2", side::rhs);
    move_cursor_to(c, "a8", side::lhs);
    move_cursor_to(c, "h1", side::rhs);
    assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::move);
    assert(get_default_piece_action(g, c, side::rhs).value() == piece_action_type::move);
  }
  // 53: Pawns are at square where they are promoted -> promote
  {
    game g{
      get_game_with_starting_position(starting_position_type::pawns_at_promotion)
    };
    game_controller c;
    do_select(g, c, "a8", side::lhs);
    do_select(g, c, "h1", side::rhs);
    move_cursor_to(c, "a8", side::lhs);
    move_cursor_to(c, "h1", side::rhs);
    assert(get_default_piece_action(g, c, side::lhs).value() != piece_action_type::unselect);
    assert(get_default_piece_action(g, c, side::lhs).value() == piece_action_type::promote_to_queen);
    assert(get_default_piece_action(g, c, side::rhs).value() == piece_action_type::promote_to_queen);
  }
  #define FIX_ISSUE_64
  #ifdef FIX_ISSUE_64
  // To do e2-e4, from e1, it takes 5 key presses
  {
    game g;
    game_controller c;
    move_cursor_to(c, "e1", side::lhs);
    const chess_move m("e4", chess_color::white);
    const user_inputs inputs{
      convert_move_to_user_inputs(g, c, m)
    };
    assert(!is_empty(inputs));
    assert(count_user_inputs(inputs) == 5);
    assert(inputs.get_user_inputs()[0].get_user_input_type() == user_input_type::press_right);
    assert(inputs.get_user_inputs()[1].get_user_input_type() == user_input_type::press_action_1);
    assert(inputs.get_user_inputs()[2].get_user_input_type() == user_input_type::press_right);
    assert(inputs.get_user_inputs()[3].get_user_input_type() == user_input_type::press_right);
    assert(inputs.get_user_inputs()[4].get_user_input_type() == user_input_type::press_action_1);
    add_user_inputs(c, inputs);
    #ifdef FIX_ISSUE_64_NO_ACTION
    assert(is_piece_at(g, "e2"));
    g.tick();
    assert(!is_piece_at(g, "e2"));
    #endif // FIX_ISSUE_64_NO_ACTION
  }
  #endif // FIX_ISSUE_64
  // get_cursor_pos
  {
    const game g;
    const game_controller c;
    assert(get_cursor_pos(g, c, chess_color::white) != get_cursor_pos(g, c, chess_color::black));
  }
  // get_keyboard_player_pos, non-const, white == lhs == keyboard
  {
    game g;
    game_controller c;
    assert(get_keyboard_user_player_color(g, c) == chess_color::white);
    const auto pos_before{get_cursor_pos(c, side::lhs)};
    const auto pos{get_cursor_pos(c, side::lhs)};
    set_cursor_pos(c, pos + game_coordinat(0.1, 0.1), side::lhs);
    const auto pos_after{get_cursor_pos(c, side::lhs)};
    assert(pos_before != pos_after);
  }

  // get_mouse_player_pos
  {
    game g;
    game_controller c;
    const auto pos_before{get_cursor_pos(c, side::rhs)};
    const auto pos{get_cursor_pos(c, side::rhs)};
    set_cursor_pos(c, pos + game_coordinat(0.1, 0.1), side::rhs);
    const auto pos_after{get_cursor_pos(c, side::rhs)};
    assert(pos_before != pos_after);
  }
  // #27: a2-a4 takes as long as b2-b3
  {
    game g;
    game_controller c;
    assert(is_piece_at(g, square("a2")));
    assert(is_piece_at(g, square("b2")));
    assert(!is_piece_at(g, square("a4")));
    assert(!is_piece_at(g, square("a3")));
    assert(!is_piece_at(g, square("b3")));
    assert(count_selected_units(g, chess_color::white) == 0);
    do_select_for_keyboard_player(g, c, square("a2"));
    assert(count_selected_units(g, chess_color::white) == 1);
    do_move_keyboard_player_piece(g, c, square("a4"));
    assert(count_selected_units(g, chess_color::white) == 0);
    do_select_for_keyboard_player(g, c, square("b2"));
    do_move_keyboard_player_piece(g, c, square("b3"));
    for (int i{0}; i!=5; ++i)
    {
      g.tick(delta_t(0.25));
    }
    assert(!is_piece_at(g, square("a2")));
    assert(!is_piece_at(g, square("b2")));
    assert(is_piece_at(g, square("a4")));
    assert(!is_piece_at(g, square("a3")));
    assert(is_piece_at(g, square("b3")));
  }
  // A piece under attack must have decreasing health
  {
    game_controller c;
    game_options options{create_default_game_options()};
    options.set_starting_position(starting_position_type::bishop_and_knight_end_game);
    game g(options);
    const double health_before{get_piece_at(g, square("d2")).get_health()};
    // Let the white knight at c4 attack the black king at d2
    assert(get_piece_at(g, square("d2")).get_color() == chess_color::black);
    do_select_and_start_attack_keyboard_player_piece(
      g,
      c,
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
    game_controller c;
    const double health_before{get_piece_at(g, square("e1")).get_health()};
    // Let the white queen at d1 attack the white king at e1
    do_select_and_start_attack_keyboard_player_piece(
      g,
      c,
      square("d1"),
      square("e1")
    );
    g.tick(delta_t(0.1));
    const double health_after{get_piece_at(g, square("d2")).get_health()};
    assert(health_after == health_before);
  }
  // When a piece is killed, the queen attacker moves to that square
  {
    game_controller c;
    game_options options{create_default_game_options()};
    options.set_starting_position(starting_position_type::before_scholars_mate);
    game g(options);
    do_select_and_start_attack_keyboard_player_piece(
      g,
      c,
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
  // #20: A queen cannot attack over pieces
  {
    game g;
    game_controller c;
    assert(is_piece_at(g, square("d1")));
    do_select_and_start_attack_keyboard_player_piece(
      g,
      c,
      square("d1"),
      square("d8")
    );
    assert(is_piece_at(g, square("d1")));
    const auto white_queen_id{get_piece_at(g, square("d1")).get_id()};
    for (int i{0}; i!=10; ++i)
    {
      g.tick(delta_t(0.25));
    }
    const piece& p{get_piece_with_id(g, white_queen_id)};
    assert(is_piece_at(g, square("d1")));
    assert(p.get_messages().back() == message_type::cannot);
    const auto messages{get_piece_at(g, square("d1")).get_messages()};
    assert(messages.back() == message_type::cannot);
  }
  //#define FIX_ISSUE_22
  #ifdef FIX_ISSUE_22
  // play replay of scholars mate with two keyboards
  {
    const physical_controllers pcs{create_two_keyboard_controllers()};
    const replay r(get_scholars_mate_as_pgn_str());
    const bool verbose{true};
    play_game_with_controller(pcs, r, verbose);
  }
  // play replay 1 with two keyboards
  {
    const physical_controllers pcs{create_two_keyboard_controllers()};
    const replay r(get_replay_1_as_pgn_str());
    const bool verbose{true};
    play_game_with_controller(pcs, r, verbose);
  }
  assert(1==2);
  #endif

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
