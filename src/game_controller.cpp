#include "game_controller.h"

#include "chess_move.h"
#include "game.h"
#include "game_options.h"
#include "lobby_options.h"
#include "message.h"
#include "piece.h"
#include "pieces.h"
#include "message_type.h"

#include <cassert>
#include <sstream>

#ifdef PHYSICAL_CONTROLLERS_H
#error 'pieces' must know nothing about 'physical_controllers'.
#error Instead, it is the other way around...
#endif

game_controller::game_controller(const game& g, const lobby_options& lo)
  : m_game{g},
    m_lobby_options{lo}
{
  m_cursor_pos[side::lhs] = game_coordinate(0.5, 4.5);
  m_cursor_pos[side::rhs] = game_coordinate(7.5, 4.5);
  m_selected_piece_id[side::lhs] = {};
  m_selected_piece_id[side::rhs] = {};
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

void game_controller::apply_user_inputs_to_game()
{
  check_selected_pieces_exist();

  game& g{m_game};
  std::map<side, std::vector<user_input>> user_inputs;
  user_inputs[side::lhs] = {};
  user_inputs[side::rhs] = {};
  for (const auto& user_input: m_user_inputs.get_user_inputs())
  {
    user_inputs[user_input.get_player()].push_back(user_input);
  }

  for (const side s: get_all_sides())
  {
    for (const auto& user_input: user_inputs.at(s))
    {
      // Need to update every input
      const auto actions = get_piece_actions(*this, s);
      switch(user_input.get_user_input_type())
      {
        case user_input_type::press_down:
        {
          const auto pos{get_cursor_pos(user_input.get_player())};
          set_cursor_pos(get_below(pos), user_input.get_player());
        }
        break;
        case user_input_type::press_left:
        {
          const auto pos{get_cursor_pos(user_input.get_player())};
          set_cursor_pos(get_left(pos), user_input.get_player());
        }
        break;
        case user_input_type::press_right:
        {
          const auto pos{get_cursor_pos(user_input.get_player())};
          set_cursor_pos(get_right(pos), user_input.get_player());
        }
        break;
        case user_input_type::press_up:
        {
          const auto pos{get_cursor_pos(user_input.get_player())};
          set_cursor_pos(get_above(pos), user_input.get_player());
        }
        break;
        case user_input_type::mouse_move:
        {
          if (is_coordinat_on_board(user_input.get_coordinat().value()))
          {
            assert(user_input.get_coordinat());
            set_cursor_pos(user_input.get_coordinat().value(), user_input.get_player());
            assert(get_cursor_pos(user_input.get_player()) == user_input.get_coordinat().value());
            assert(square(get_cursor_pos(user_input.get_player())) == square(user_input.get_coordinat().value()));
          }
        }
        break;
        case user_input_type::rmb_down:
        {
          if (actions.size() < 2) continue;
          apply_action_type_to_game(g, actions[1], s);
        }
        break;
        case user_input_type::lmb_down:
        {
          if (actions.size() < 1) continue;
          apply_action_type_to_game(g, actions[0], s);
        }
        break;
        case user_input_type::press_action_1:
        {
          if (actions.size() < 1)
          {
            continue;
          }
          if (actions[0] == piece_action_type::attack_en_passant)
          {
            const auto selected_piece{get_selected_pieces(*this, s)[0]};
            const auto selected_piece_type{selected_piece.get_type()};
            assert(selected_piece_type == piece_type::pawn && "#156");
          }

          apply_action_type_to_game(g, actions[0], s);
        }
        break;
        case user_input_type::press_action_2:
        {
          if (actions.size() < 2) continue;
          apply_action_type_to_game(g, actions[1], s);
        }
        break;
        case user_input_type::press_action_3:
        {
          if (actions.size() < 3) continue;
          apply_action_type_to_game(g, actions[2], s);
        }
        break;
        case user_input_type::press_action_4:
        {
          if (actions.size() < 4) continue;
          apply_action_type_to_game(g, actions[3], s);
        }
        break;
      }
      g.tick(delta_t(0.0));
    }
  }
  m_user_inputs = std::vector<user_input>();
  check_selected_pieces_exist();
}

void game_controller::apply_action_type_to_game(game& g, const piece_action_type t, const side s)
{
  switch (t)
  {
    case piece_action_type::attack: apply_action_type_attack_to_game(g, s); break;
    case piece_action_type::attack_en_passant:
    {
      const auto selected_piece{get_selected_pieces(*this, s)[0]};
      const auto selected_piece_type{selected_piece.get_type()};
      assert(selected_piece_type == piece_type::pawn && "#156");
      apply_action_type_attack_en_passant_to_game(g, s); break;
    }
    case piece_action_type::castle_kingside: apply_action_type_castle_kingside_to_game(g, s); break;
    case piece_action_type::castle_queenside: apply_action_type_castle_queenside_to_game(g, s); break;
    case piece_action_type::move:
      assert(has_selected_pieces(*this, s));
      apply_action_type_move_to_game(g, s);
      break;
    case piece_action_type::promote_to_bishop: apply_action_type_promote_to_game(g, piece_type::bishop, s); break;
    case piece_action_type::promote_to_knight: apply_action_type_promote_to_game(g, piece_type::knight, s); break;
    case piece_action_type::promote_to_queen: apply_action_type_promote_to_game(g, piece_type::queen, s); break;
    case piece_action_type::promote_to_rook: apply_action_type_promote_to_game(g, piece_type::rook, s); break;
    case piece_action_type::select: apply_action_type_select_to_game(g, s); break;
    case piece_action_type::unselect: apply_action_type_unselect_to_game(g, s); break;

  }
}

void game_controller::apply_action_type_attack_to_game(game& g, const side s)
{
  const chess_color player_color{m_lobby_options.get_color(s)};
  const game_coordinate cursor_pos{get_cursor_pos(s)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};
  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  const bool is_cursor_on_enemy_piece{
    is_cursor_on_piece && get_piece_at(g, cursor).get_color() != player_color
  };

  assert(is_cursor_on_enemy_piece);

  assert(m_selected_piece_id.at(s).has_value());
  //assert(has_selected_pieces(g, s));
  //assert(has_selected_pieces(g, player_color));
  assert(!get_selected_pieces(*this, s).empty());
  assert(get_selected_pieces(*this, s).size() == 1);
  const auto selected_piece{get_selected_pieces(*this, s)[0]};
  const auto selected_piece_type{selected_piece.get_type()};
  const auto selected_piece_square{selected_piece.get_current_square()};

  assert(
    can_attack_on_empty_board(
      player_color,
      selected_piece_type,
      selected_piece_square,
      cursor
    )
  );

  start_attack(
    g,
    *this,
    cursor_pos,
    player_color
  );
}

void game_controller::apply_action_type_attack_en_passant_to_game(game& g, const side s)
{
  assert(!g.get_pieces().empty());
  assert(!to_str(s).empty());


  const chess_color player_color{m_lobby_options.get_color(s)};
  const game_coordinate cursor_pos{get_cursor_pos(s)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};
  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  assert(!is_cursor_on_piece);


  assert(has_selected_pieces(*this, s));
  assert(has_selected_pieces(*this, player_color));
  assert(!get_selected_pieces(*this, s).empty());
  assert(get_selected_pieces(*this, s).size() == 1);
  const auto selected_piece{get_selected_pieces(*this, s)[0]};
  const auto selected_piece_type{selected_piece.get_type()};
  assert(selected_piece_type == piece_type::pawn && "#156");

  assert(can_attack_en_passant(*this, s));

  start_en_passant_attack(
    g,
    *this,
    cursor_pos,
    player_color
  );
}


void game_controller::apply_action_type_castle_kingside_to_game(game& g, const side player_side)
{
  const chess_color player_color{m_lobby_options.get_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos(player_side)};
  assert(is_coordinat_on_board(cursor_pos));

  const square king_square{get_initial_king_square(player_color)};

  const bool is_castle_kingside{
       is_piece_at(g, king_square)
    && get_piece_at(g, king_square).get_color() == player_color
    // && get_piece_at(c, king_square).is_selected()
    && can_castle_kingside(get_piece_at(g, king_square), g)
  };
  assert(is_castle_kingside);

  // Unselect all pieces
  //unselect_all_pieces(g, player_color);

  // King starts promoting

  const square king_target_square{get_king_target_square(player_color, piece_action_type::castle_kingside)};
  get_piece_at(g, king_square).add_action(
    piece_action(
      player_color,
      piece_type::king,
      piece_action_type::castle_kingside,
      king_square,
      king_target_square
    )
  );

  // Rook starts promoting
  const square rook_target_square{get_rook_target_square(player_color, piece_action_type::castle_kingside)};
  get_piece_at(g, get_initial_rook_square(player_color, castling_type::king_side)).add_action(
    piece_action(
      player_color,
      piece_type::rook,
      piece_action_type::castle_kingside,
      get_initial_rook_square(player_color, castling_type::king_side),
      rook_target_square
    )
  );
}


void game_controller::apply_action_type_castle_queenside_to_game(game& g, const side player_side)
{
  const chess_color player_color{m_lobby_options.get_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos(player_side)};
  assert(is_coordinat_on_board(cursor_pos));

  const square king_square{get_initial_king_square(player_color)};

  const bool is_castle_queenside{
       is_piece_at(g, king_square)
    && get_piece_at(g, king_square).get_color() == player_color
    // && get_piece_at(c, king_square).is_selected()
    && can_castle_queenside(get_piece_at(g, king_square), g)
  };
  assert(is_castle_queenside);

  // King start castling
  const square king_target_square{get_king_target_square(player_color, piece_action_type::castle_queenside)};
  get_piece_at(g, get_initial_king_square(player_color)).add_action(
    piece_action(
      player_color,
      piece_type::king,
      piece_action_type::castle_queenside,
      get_initial_king_square(player_color),
      king_target_square
    )
  );
  // Rook starts castling
  const square rook_target_square{get_rook_target_square(player_color, piece_action_type::castle_queenside)};
  get_piece_at(g, get_initial_rook_square(player_color, castling_type::queen_side)).add_action(
    piece_action(
      player_color,
      piece_type::rook,
      piece_action_type::castle_queenside,
      get_initial_rook_square(player_color, castling_type::queen_side),
      rook_target_square
    )
  );
}


void game_controller::apply_action_type_move_to_game(game& g, const side player_side)
{
  assert(has_selected_pieces(*this, player_side)); // ???

  const chess_color player_color{m_lobby_options.get_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos(player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};
  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  const bool is_cursor_on_friendly_piece{
    is_cursor_on_piece && get_piece_at(g, cursor).get_color() == player_color
  };

  assert(!is_cursor_on_friendly_piece);
  assert(has_selected_pieces(*this, player_side));
  assert(has_selected_pieces(*this, player_color));
  assert(!get_selected_pieces(*this, player_side).empty());
  assert(get_selected_pieces(*this, player_side).size() == 1);
  const auto selected_piece{get_selected_pieces(*this, player_side)[0]};
  const auto selected_piece_type{selected_piece.get_type()};
  const auto selected_piece_square{selected_piece.get_current_square()};

  assert(
    can_move(
      g.get_pieces(),
      player_color,
      selected_piece_type,
      selected_piece_square,
      cursor
    )
  );

  start_move_unit(
    g,
    *this,
    cursor_pos,
    player_color
  );
}


void game_controller::apply_action_type_promote_to_game(
  game& g,
  const piece_type promote_to_type,
  const side player_side
)
{
  const chess_color player_color{m_lobby_options.get_color(player_side)};

  assert(has_selected_pieces( *this, player_side));
  const auto selected_pieces{get_selected_pieces(*this, player_side)};
  assert(selected_pieces.size() == 1);
  const auto selected_square{selected_pieces[0].get_current_square()};
  assert(can_promote(get_piece_at(g, selected_square)));

  auto& p{get_piece_at(g, selected_square)};
  //assert(p.is_selected());
  assert(can_promote(p));

  piece_action_type pat{piece_action_type::attack_en_passant};
  switch (promote_to_type)
  {
    case piece_type::bishop: pat = piece_action_type::promote_to_bishop; break;
    case piece_type::knight: pat = piece_action_type::promote_to_knight; break;
    case piece_type::queen: pat = piece_action_type::promote_to_queen; break;
    default:
    case piece_type::rook:
      assert(promote_to_type == piece_type::rook);
      pat = piece_action_type::promote_to_rook;
      break;
  }

  assert(is_promotion);
  p.add_action(
    piece_action(
      player_color,
      promote_to_type,
      pat,
      selected_square,
      selected_square
    )
  );
}

void game_controller::apply_action_type_select_to_game(
  game& g,
  const side player_side
)
{
  const chess_color player_color{m_lobby_options.get_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos(player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};

  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  const bool is_cursor_on_friendly_piece{
    is_cursor_on_piece && get_piece_at(g, cursor).get_color() == player_color
  };
  assert(is_cursor_on_friendly_piece);

  // Unselect the old piece
  if (m_selected_piece_id[player_side].has_value())
  {
    assert(has_piece_with_id(g, m_selected_piece_id[player_side].value()) && "#137");
    auto& previously_selected_piece{get_piece_with_id(g, m_selected_piece_id[player_side].value())};
    assert(previously_selected_piece.get_color() == player_color);
    previously_selected_piece.add_action(
      piece_action(
        player_color,
        previously_selected_piece.get_type(),
        piece_action_type::unselect,
        previously_selected_piece.get_current_square(),
        previously_selected_piece.get_current_square()
      )
    );
  }

  auto& newly_selected_piece{get_piece_at(g, cursor)};

  newly_selected_piece.add_action(
    piece_action(
      player_color,
      newly_selected_piece.get_type(),
      piece_action_type::select,
      cursor,
      cursor
    )
  );

  assert(
       !this->get_selected_piece_id(player_side).has_value()
    ||  this->get_selected_piece_id(player_side).value() != newly_selected_piece.get_id()
  );

  this->set_selected_piece_id(player_side, newly_selected_piece.get_id());
}


void game_controller::apply_action_type_unselect_to_game(game& g, const side player_side)
{
  const chess_color player_color{m_lobby_options.get_color(player_side)};
  const game_coordinate cursor_pos{get_cursor_pos( player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  const square cursor{square(cursor_pos)};

  const bool is_cursor_on_piece{is_piece_at(g, cursor)};
  const bool is_cursor_on_friendly_piece{
    is_cursor_on_piece && get_piece_at(g, cursor).get_color() == player_color
  };

  assert(is_cursor_on_friendly_piece);
  auto& p{get_piece_at(g, cursor)};

  p.add_action(
    piece_action(
      player_color,
      p.get_type(),
      piece_action_type::unselect,
      cursor,
      cursor
    )
  );
  assert(this->get_selected_piece_id(player_side).value() == p.get_id());
  this->set_selected_piece_id(player_side, {} );
}

bool can_attack(
  const game_controller& c,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(c, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};

  if (selected_piece.get_current_action_progress() != delta_t(0.0))
  {
    // Already moving
    return false;
  }

  if (!selected_piece.get_messages().empty()
    && selected_piece.get_messages().back() == message_type::start_attack
  )
  {
    // Already attacking
    return false;
  }
  const square cursor_square(square(c.get_cursor_pos(player_side)));
  return can_do_attack(c.get_game(), selected_piece, cursor_square, lobby_options().get_color(player_side));
}

bool can_attack_en_passant(
  const game_controller& c,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(c, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};

  if (selected_piece.get_current_action_progress() != delta_t(0.0))
  {
    // Already moving
    return false;
  }

  if (!selected_piece.get_messages().empty()
    && selected_piece.get_messages().back() == message_type::start_en_passant_attack
  )
  {
    // Already attacking
    return false;
  }
  const square cursor_square(square(c.get_cursor_pos(player_side)));
  return can_do_en_passant(c.get_game(), selected_piece, cursor_square, c.get_lobby_options().get_color(player_side));
}

bool can_castle_kingside(
  const game_controller& c,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(c, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};

  if (selected_piece.get_current_action_progress() != delta_t(0.0))
  {
    // Already moving
    return false;
  }

  if (selected_piece.get_current_action_progress() != delta_t(0.0))
  {
    // Already castling
    return false;
  }

  // 'can_castle_kingside' will check if it is a king
  return can_castle_kingside(selected_piece, c.get_game());
}

bool can_castle_queenside(
  const game_controller& c,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(c, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};

  if (selected_piece.get_current_action_progress() != delta_t(0.0))
  {
    // Already castling
    return false;
  }

  // 'can_castle_kingside' will check if it is a king
  return can_castle_queenside(selected_piece, c.get_game());
}

bool can_move(
  const game_controller& c,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(c, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};
  if (selected_piece.get_current_action_progress() != delta_t(0.0))
  {
    // Already moving
    return false;
  }
  const square cursor_square(square(c.get_cursor_pos(player_side)));
  return can_do_move(c.get_game(), selected_piece, cursor_square, c.get_lobby_options().get_color(player_side));
}

bool can_player_select_piece_at_cursor_pos(
  const game_controller& c,
  const chess_color cursor_color
)
{
  const auto& cursor_pos{
    get_cursor_pos(c, c.get_lobby_options().get_side(cursor_color))
  };
  const double click_distance{get_default_click_distance()};
  if (!is_piece_at(c.get_game(), cursor_pos, click_distance))
  {
    return false;
  }
  const auto& piece{get_closest_piece_to(c.get_game(), cursor_pos)};
  const bool is_piece_already_selected{
    c.get_selected_piece_id(c.get_lobby_options().get_side(cursor_color)) == piece.get_id()
  };

  // Cannot select a selected piece
  if (is_piece_already_selected) return false;

  const bool does_player_own_that_piece{piece.get_color() == cursor_color};
  return does_player_own_that_piece;
}

bool can_player_select_piece_at_cursor_pos(
  const game_controller& c,
  const side player_side
)
{
  return can_player_select_piece_at_cursor_pos(
    c, c.get_lobby_options().get_color(player_side)
  );
}

bool can_promote(
  const game_controller& c,
  const side player_side
) noexcept
{
  const auto selected_pieces{get_selected_pieces(c, player_side)};
  if (selected_pieces.empty()) return false;
  assert(selected_pieces.size() == 1);
  const auto& selected_piece{selected_pieces[0]};

  // can_promote will check if it is a pawn
  return can_promote(selected_piece);
}

bool can_select(
  const game_controller& c,
  const side player_side
) noexcept
{
  return can_player_select_piece_at_cursor_pos(c, player_side);
}

bool can_unselect(
  const game_controller& c,
  const side player_side
) noexcept
{
  return is_cursor_on_selected_piece(c, player_side);
}

void game_controller::check_selected_pieces_exist()
{
  for (const side s: get_all_sides())
  {
    if (!m_selected_piece_id[s].has_value()) continue;
    assert(has_piece_with_id(m_game, m_selected_piece_id[s].value()) && "#137");
  }
}

std::vector<piece_id> collect_selected_piece_ids(const game_controller& c)
{
  std::vector<piece_id> v;
  for (const side s: get_all_sides())
  {
    if (c.get_selected_piece_id(s).has_value())
    {
      v.push_back(c.get_selected_piece_id(s).value());
    }
  }
  return v;
}

user_inputs convert_move_to_user_inputs(
  const game_controller& c,
  const chess_move& m,
  const physical_controller_type t
)
{
  // The moves '0-1', or '1-0' have no user inputs
  if (!m.get_winner().empty()) return user_inputs();

  const auto player_side{c.get_lobby_options().get_side(m.get_color())};

  assert(m.get_from().has_value());
  const square from{m.get_from().value()};

  user_inputs inputs;
  // Move the cursor to piece's square
  {
    const auto v{get_user_inputs_to_move_cursor_to(c, from, player_side, t)};
    add(inputs, v);
  }
  // Select the piece
  {
    const auto i{get_user_input_to_select(player_side, t)};
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
        player_side,
        t
      )
    };
    add(inputs, v);
  }
  // Do the action
  {
    const auto i{get_user_input_to_do_action_1(player_side, t)};
    inputs.add(i);
  }
  return inputs;
}

int count_selected_units(const game_controller& c, const chess_color player_color)
{
  return c.get_selected_piece_id(c.get_lobby_options().get_side(player_color)).has_value();
}

int count_user_inputs(const game_controller& c) noexcept
{
  return count_user_inputs(c.get_user_inputs());
}

const game_coordinate& game_controller::get_cursor_pos(const side player) const noexcept
{
  return m_cursor_pos.at(player);
}

const game_coordinate& get_cursor_pos(
  const game_controller&c,
  const side player_side
) noexcept
{
  return c.get_cursor_pos(player_side);
}

const game_coordinate& get_cursor_pos(
  const game_controller& c,
  const chess_color cursor_color
)
{
  return get_cursor_pos(c, c.get_lobby_options().get_side(cursor_color));
}

square get_cursor_square(
  const game_controller& c,
  const side player_side
)
{
  const game_coordinate cursor_pos{get_cursor_pos(c, player_side)};
  assert(is_coordinat_on_board(cursor_pos));
  return square(cursor_pos);
}

const in_game_time& get_in_game_time(
  const game_controller& c
) noexcept
{
  return c.get_game().get_in_game_time();
}

std::vector<piece_action_type> get_piece_actions(
  const game_controller& c,
  const side player_side
) noexcept
{
  std::vector<piece_action_type> actions;

  // 1.2
  if (
    c.get_lobby_options().get_color(player_side) == chess_color::black
    && get_in_game_time(c) < in_game_time(1.0)
  )
  {
    return actions;
  }

  if (can_select(c, player_side))
  {
    actions.push_back(piece_action_type::select);
  }
  if (can_unselect(c, player_side))
  {
    actions.push_back(piece_action_type::unselect);
  }
  if (can_attack(c, player_side))
  {
    actions.push_back(piece_action_type::attack);
  }
  if (can_attack_en_passant(c, player_side))
  {
    actions.push_back(piece_action_type::attack_en_passant);
  }
  if (can_move(c, player_side))
  {
    actions.push_back(piece_action_type::move);
  }
  if (can_castle_kingside(c, player_side))
  {
    actions.push_back(piece_action_type::castle_kingside);
  }
  if (can_castle_queenside(c, player_side))
  {
    actions.push_back(piece_action_type::castle_queenside);
  }
  if (can_promote(c, player_side))
  {
    actions.push_back(piece_action_type::promote_to_queen);
    actions.push_back(piece_action_type::promote_to_rook);
    actions.push_back(piece_action_type::promote_to_bishop);
    actions.push_back(piece_action_type::promote_to_knight);
  }
  return actions;
}

piece& get_piece_at(game_controller& c, const std::string& square_str)
{
  return get_piece_at(c.get_game(), square_str);
}

piece& get_piece_at(game_controller& c, const square& s)
{
  return get_piece_at(c.get_game(), s);
}


std::vector<square> get_possible_moves(
  const game_controller& c,
  const side player_side
)
{
  const auto maybe_selected_piece_id{c.get_selected_piece_id(player_side)};
  if (!maybe_selected_piece_id.has_value()) return {};
  const piece_id& selected_piece_id{maybe_selected_piece_id.value()};
  assert(has_piece_with_id(c.get_game(), selected_piece_id));
  const auto& focal_piece{get_piece_with_id(c.get_game(), selected_piece_id)};
  return get_possible_moves(c.get_game().get_pieces(), focal_piece);
}

std::vector<piece> get_selected_pieces(
  const game_controller& c,
  const chess_color player_color
)
{
  return get_selected_pieces(c, c.get_lobby_options().get_side(player_color));
}

std::vector<piece> get_selected_pieces(
  const game_controller& c,
  const side player_side
)
{
  const auto maybe_selected_piece_id{c.get_selected_piece_id(player_side)};
  if (!maybe_selected_piece_id.has_value()) return {};
  const piece_id& selected_piece_id{maybe_selected_piece_id.value()};
  if (!has_piece_with_id(c.get_game(), selected_piece_id)) return {};
  return { get_piece_with_id(c.get_game(), selected_piece_id) };
}


const std::optional<piece_id>& game_controller::get_selected_piece_id(const side s) const noexcept
{
  return m_selected_piece_id.at(s);
}

const user_inputs& get_user_inputs(const game_controller& c) noexcept
{
 return c.get_user_inputs();
}

user_inputs get_user_inputs_to_move_cursor_from_to(
  const game_controller& c,
  const square& from,
  const square& to,
  const side player_side,
  const physical_controller_type t
)
{
  if (t == physical_controller_type::mouse)
  {
    // A mouse user 'just' moves its mouse at the correct position,
    // regardless of the current cursors' position
    return get_user_inputs_to_move_cursor_to(c, to, player_side, t);
  }
  else
  {
    assert(t == physical_controller_type::keyboard);
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
  const side player_side,
  const physical_controller_type t
)
{
  if (t == physical_controller_type::mouse)
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
    assert(t == physical_controller_type::keyboard);
    // A keyboard user must move the cursor from its existing position
    const square from{get_cursor_pos(c, player_side)};
    return get_user_inputs_to_move_cursor_from_to(
      c,
      from,
      to,
      player_side,
      t
    );
  }
}

const std::optional<chess_color>& get_winner(const game_controller& c) noexcept
{
  return c.get_game().get_winner();
}

bool has_selected_pieces(
  const game_controller& c,
  const chess_color player_color
)
{
  return !get_selected_pieces(c, player_color).empty();
}

bool has_selected_pieces(
  const game_controller& c,
  const side player_side
)
{
  return !get_selected_pieces(c, player_side).empty();
}

bool has_winner(const game_controller& c) noexcept
{
  return get_winner(c).has_value();
}

bool is_cursor_on_selected_piece(
  const game_controller& c,
  const side player_side
) noexcept
{
  if (!has_selected_pieces(c, player_side)) return false;
  const auto& cursor_coordinate{c.get_cursor_pos(player_side)};
  const square cursor_square(cursor_coordinate);
  const auto selected_pieces{get_selected_pieces(c, player_side)};
  assert(selected_pieces.size() == 1);
  const auto piece_square{selected_pieces[0].get_current_square()};
  return cursor_square == piece_square;
}

bool is_draw(const game_controller& c)
{
  return is_draw(c.get_game());
}

bool is_piece_at(
  const game_controller& g,
  const square& coordinate
)
{
  return is_piece_at(
    g.get_game(),
    coordinate
  );
}

/// Determine if there is a piece at the coordinat
bool is_piece_at(
  const game_controller& g,
  const std::string& square_str
)
{
  return is_piece_at(
    g.get_game(),
    square_str
  );
}

bool is_selected(const piece& p, const game_controller& c)
{
  const chess_color player_color{p.get_color()};
  const auto player_side{c.get_lobby_options().get_side(player_color)};
  const auto maybe_selected_piece_id{c.get_selected_piece_id(player_side)};
  if (!maybe_selected_piece_id.has_value()) return false;
  const auto piece_id{p.get_id()};
  return maybe_selected_piece_id.value() == piece_id;
}

bool is_square_attacked(
  const game_controller& c,
  const square& s,
  const chess_color attacker_color
)
{
  return is_square_attacked(c.get_game(), s, attacker_color);
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

game play_random_game(const int n_turns, const int seed)
{
  game g{create_game_with_standard_starting_position()};
  game_controller c;

  std::default_random_engine rng_engine(seed);

  for (int i=0; i!=n_turns; ++i)
  {
    const auto user_input{create_useful_random_user_input(rng_engine)};
    c.add_user_input(user_input);
    c.apply_user_inputs_to_game();
    assert(c.get_user_inputs().get_user_inputs().empty());
    c.tick(delta_t(0.1));
    if (g.get_winner().has_value()) break;
  }
  return g;
}

void set_cursor_pos(
  game_controller& c,
  const game_coordinate& pos,
  const side player_side
) noexcept
{
  c.set_cursor_pos(pos, player_side);
}

void game_controller::set_cursor_pos(
  const game_coordinate& pos,
  const side player_side) noexcept
{
  m_cursor_pos[player_side] = pos;
}

void game_controller::set_selected_piece_id(
  const side s,
  const std::optional<piece_id>& selected_piece_id
) noexcept
{
  m_selected_piece_id[s] = selected_piece_id;
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
  // collect_selected_piece_ids, no selected pieces
  {
    const game_controller c;
    assert(collect_selected_piece_ids(c).empty());
  }
  // collect_selected_piece_ids, select the piece at the cursor
  {
    game_controller c;
    c.add_user_input(
      create_press_action_1(side::lhs)
    );
    c.apply_user_inputs_to_game();
    assert(!collect_selected_piece_ids(c).empty());
  }
  // convert_move_to_user_inputs
  {
    const game_controller c;
    const auto cursor_pos{square(c.get_cursor_pos(side::lhs))};
    assert(cursor_pos == square("e1"));
    const chess_move m(pgn_move_string("e4"));
    const auto user_inputs{
      convert_move_to_user_inputs(c, m, physical_controller_type::keyboard)
    };
    // Move cursor from e1 to e2
    assert(user_inputs.get_user_inputs().at(0).get_user_input_type() == user_input_type::press_right);

    // Select the pawn at e2
    assert(user_inputs.get_user_inputs().at(1).get_user_input_type() == user_input_type::press_action_1);

    // Move cursor from e2 to e3
    assert(user_inputs.get_user_inputs().at(2).get_user_input_type() == user_input_type::press_right);

    // Move cursor from e3 to e4
    assert(user_inputs.get_user_inputs().at(3).get_user_input_type() == user_input_type::press_right);

    // Select the target square at e4
    assert(user_inputs.get_user_inputs().at(4).get_user_input_type() == user_input_type::press_action_1);

    assert(user_inputs.get_user_inputs().size() == 5);
  }

  // count_user_inputs
  {
    game_controller c;
    assert(count_user_inputs(c) == 0);
  }
  // get_user_inputs_to_move_cursor_from_to
  {
    game_controller c;
    const auto keyboard_inputs{
      get_user_inputs_to_move_cursor_from_to(
        c, square("e2"), square("e4"), side::lhs,
        physical_controller_type::keyboard
      )
    };
    const auto mouse_inputs{
      get_user_inputs_to_move_cursor_from_to(
        c, square("e2"), square("e4"), side::lhs,
        physical_controller_type::mouse
      )
    };
    assert(keyboard_inputs != mouse_inputs);
  }
  // Moving the mouse
  {
    game_controller c;
    c.add_user_input(create_mouse_move_action(game_coordinate(0.5, 0.5), side::lhs));
    c.apply_user_inputs_to_game();
    assert(c.get_cursor_pos(side::lhs) == game_coordinate(0.5, 0.5));

    // Does nothing
    c.add_user_input(create_mouse_move_action(game_coordinate(314.15, 42.00), side::lhs));
    c.apply_user_inputs_to_game();

    // Cursor is still the old position
    assert(c.get_cursor_pos(side::lhs) == game_coordinate(0.5, 0.5));
  }

  // play_random_game
  {
    const int n_turns{2};
    play_random_game(n_turns);
  }
  // 55: move_cursor_to
  {
    game_controller c;
    //game_controller c{create_game_controller_with_keyboard_mouse()};
    move_cursor_to(c, "d1", side::lhs);
    assert(get_cursor_pos(c, side::lhs) == to_coordinat(square("d1")));
    move_cursor_to(c, "f6", side::rhs);
    assert(get_cursor_pos(c, side::rhs) == to_coordinat(square("f6")));
  }
  // Keyboard: select white king
  {
    game_controller c;
    //game_controller c{
    //  create_game_controller_with_keyboard_mouse(create_game_with_standard_starting_position())
    //};
    const auto white_king{find_pieces(c.get_game(), piece_type::king, chess_color::white).at(0)};
    set_cursor_pos(c, to_coordinat(white_king.get_current_square()), side::lhs);
    assert(count_selected_units(c, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    assert(count_selected_units(c, chess_color::white) == 0);
    c.apply_user_inputs_to_game();
    assert(count_selected_units(c, chess_color::white) == 1);
  }
  // 60: selectedness is transferred, for white
  {
    game_controller c;
    //game_controller c{
    //  create_game_controller_with_keyboard_mouse(create_game_with_standard_starting_position())
    //};
    move_cursor_to(c, "e1", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 0);
    assert(!is_selected(get_piece_at(c.get_game(), "e1")));

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));

    assert(count_selected_units(c, chess_color::white) == 1);
    assert(is_selected(get_piece_at(c.get_game(), "e1")));

    move_cursor_to(c, "d1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));

    assert(count_selected_units(c, chess_color::white) != 2);
    assert(count_selected_units(c, chess_color::white) != 0);
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(is_selected(get_piece_at(c.get_game(), "d1")));
    assert(!is_selected(get_piece_at(c.get_game(), "e1")));
  }
  #ifdef FIX_SELECTEDNESS
  // selectedness is transferred, for black
  {
    game_controller c;
    move_cursor_to(c, "e8", side::rhs);
    assert(get_cursor_square(c, side::rhs) == square("e8"));
    assert(count_selected_units(c, chess_color::black) == 0);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.01));
    assert(count_selected_units(c, chess_color::black) == 1);
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.01));
    assert(count_selected_units(c, chess_color::black) == 1);
  }
  #endif // FIX_SELECTEDNESS
  // Selecting a unit twice with action 1 selects and unselects it
  {
    game_controller c;
    assert(count_selected_units(c, chess_color::white) == 0);
    move_cursor_to(c, "e1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    assert(count_selected_units(c, chess_color::white) == 0);
  }
  // Cannot select two units
  {
    game_controller c;
    do_select(c, "d2", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    do_select(c, "e2", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
  }
  // Keyboard: can move pawn forward
  {
    game_controller c;
    move_cursor_to(c, "e2", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick();
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(collect_messages(c.get_game()).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "e4", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));

    assert(has_selected_pieces(c, side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    c.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    c.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    c.tick(delta_t(0.25)); // Moves it to e3, unselects piece
    assert(get_closest_piece_to(c.get_game(), to_coordinat("e3")).get_type() == piece_type::pawn);
    const auto messages{collect_messages(c.get_game())};
    const auto message{messages.at(1)};
    assert(message.get_message_type() == message_type::start_move);
  }
  // Keyboard: cannot move pawn backward
  {
    game_controller c{
      game(get_pieces_pawn_all_out_assault()),
      lobby_options()
    };
    //game_controller c{create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::pawn_all_out_assault))};
    move_cursor_to(c, "e4", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 0);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.01));
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(collect_messages(c.get_game()).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "e3", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.01)); // Ignores invalid action, adds sound effect

    // No effect, so unit stays selected
    //assert(count_selected_units(c, chess_color::white) == 0);

    assert(get_closest_piece_to(c.get_game(), to_coordinat("e4")).get_type() == piece_type::pawn);

    // Cannot be done, so no message
    // assert(collect_messages(g).at(1).get_message_type() == message_type::cannot);
  }
  // white castles kingside
  {
    game_controller c{
      game(get_pieces_ready_to_castle())
    };
    do_select(c, "e1", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(collect_messages(c.get_game()).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "g1", side::lhs); // Cursor must not be on king
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));
    const auto msg{collect_messages(c.get_game())};
    assert(msg.at(1).get_message_type() == message_type::start_castling_kingside);
  }
  // white castles queenside
  {
    game_controller c{
      game(get_pieces_ready_to_castle())
    };
    do_select(c, "e1", side::lhs);

    c.tick(delta_t(0.0));
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(collect_messages(c.get_game()).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "g1", side::lhs); // Cursor must not be on king
    assert(count_selected_units(c, chess_color::white) == 1);

    add_user_input(c, create_press_action_2(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));


    const auto msg{collect_messages(c.get_game())};
    assert(msg.at(0).get_message_type() == message_type::start_castling_queenside);
    assert(msg.at(2).get_message_type() == message_type::start_castling_queenside);
  }
  // Clicking a unit once with LMB selects it
  {
    game_controller c{
      game(get_standard_starting_pieces())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    assert(count_selected_units(c, chess_color::black) == 0);
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game();
    assert(count_selected_units(c, chess_color::black) == 1);
    c.tick(delta_t(0.0));
    assert(count_selected_units(c, chess_color::black) == 1);
  }
  // Clicking a unit twice with LMB selects and unselects it
  {
    game_controller c{
      game(get_standard_starting_pieces())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    assert(count_selected_units(c, chess_color::black) == 0);
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));
    assert(count_selected_units(c, chess_color::black) == 1);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));
    assert(count_selected_units(c, chess_color::black) == 0);
  }
  // 60: selectedness is transferred
  // Clicking a unit with LMB,
  // then another unit with LMB, only the last unit is selected
  {
    game_controller c{
      game(get_standard_starting_pieces()),
      lobby_options()
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    assert(count_selected_units(c, chess_color::black) == 0);
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));
    assert(count_selected_units(c, chess_color::black) == 1);
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.tick(delta_t(0.0));
    assert(count_selected_units(c, chess_color::black) != 2);
    assert(count_selected_units(c, chess_color::black) != 0);
    assert(count_selected_units(c, chess_color::black) == 1);
  }
  // Ke8e7 works by LMB, LMB
  {
    game_controller c{
      game(get_pieces_kings_only())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    move_cursor_to(c, "e8", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));
    assert(count_piece_actions(c.get_game(), chess_color::black) == 0);
    move_cursor_to(c, "e7", side::rhs);
    add_user_input(c, create_press_lmb_action(side::rhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.01));
    assert(count_piece_actions(c.get_game(), chess_color::black) >= 1);
  }
  // nothing selected, cursor at empty square -> no actions, lhs
  {
    game_controller c{
      game(get_standard_starting_pieces()),
      lobby_options()
    };
    //game_controller c{create_game_controller_with_two_keyboards(create_game_with_starting_position(starting_position_type::standard))};
    move_cursor_to(c, "d4", side::lhs);
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.empty());
  }
  // nothing selected, cursor at empty square -> no actions, rhs
  {
    game_controller c;
    //game_controller c{create_game_controller_with_two_keyboards(create_game_with_starting_position(starting_position_type::standard))};
    move_cursor_to(c, "d5", side::rhs);
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.empty());
  }
  // nothing selected, cursor at square with opponent piece -> no actions, lhs
  {
    game_controller c;
    //game_controller c{create_game_controller_with_two_keyboards(create_game_with_starting_position(starting_position_type::standard))};
    move_cursor_to(c, "d8", side::lhs);
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.empty());
  }
  // 53: nothing selected, cursor at square with opponent piece -> no actions, rhs
  {
    game_controller c;
    //game_controller c{create_game_controller_with_two_keyboards(create_game_with_starting_position(starting_position_type::standard))};
    move_cursor_to(c, "d1", side::rhs);
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.empty());
  }
  // nothing selected, cursor at square of own color -> select, lhs
  {
    game_controller c;
    //game_controller c{create_game_controller_with_two_keyboards(create_game_with_starting_position(starting_position_type::standard))};
    move_cursor_to(c, "d1", side::lhs);
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::select);
  }
  // 53: nothing selected, cursor at square of own color -> select, rhs
  {
    game_controller c;
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    move_cursor_to(c, "d8", side::rhs);
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::select);
  }
  // 53: selected piece, cursor still there -> unselect, lhs
  {
    game_controller c;
    //game_controller c{create_game_controller_with_two_keyboards(create_game_with_starting_position(starting_position_type::standard))};
    move_cursor_to(c, "d1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));

    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::unselect);
  }
  // Selected piece, cursor still there -> unselect, rhs
  {
    game_controller c;
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    move_cursor_to(c, "d8", side::rhs);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.0));

    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::unselect);
  }

  // 53: Piece selected, cursor at valid other selection target, lhs
  {
    game_controller c;
    //game_controller c{create_game_controller_with_two_keyboards(create_game_with_starting_position(starting_position_type::standard))};
    do_select(c, "d2", side::lhs);
    move_cursor_to(c, "e2", side::lhs);
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::select);
  }
  // Piece selected, cursor at valid other selection target, rhs
  {
    game_controller c;
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "d7", side::rhs);
    move_cursor_to(c, "e7", side::rhs);
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::select);
  }
  // 53: Piece selected, cursor at valid target square -> move, lhs
  {
    game_controller c;
    do_select(c, "d2", side::lhs);
    move_cursor_to(c, "d3", side::lhs);
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::move);
  }
  // Piece selected, cursor at valid target square -> move, rhs
  {
    game_controller c;
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "d7", side::rhs);
    move_cursor_to(c, "d5", side::rhs);
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::move);
  }
  // 53: Piece selected, opponent at target square -> attack, lhs
  {
    game_controller c{
      game(get_pieces_queen_endgame())
    };
    //game_controller c{create_game_controller_with_two_keyboards(create_game_with_starting_position(starting_position_type::queen_end_game))};
    do_select(c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::lhs);
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::attack);
  }
  // Piece selected, opponent at target square -> attack, rhs
  {
    game_controller c{
      game(get_pieces_queen_endgame())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "d8", side::rhs);
    move_cursor_to(c, "d1", side::rhs);
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::attack);
  }
  // King-side castle, cursor on empty square, lhs
  {
    game_controller c{
      game(get_pieces_ready_to_castle())
    };
    do_select(c, "e1", side::lhs);
    move_cursor_to(c, "e4", side::lhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.size() == 2);
    assert(actions[0] == piece_action_type::castle_kingside);
    assert(actions[1] == piece_action_type::castle_queenside);
  }
  // King-side castle, cursor on empty square, rhs
  {
    game_controller c{
      game(get_pieces_ready_to_castle())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "e8", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 2);
    assert(actions[0] == piece_action_type::castle_kingside);
    assert(actions[1] == piece_action_type::castle_queenside);
  }

  // King-side castle, cursor on friendly square, lhs
  {
    game_controller c{
      game(get_pieces_ready_to_castle()),
      lobby_options()
    };
    do_select(c, "e1", side::lhs);
    move_cursor_to(c, "a1", side::lhs);
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.size() == 3);
    assert(actions[0] == piece_action_type::select);
    assert(actions[1] == piece_action_type::castle_kingside);
    assert(actions[2] == piece_action_type::castle_queenside);
  }
  // King-side castle, cursor on friendly square, rhs
  {
    game_controller c{
      game(get_pieces_ready_to_castle())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "e8", side::rhs);
    move_cursor_to(c, "a8", side::rhs);
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 3);
    assert(actions[0] == piece_action_type::select);
    assert(actions[1] == piece_action_type::castle_kingside);
    assert(actions[2] == piece_action_type::castle_queenside);
  }
  // Pawns move to the square where they are promoted -> move, lhs
  {
    game_controller c{
      game(get_pieces_pawns_near_promotion())
    };
    do_select(c, "a7", side::lhs);
    move_cursor_to(c, "a8", side::lhs);
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::move);
  }
  // 53: Pawns move to the square where they are promoted -> move, rhs
  {
    game_controller c{
      game(get_pieces_pawns_near_promotion())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "h2", side::rhs);
    move_cursor_to(c, "h1", side::rhs);
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::move);
  }
  // Selected pawns at a promotion square, empty square selected -> promote, lhs
  {
    game_controller c{
      game(get_pieces_pawns_at_promotion())
    };
    do_select(c, "a8", side::lhs);
    move_cursor_to(c, "e4", side::lhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::lhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
  }
  // Selected pawns at a promotion square, empty square selected -> promote, rhs
  {
    game_controller c{
      game(get_pieces_pawns_at_promotion())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "h1", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
  }
  // Promote to queen, from black pawn at h1, using keyboard
  {
    game_controller c{
      game(get_pieces_pawns_at_promotion())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "h1", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
    c.add_user_input(create_press_action_1(side::rhs));
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::pawn);
    c.apply_user_inputs_to_game();
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::queen);
  }
  // Promote to queen, from black pawn at h1, using mouse
  {
    game_controller c{
      game(get_pieces_pawns_at_promotion())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "h1", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
    c.add_user_input(create_press_lmb_action(side::rhs));
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::pawn);
    c.apply_user_inputs_to_game();
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::queen);
  }
  // Promote to rook, from black pawn at h1, using keyboard
  {
    game_controller c{
      game(get_pieces_pawns_at_promotion())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "h1", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
    c.add_user_input(create_press_action_2(side::rhs));
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::pawn);
    c.apply_user_inputs_to_game();
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::rook);
  }
  // Promote to rook, from black pawn at h1, using mouse
  {
    game_controller c{
      game(get_pieces_pawns_at_promotion())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "h1", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
    c.add_user_input(create_press_rmb_action(side::rhs));
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::pawn);
    c.apply_user_inputs_to_game();
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::rook);
  }
  // Promote to bishop, from black pawn at h1
  {
    game_controller c{
      game(get_pieces_pawns_at_promotion())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "h1", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
    c.add_user_input(create_press_action_3(side::rhs));
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::pawn);
    c.apply_user_inputs_to_game();
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::bishop);
  }
  // Promote to bishop, from black pawn at h1
  {
    game_controller c{
      game(get_pieces_pawns_at_promotion())
    };
    c.tick(delta_t(1.0)); // Black can move after 1 chess move
    do_select(c, "h1", side::rhs);
    move_cursor_to(c, "e5", side::rhs); // Must be an empty square, else 'select' becomes an option
    const auto actions{get_piece_actions(c, side::rhs)};
    assert(actions.size() == 4);
    assert(actions[0] == piece_action_type::promote_to_queen);
    assert(actions[1] == piece_action_type::promote_to_rook);
    assert(actions[2] == piece_action_type::promote_to_bishop);
    assert(actions[3] == piece_action_type::promote_to_knight);
    c.add_user_input(create_press_action_4(side::rhs));
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::pawn);
    c.apply_user_inputs_to_game();
    assert(get_piece_at(c.get_game(), square("h1")).get_type() == piece_type::knight);
  }
  // get_cursor_pos
  {
    game_controller c;
    assert(get_cursor_pos(c, chess_color::white) != get_cursor_pos(c, chess_color::black));
  }
  // get_mouse_player_pos
  {
    game_controller c;
    const auto pos_before{get_cursor_pos(c, side::rhs)};
    const auto pos{get_cursor_pos(c, side::rhs)};
    set_cursor_pos(c, pos + game_coordinate(0.1, 0.1), side::rhs);
    const auto pos_after{get_cursor_pos(c, side::rhs)};
    assert(pos_before != pos_after);
  }
  // get_piece_actions
  {
    game_controller c;
    //game_controller c{create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::standard))};
    const auto m{get_piece_actions(c, side::lhs)};
    assert(!m.empty());
  }
  // a2-a3 takes 1 time unit
  {
    game_controller c;
    //game_controller c{create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::standard))};
    assert(is_piece_at(c, square("a2")));
    assert(!is_piece_at(c, square("a3")));
    assert(count_selected_units(c, chess_color::white) == 0);

    do_select(c, "a2", side::lhs);
    move_cursor_to(c, "a3", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    assert(is_piece_at(c, square("a2")));
    assert(!is_piece_at(c, square("a3")));
    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("a2")));
    assert(is_piece_at(c, square("a3")));
  }
  // a2-a4 takes 1 time unit, after which the pawn is en-passantable
  {
    game_controller c;
    //game_controller c{create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::standard))};
    assert(is_piece_at(c, square("a2")));
    assert(!is_piece_at(c, square("a3")));
    assert(!is_piece_at(c, square("a4")));

    do_select(c, "a2", side::lhs);
    move_cursor_to(c, "a4", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    for (int i{0}; i!=4; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("a2")));
    assert(is_piece_at(c, square("a4")));
  }
  // a2-a4 makes the pawn en-passantable
  {
    game_controller c;
    //game_controller c{create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::standard))};
    assert(is_piece_at(c, square("a2")));
    assert(!is_piece_at(c, square("a3")));
    assert(!is_piece_at(c, square("a4")));

    do_select(c, "a2", side::lhs);
    move_cursor_to(c, "a4", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    for (int i{0}; i!=4; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("a2")));
    assert(is_piece_at(c, square("a4")));
    assert(has_just_double_moved(get_piece_at(c.get_game(), "a4"), c.get_game().get_in_game_time()));
    assert(is_enpassantable(get_piece_at(c.get_game(), "a4"), c.get_game().get_in_game_time()));
  }
  // a2-a5 does nothing
  {
    game_controller c;
    //game_controller c{create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::standard))};
    assert(is_piece_at(c, square("a2")));
    assert(!is_piece_at(c, square("a5")));
    assert(count_selected_units(c, chess_color::white) == 0);

    do_select(c, "a2", side::lhs);
    move_cursor_to(c, "a5", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    assert(is_piece_at(c, square("a2")));
    assert(!is_piece_at(c, square("a5")));
    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(is_piece_at(c, square("a2")));
    assert(!is_piece_at(c, square("a5")));
  }
  // En-passant capture
  {
    game_controller c{
      game(get_pieces_before_en_passant())
    };
    //game_controller c{create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::before_en_passant))};
    assert(is_piece_at(c, square("g2"))); // White pawn to be captured
    assert(is_piece_at(c, square("h4"))); // Black pawn to capture
    assert(!is_piece_at(c, square("g3"))); // White pawn can move forward
    assert(!is_piece_at(c, square("g4"))); // White pawn can move forward
    assert(count_selected_units(c, chess_color::white) == 0);

    // g2-g4
    do_select(c, "g2", side::lhs); // White pawn
    move_cursor_to(c, "g4", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    for (int i{0}; i!=5; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("g2")));
    assert(is_piece_at(c, square("g4")));
    assert(is_enpassantable(get_piece_at(c.get_game(), "g4"), c.get_game().get_in_game_time()));

    // h4xg3 e.p.
    do_select(c, "h4", side::rhs); // Black pawn
    move_cursor_to(c, "g3", side::rhs);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game();

    const auto messages{collect_messages(c.get_game())};
    assert(messages.back().get_message_type() == message_type::start_en_passant_attack);

    for (int i{0}; i!=4; ++i)
    {
      c.tick(delta_t(0.25));
    }
    c.tick(delta_t(0.25));
    assert(!is_piece_at(c, square("h4"))); // Black pawn has moved from here
    assert(!is_piece_at(c, square("g4"))); // White pawn is captured
    assert(is_piece_at(c, square("g3"))); // Black pawn has moved here
  }
  // castling kingside works when allowed
  {
    game_controller c{
      game(get_pieces_ready_to_castle()),
      lobby_options()
    };

    auto& white_king{get_piece_at(c.get_game(), "e1")};
    assert(white_king.get_messages().size() == 0);
    auto& white_rook{get_piece_at(c.get_game(), "h1")};
    assert(white_rook.get_messages().size() == 0);

    do_select(c, "e1", side::lhs);

    assert(count_selected_units(c, chess_color::white) == 1);

    move_cursor_to(c, "a4", side::lhs); // Square is irrelevant, but it should be empty

    assert(count_selected_units(c, chess_color::white) == 1);
    assert(white_king.get_messages().size() == 1); // Selected
    assert(white_rook.get_messages().size() == 0); // No need to be selected

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    assert(count_selected_units(c, chess_color::white) == 1);
    assert(white_king.get_messages().size() == 2); // Castling started
    assert(white_rook.get_messages().size() == 1); // Castling started

    assert(is_piece_at(c, square("e1")));  // K
    assert(!is_piece_at(c, square("f1"))); // Empty
    assert(!is_piece_at(c, square("g1"))); // Empty
    assert(is_piece_at(c, square("h1")));  // Rook
    // Should take 1 time unit
    for (int i{0}; i!=5; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("e1"))); // Empty
    assert(!is_piece_at(c, square("h1"))); // Empty
    assert(is_piece_at(c, square("g1")));  // K
    assert(is_piece_at(c, square("f1")));  // Rook
  }

  // cannot castle through check
  {
    game_controller c{
      game(get_pieces_ready_to_not_castle())
    };

    auto& white_king{get_piece_at(c.get_game(), "e1")};
    assert(white_king.get_messages().size() == 0);
    auto& white_rook{get_piece_at(c.get_game(), "h1")};
    assert(white_rook.get_messages().size() == 0);

    do_select(c, "e1", side::lhs);

    move_cursor_to(c, "a4", side::lhs); // Square is irrelevant, as long as it is empty

    assert(white_king.get_messages().size() == 1); // Selected
    assert(white_rook.get_messages().size() == 0); // No need to be selected
    const auto actions = get_piece_actions(c, side::lhs);
    assert(actions.empty());
  }
  // cannot castle when king is in check
  {
    game_controller c{
      game(create_pieces_from_fen_string(fen_string("4k3/4q3/8/8/8/8/8/R3K2R w KQ - 0 1")))
    };

    auto& white_king{get_piece_at(c.get_game(), "e1")};
    assert(white_king.get_messages().size() == 0);
    auto& white_rook{get_piece_at(c.get_game(), "h1")};
    assert(white_rook.get_messages().size() == 0);

    do_select(c, "e1", side::lhs);

    move_cursor_to(c, "a4", side::lhs); // Square is irrelevant, as long as it is empty

    assert(white_king.get_messages().size() == 1); // Selected
    assert(white_rook.get_messages().size() == 0); // No need to be selected
    const auto actions = get_piece_actions(c, side::lhs);
    assert(actions.empty());
  }
  // cannot castle when d and f are attacked
  {
    game_controller c{
      game(create_pieces_from_fen_string(fen_string("4k3/3r1r2/8/8/8/8/8/R3K2R w KQ - 0 1")))
    };

    auto& white_king{get_piece_at(c.get_game(), "e1")};
    assert(white_king.get_messages().size() == 0);
    auto& white_rook{get_piece_at(c.get_game(), "h1")};
    assert(white_rook.get_messages().size() == 0);

    do_select(c, "e1", side::lhs);

    move_cursor_to(c, "a4", side::lhs); // Square is irrelevant, as long as it is empty

    assert(white_king.get_messages().size() == 1); // Selected
    assert(white_rook.get_messages().size() == 0); // No need to be selected
    const auto actions = get_piece_actions(c, side::lhs);
    assert(actions.empty());
  }
  // castling queenside
  {
    game_controller c{
      game(get_pieces_ready_to_castle())
    };
    do_select(c, "e1", side::lhs);
    move_cursor_to(c, "a4", side::lhs); // Square is irrelevant
    assert(count_selected_units(c, chess_color::white) == 1);

    add_user_input(c, create_press_action_2(side::lhs)); // Action 1 is a king-side castle
    c.apply_user_inputs_to_game();

    assert(is_piece_at(c, square("a1")));  // Rook
    assert(!is_piece_at(c, square("b1"))); // Empty
    assert(!is_piece_at(c, square("c1"))); // Empty
    assert(!is_piece_at(c, square("d1"))); // Empty
    assert(is_piece_at(c, square("e1")));  // K
    // Should take 1 time unit
    for (int i{0}; i!=5; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("a1"))); // Empty
    assert(!is_piece_at(c, square("b1"))); // Empty
    assert(is_piece_at(c, square("c1")));  // K
    assert(is_piece_at(c, square("d1")));  // Rook
    assert(!is_piece_at(c, square("e1"))); // Empty
  }
  // A piece under attack must have decreasing health
  {
    game_controller c{
      game(get_pieces_bishop_and_knight_end_game()),
      lobby_options()
    };

    const double health_before{get_piece_at(c.get_game(), square("d2")).get_health()};
    // Let the white knight at c4 attack the black king at d2
    assert(get_piece_at(c.get_game(), square("d2")).get_color() == chess_color::black);

    do_select(c, "c4", side::lhs);
    move_cursor_to(c, "d2", side::lhs);

    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    assert(get_piece_at(c.get_game(), square("d2")).get_color() == chess_color::black);
    c.tick(delta_t(0.1));
    assert(get_piece_at(c.get_game(), square("d2")).get_color() == chess_color::black);
    const double health_after{get_piece_at(c.get_game(), square("d2")).get_health()};
    assert(health_after < health_before);
  }
  // Cannot attack a piece of one's own color
  {
    game_controller c;
    //game_controller c{create_game_controller_with_keyboard_mouse(create_game_with_starting_position(starting_position_type::standard))};
    const double health_before{get_piece_at(c.get_game(), square("e1")).get_health()};

    assert(get_piece_at(c.get_game(), square("d2")).get_action_history().get().empty());

    do_select(c, "d1", side::lhs);
    move_cursor_to(c, "e1", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    c.tick(delta_t(0.1));
    const double health_after{get_piece_at(c.get_game(), square("d2")).get_health()};
    assert(health_after == health_before);
    assert(get_piece_at(c.get_game(), square("d2")).get_action_history().get().empty());
  }

  // A queen cannot attack over pieces
  {
    game_controller c;
    assert(is_piece_at(c, square("d1")));

    do_select(c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game(); // Will do nothing

    assert(is_piece_at(c, square("d1")));
    for (int i{0}; i!=5; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(is_piece_at(c, square("d1")));
  }
  // Detect mate
  {
    game_controller c{
      game(get_pieces_before_scholars_mate()),
      lobby_options()
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
  }
  // get_possible_moves
  {
    // No moves when nothing selected
    {
      game_controller c;
      //game_controller c{create_game_controller_with_two_keyboards(create_game_with_standard_starting_position())};
      assert(get_possible_moves(c, side::lhs).empty());
      assert(get_possible_moves(c, side::rhs).empty());
    }
    // Knight at b1 has four moves when selected (two regular, and two moves beyond)
    {
      game_controller c;
      //game_controller c{create_game_controller_with_two_keyboards(create_game_with_standard_starting_position())};
      const std::vector<square> moves{get_possible_moves(c, side::lhs)};
      assert(moves.empty());
      auto& piece{get_piece_at(c, square("b1"))};
      assert(piece.get_type() == piece_type::knight);
      do_select(c, "b1", side::lhs);
      assert(get_possible_moves(c, side::lhs).size() == 4);
    }
    // Pawn at e2 has four moves when selected
    {
      game_controller c;
      const std::vector<square> moves{get_possible_moves(c, side::lhs)};
      assert(moves.empty());
      do_select(c, "e2", side::lhs);
      assert(get_possible_moves(c, side::lhs).size() == 4);
    }
  }
  // operator<<
  {
    std::stringstream s;
    game_controller c;
    //game_controller c{create_game_controller_with_keyboard_mouse()};
    s << c;
    assert(!s.str().empty());
  }
  // to_pgn
  {
    game_controller c;
    //game_controller c{create_game_controller_with_keyboard_mouse(create_game_with_standard_starting_position())};
    assert(to_pgn(c.get_game()).empty());
    const piece_action e2e4(
      chess_color::white,
      piece_type::pawn,
      piece_action_type::move,
      square("e2"),
      square("e4")
    );

    do_select(c, "e2", side::lhs);
    move_cursor_to(c, "e4", side::lhs);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    tick_until_idle(c.get_game());
    const std::string pgn{to_pgn(c.get_game())};
    assert(!pgn.empty());
    assert(pgn == "0.00: white pawn selected at e2\n0.00: white pawn moves from e2 to e4");
  }
  // Moving a mouse cursor over the board does nothing
  {
    game_controller c;
    //game_controller c{create_game_controller_with_mouse_keyboard(create_game_with_starting_position(starting_position_type::standard))};
    c.add_user_input(create_mouse_move_action(game_coordinate(4.5, 4.5), side::lhs));
    c.apply_user_inputs_to_game();
    assert(count_selected_units(c, chess_color::white) == 0);
    assert(count_selected_units(c, chess_color::black) == 0);
  }
  // Moving a mouse cursor outside the board does no harm
  {
    game_controller c;
    //game_controller c{create_game_controller_with_mouse_keyboard(create_game_with_starting_position(starting_position_type::standard))};
    c.add_user_input(create_mouse_move_action(game_coordinate(94.5, -54.5), side::lhs));
    c.apply_user_inputs_to_game();
    assert(count_selected_units(c, chess_color::white) == 0);
    assert(count_selected_units(c, chess_color::black) == 0);
  }
  //----------------------------------------------------------------------------
  // Things that cannot be done during a move
  //----------------------------------------------------------------------------
  // A selected piece that is moving, has no actions
  {
    game_controller c;

    // Start e2 e4
    do_select(c, "e2", side::lhs);
    move_cursor_to(c, "e4", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(get_piece_actions(c, side::lhs).size() == 1); // Move
    assert(get_piece_actions(c, side::lhs)[0] == piece_action_type::move);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.25));

    // The piece remained selected
    assert(count_selected_units(c, chess_color::white) == 1);

    // Cannot stop the movement
    assert(get_piece_actions(c, side::lhs).size() == 0);
  }
  // A selected piece that is moving, cannot attack an enemy
  {
    game_controller c(
      create_game_with_starting_position(starting_position_type::queen_end_game)
    );

    // Start Qd1 d6
    do_select(c, "d1", side::lhs);
    move_cursor_to(c, "d6", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(get_piece_actions(c, side::lhs).size() == 1); // Move
    assert(get_piece_actions(c, side::lhs)[0] == piece_action_type::move);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.25));

    assert(count_selected_units(c, chess_color::white) == 1);
    assert(get_piece_actions(c, side::lhs).size() == 0);

    move_cursor_to(c, "d8", side::lhs);
    assert(get_piece_actions(c, side::lhs).size() == 0);
  }
  // A selected piece that is attacking, has no actions
  {
    game_controller c(
      create_game_with_starting_position(starting_position_type::queen_end_game)
    );

    // Start Qd1xd8
    do_select(c, "d1", side::lhs);
    move_cursor_to(c, "d8", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(get_piece_actions(c, side::lhs).size() == 1); // Attack
    assert(get_piece_actions(c, side::lhs)[0] == piece_action_type::attack);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.25));

    // The piece remained selected
    assert(count_selected_units(c, chess_color::white) == 1);

    // Cannot stop the attack
    assert(get_piece_actions(c, side::lhs).size() == 0);
  }
  // A selected piece doing an en-passant capture, cannot attack anymore
  {
    game_controller c{
      game(get_pieces_before_en_passant())
    };
    assert(is_piece_at(c, square("g2"))); // White pawn to be captured
    assert(is_piece_at(c, square("h4"))); // Black pawn to capture
    assert(!is_piece_at(c, square("g3"))); // White pawn can move forward
    assert(!is_piece_at(c, square("g4"))); // White pawn can move forward
    assert(count_selected_units(c, chess_color::white) == 0);

    // g2-g4
    do_select(c, "g2", side::lhs); // White pawn
    move_cursor_to(c, "g4", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    for (int i{0}; i!=5; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(!is_piece_at(c, square("g2")));
    assert(is_piece_at(c, square("g4")));
    assert(is_enpassantable(get_piece_at(c.get_game(), "g4"), c.get_game().get_in_game_time()));

    // h4xg3 e.p.
    do_select(c, "h4", side::rhs); // Black pawn
    move_cursor_to(c, "g3", side::rhs);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game();

    const auto messages{collect_messages(c.get_game())};
    assert(messages.back().get_message_type() == message_type::start_en_passant_attack);

    const auto actions = get_piece_actions(c, side::lhs);
    // Cannot attack anymore, but can unselect
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::unselect);
  }

  // a selected white king cannot castle when castling kingside
  {
    game_controller c{
      game(get_pieces_ready_to_castle())
    };
    do_select(c, "e1", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(collect_messages(c.get_game()).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "g1", side::lhs); // Cursor must not be on king
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.01));
    const auto msg{collect_messages(c.get_game())};
    assert(msg.at(1).get_message_type() == message_type::start_castling_kingside);

    move_cursor_to(c, "e1", side::lhs); // Cursor must be on king again

    const auto actions = get_piece_actions(c, side::lhs);
    // Cannot castle anymore, but can unselect
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::unselect);
  }
  // a selected white king cannot castle when castling queenside
  {
    game_controller c{
      game(get_pieces_ready_to_castle())
    };
    do_select(c, "e1", side::lhs);

    c.tick(delta_t(0.0));
    assert(count_selected_units(c, chess_color::white) == 1);
    assert(collect_messages(c.get_game()).at(0).get_message_type() == message_type::select);
    move_cursor_to(c, "g1", side::lhs); // Cursor must not be on king
    assert(count_selected_units(c, chess_color::white) == 1);

    add_user_input(c, create_press_action_2(side::lhs));
    c.apply_user_inputs_to_game();
    c.tick(delta_t(0.01));

    const auto msg{collect_messages(c.get_game())};
    assert(msg.at(0).get_message_type() == message_type::start_castling_queenside);
    assert(msg.at(2).get_message_type() == message_type::start_castling_queenside);

    move_cursor_to(c, "e1", side::lhs); // Cursor must be on king again

    const auto actions = get_piece_actions(c, side::lhs);
    // Cannot castle anymore, but can unselect
    assert(actions.size() == 1);
    assert(actions[0] == piece_action_type::unselect);
  }
  //----------------------------------------------------------------------------
  // Moves that do not complete
  //----------------------------------------------------------------------------
  // When two pieces of the same color want to move to the same square, one will go back
  {
    game_controller c;

    // Start c2-c3
    do_select(c, "c2", side::lhs);
    move_cursor_to(c, "c3", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    // Get that pawn moving
    c.tick(delta_t(0.25));

    // Start Nb1-c3
    do_select(c, "b1", side::lhs);
    move_cursor_to(c, "c3", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(is_piece_at(c, square("c3")));
    assert(is_piece_at(c, square("b1")));
    assert(!is_piece_at(c, square("c2"))); // The pawn got there first

    const auto knight_messages{get_piece_at(c, "b1").get_messages()};
    assert(knight_messages.size() == 5);
    assert(knight_messages[0] == message_type::select);
    assert(knight_messages[1] == message_type::start_move);
    assert(knight_messages[2] == message_type::start_move); // ?Why twice
    assert(knight_messages[3] == message_type::cannot);
    assert(knight_messages[4] == message_type::done);
  }
  // When two pieces of the same color want to move to the same square, one will go back
  {
    game_controller c(
      create_game_with_starting_position(starting_position_type::queen_end_game)
    );
    c.tick(delta_t(1.0)); // Black can move after 1 chess move

    // Start white Qd1-d5
    do_select(c, "d1", side::lhs);
    move_cursor_to(c, "d5", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    // Get that queen moving
    c.tick(delta_t(0.25));

    // Start black Qd8-d5
    do_select(c, "d8", side::rhs);
    move_cursor_to(c, "d5", side::rhs);
    assert(count_selected_units(c, chess_color::black) == 1);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game();

    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(is_piece_at(c, square("d5")));
    assert(is_piece_at(c, square("d8")));
    assert(!is_piece_at(c, square("d1"))); // The white queen got there first

    const auto white_queen_messages{get_piece_at(c, "d5").get_messages()};
    assert(white_queen_messages.size() == 3);
    assert(white_queen_messages[0] == message_type::select);
    assert(white_queen_messages[1] == message_type::start_move);
    assert(white_queen_messages[2] == message_type::done);

    const auto black_queen_messages{get_piece_at(c, "d8").get_messages()};
    assert(black_queen_messages.size() == 5);
    assert(black_queen_messages[0] == message_type::select);
    assert(black_queen_messages[1] == message_type::start_move);
    assert(black_queen_messages[2] == message_type::start_move);
    assert(black_queen_messages[3] == message_type::cannot);
    assert(black_queen_messages[4] == message_type::done);
  }
  // When a piece is attacking a piece that is fleeing successfully,
  // there is no capture
  {
    game_controller c(
      create_game_with_starting_position(starting_position_type::kasparov_vs_topalov)
    );
    c.tick(delta_t(1.0)); // Black can move after 1 chess move

    // Start white g6-g5
    do_select(c, "g6", side::rhs);
    move_cursor_to(c, "g5", side::rhs);
    assert(count_selected_units(c, chess_color::black) == 1);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game();

    // Get that black pawn moving
    c.tick(delta_t(0.25));

    // White starts attack Qf6-g6
    do_select(c, "f6", side::lhs);
    move_cursor_to(c, "g6", side::lhs);
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs));
    c.apply_user_inputs_to_game();

    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      c.tick(delta_t(0.25));
    }
    assert(is_piece_at(c, square("g5")));
    assert(is_piece_at(c, square("f6")));
    assert(!is_piece_at(c, square("g6"))); // The pawn fled

    const auto black_pawn_messages{get_piece_at(c, "g5").get_messages()};
    assert(black_pawn_messages.size() == 3);
    assert(black_pawn_messages[0] == message_type::select);
    assert(black_pawn_messages[1] == message_type::start_move);
    assert(black_pawn_messages[2] == message_type::done);

    const auto white_queen{get_piece_at(c, "f6")};
    const auto white_queen_messages{white_queen.get_messages()};
    assert(white_queen_messages.size() == 3);
    assert(white_queen_messages[0] == message_type::select);
    assert(white_queen_messages[1] == message_type::start_attack);
    assert(white_queen_messages[2] == message_type::cannot);
  }
  //#define FIX_INTERRUPT_WHITE_KINGSIDE_CASTLING_BY_CAPTURING_WHITE_ROOK
  #ifdef FIX_INTERRUPT_WHITE_KINGSIDE_CASTLING_BY_CAPTURING_WHITE_ROOK
  // Interrupt white king-side castling, by capturing the white rook
  {
    game_controller c(
      create_game_with_starting_position(starting_position_type::ready_to_castle)
    );

    // Black attacks king-side rook
    do_select(c, "h8", side::rhs);
    move_cursor_to(c, "h1", side::rhs);
    assert(count_selected_units(c, chess_color::black) == 1);
    add_user_input(c, create_press_action_1(side::rhs));
    c.apply_user_inputs_to_game();

    // Get that black pawn attacking
    c.tick(delta_t(0.75));

    // White starts king-side castle
    do_select(c, "e1", side::lhs);
    move_cursor_to(c, "e4", side::lhs); // So cursor is on empty square
    assert(count_selected_units(c, chess_color::white) == 1);
    add_user_input(c, create_press_action_1(side::lhs)); // Start king-side castle
    c.apply_user_inputs_to_game();

    // Should take 1 time unit
    for (int i{0}; i!=4; ++i)
    {
      c.tick(delta_t(0.25));
    }

    std::clog << to_board_str(c) << '\n';

    assert(is_piece_at(c, square("h1")));
    assert(is_piece_at(c, square("e1")));
    assert(!is_piece_at(c, square("h8"))); // The black rook captured the white one

    const auto black_rook_messages{get_piece_at(c, "h1").get_messages()};
    assert(black_rook_messages.size() == 3);
    assert(black_rook_messages[0] == message_type::select);
    assert(black_rook_messages[1] == message_type::start_attack);
    assert(black_rook_messages[2] == message_type::done);

    const auto white_king{get_piece_at(c, "e1")};
    const auto white_king_messages{white_king.get_messages()};
    assert(white_king_messages.size() == 3);
    assert(white_king_messages[0] == message_type::select);
    assert(white_king_messages[1] == message_type::start_castling_kingside);
    assert(white_king_messages[2] == message_type::cannot);
    assert(!"Yay");
  }
  #endif // FIX_INTERRUPT_WHITE_KINGSIDE_CASTLING_BY_CAPTURING_WHITE_ROOK
  #endif // NDEBUG // no tests in release
}

void game_controller::tick(const delta_t& dt)
{
  check_selected_pieces_exist();
  m_game.tick(dt);

  update_selectedness_due_to_captures();
  check_selected_pieces_exist();
}

std::string to_board_str(
  const game_controller& c,
  const board_to_text_options& options
) noexcept
{
  return to_board_str(c.get_game(), options);
}

void game_controller::update_selectedness_due_to_captures()
{
  for (const side s: get_all_sides())
  {
    if (!m_selected_piece_id[s].has_value()) continue;
    // A piece that died
    if (!has_piece_with_id(m_game, m_selected_piece_id[s].value()))
    {
      m_selected_piece_id[s] = {};
    }
  }
  check_selected_pieces_exist();
}

std::ostream& operator<<(std::ostream& os, const game_controller& g) noexcept
{

  os
    << "LHS cursor position: " << g.get_cursor_pos(side::lhs) << '\n'
    << "RHS cursor position: " << g.get_cursor_pos(side::rhs) << '\n'
    //<< "LHS player physical controller: " << get_physical_controller(g, side::lhs) << '\n'
    //<< "RHS player physical controller: " << get_physical_controller(g, side::rhs) << '\n'
    << "User inputs: " << g.get_user_inputs()
  ;
  return os;
}
