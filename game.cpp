#include "game.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>

game::game(
  const game_options& options
)
  : m_layout{options.get_screen_size(), options.get_margin_width()},
    m_player_1_pos{0.5, 4.5},
    m_player_2_pos{7.5, 4.5},
    m_options{options},
    m_pieces{options.get_starting_pieces()}
{

}

void game::add_action(const control_action a)
{
  // These will be processed in 'tick'
  m_actions.push_back(a);
}

std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
) {
  std::vector<double> distances;
  distances.reserve(pieces.size());
  std::transform(
    std::begin(pieces),
    std::end(pieces),
    std::back_inserter(distances),
    [coordinat](const auto& piece) {
      return calc_distance(
        coordinat,
        piece.get_coordinat()
      );
    }
  );
  assert(distances.size() == pieces.size());
  return distances;
}

bool can_player_select_piece_at_cursor_pos(
  const game& g,
  const chess_color player
)
{
  const auto& cursor_pos{get_cursor_pos(g, player)};
  if (
    !is_piece_at(
      g,
      cursor_pos,
      g.get_options().get_click_distance()
    )
  )
  {
    return false;
  }
  const auto& piece{g.get_closest_piece_to(cursor_pos)};
  return !piece.is_selected();
}

void game::clear_sound_effects() noexcept
{
  m_sound_effects.clear();
  assert(m_sound_effects.empty());
}

int count_game_actions(const game& g)
{
  return static_cast<int>(g.get_actions().size());
}

int count_piece_actions(
  const game& g,
  const chess_color player
)
{
  const auto& pieces{g.get_pieces()};
  return std::accumulate(
    std::begin(pieces),
    std::end(pieces),
    0,
    [player](const int n, const auto& piece)
    {
      if (piece.get_color() == player)
      {
        return n + count_piece_actions(piece);
      }
      return n;
    }
  );
}

int count_selected_units(
  const game& g,
  const chess_color player
)
{
  return std::count_if(
    std::begin(g.get_pieces()),
    std::end(g.get_pieces()),
    [player](const auto& piece)
    {
      return piece.is_selected()
        && piece.get_color() == player;
    }
  );
}

game create_king_versus_king_game()
{
  const game_options options(
    get_default_screen_size(),
    get_king_versus_king_starting_pieces(),
    get_default_delta_t(),
    get_default_margin_width()
  );
  return game(options);
}

void game::do_select(
  const game_coordinat& coordinat,
  const chess_color player_color
)
{
  // #|Has selected pieces? |Clicking on what?|Do what?
  // -|---------------------|-----------------|----------------
  // 1|Yes                  |Selected unit    |Unselect unit
  // 2|Yes                  |Unselected unit  |Select unit
  // 3|Yes                  |Empty square     |Unselect all units
  // 4|No                   |Selected unit    |NA
  // 5|No                   |Unselected unit  |Select unit
  // 6|No                   |Empty square     |Nothing
  if (has_selected_pieces(*this, player_color))
  {
    if (is_piece_at(*this, coordinat)) {
      auto& piece{get_closest_piece_to(coordinat)};
      if (piece.get_color() == player_color)
      {
        if (piece.is_selected())
        {
          unselect(piece); // 1
        }
        else
        {
          unselect_all_pieces(*this, player_color);
          select(piece); // 2
          m_sound_effects.push_back(
            sound_effect(
              sound_effect_type::select,
              piece.get_color(),
              piece.get_type()
            )
          );
        }
      }
    }
    else
    {
      unselect_all_pieces(*this, player_color); // 3
    }
  }
  else
  {
    if (is_piece_at(*this, coordinat)) {
      auto& piece{get_closest_piece_to(coordinat)};
      if (piece.get_color() == player_color)
      {
        if (piece.is_selected())
        {
          assert(!"Should never happen, as there are no selected pieces at all");
          unselect(piece); // 4
        }
        else
        {
          select(piece); // 5
          m_sound_effects.push_back(
            sound_effect(
              sound_effect_type::select,
              piece.get_color(),
              piece.get_type()
            )
          );
        }
      }
    }
    else
    {
      // 6
    }
  }
}

bool do_show_selected(const game& g) noexcept
{
  return do_show_selected(g.get_options());
}

void game::start_move_unit(
  const game_coordinat& coordinat,
  const chess_color player_color
)
{
  for (auto& p: m_pieces)
  {
    if (p.is_selected() && p.get_color() == player_color)
    {
      // No shift, so all current actions are void
      clear_actions(p);

      p.add_action(
        piece_action(
          piece_action_type::move,
          center_on_center(coordinat)
        )
      );

      m_sound_effects.push_back(
        sound_effect(
          sound_effect_type::start_move,
          p.get_color(),
          p.get_type()
        )
      );
    }
  }
  unselect_all_pieces(*this, player_color);
}

std::vector<piece> find_pieces(
  const game& g,
  const piece_type type,
  const chess_color color
)
{
  std::vector<piece> pieces;
  std::copy_if(
    std::begin(g.get_pieces()),
    std::end(g.get_pieces()),
    std::back_inserter(pieces),
    [type, color](const auto& piece)
    {
      return piece.get_color() == color
        && piece.get_type() == type;
    }
  );
  return pieces;
}

const piece& game::get_closest_piece_to(const game_coordinat& coordinat) const
{
  return m_pieces[get_index_of_closest_piece_to(*this, coordinat)];
}

piece& game::get_closest_piece_to(const game_coordinat& coordinat)
{
  return m_pieces[get_index_of_closest_piece_to(*this, coordinat)];
}

game_coordinat get_cursor_pos(
  const game& g,
  const chess_color c
)
{
  if (c == g.get_options().get_keyboard_user_player_color())
  {
    return g.get_player_1_pos();
  }
  assert(c == g.get_options().get_mouse_user_player_color());
  return g.get_player_2_pos();
}

game get_default_game() noexcept
{
  return game{
    get_default_game_options()
  };
}

int get_index_of_closest_piece_to(
  const game& g,
  const game_coordinat& coordinat
)
{
  const std::vector<double> distances{
    calc_distances(g.get_pieces(), coordinat)
  };
  const auto iter = std::min_element(std::begin(distances), std::end(distances));
  const auto index = std::distance(std::begin(distances), iter);
  return index;
}

std::vector<piece> get_selected_pieces(
  const game& g,
  const chess_color player
)
{
  std::vector<piece> pieces;
  const auto& all_pieces = g.get_pieces();
  std::copy_if(
    std::begin(all_pieces),
    std::end(all_pieces),
    std::back_inserter(pieces),
    [player](const auto& piece)
    {
      return piece.is_selected() && piece.get_color() == player;
    }
  );
  return pieces;
}

const std::vector<piece>& get_pieces(const game& g) noexcept
{
  return g.get_pieces();
}

bool has_selected_pieces(const game& g, const chess_color player)
{
  return !get_selected_pieces(g, player).empty();
}

bool is_piece_at(
  const game& g,
  const game_coordinat& coordinat,
  const double distance
) {
  const std::vector<double> distances{
    calc_distances(g.get_pieces(), coordinat)
  };
  const auto iter = std::find_if(
    std::begin(distances),
    std::end(distances),
    [distance](const double this_distance) { return this_distance < distance; }
  );
  return iter != std::end(distances);
}

void game::tick()
{
  for (const auto& action: m_actions)
  {
    if (action.get_type() == control_action_type::press_attack)
    {
      // TODO
    }
    else if (action.get_type() == control_action_type::press_down)
    {
      m_player_1_pos += game_coordinat(0.0, 1.0);
      if (m_player_1_pos.get_y() > 8.0)
      {
        m_player_1_pos += game_coordinat(0.0, -8.0);
      }
    }
    else if (action.get_type() == control_action_type::press_left)
    {
      m_player_1_pos += game_coordinat(-1.0, 0);
      if (m_player_1_pos.get_x() < 0.0)
      {
        m_player_1_pos += game_coordinat(8.0, 0.0);
      }
    }
    else if (action.get_type() == control_action_type::press_move)
    {
      start_move_unit(
        m_player_1_pos,
        m_options.get_keyboard_user_player_color()
      );
    }
    else if (action.get_type() == control_action_type::press_right)
    {
      m_player_1_pos += game_coordinat(1.0, 0);
      if (m_player_1_pos.get_x() > 8.0)
      {
        m_player_1_pos += game_coordinat(-8.0, 0.0);
      }
    }
    else if (action.get_type() == control_action_type::press_select)
    {
      do_select(m_player_1_pos, chess_color::white);
    }
    else if (action.get_type() == control_action_type::press_up)
    {
      m_player_1_pos += game_coordinat(0.0, -1.0);
      if (m_player_1_pos.get_y() < 0.0)
      {
        m_player_1_pos += game_coordinat(0.0, 8.0);
      }
    }
    else if (action.get_type() == control_action_type::mouse_move)
    {
      m_player_2_pos = action.get_coordinat();
    }
    else if (action.get_type() == control_action_type::lmb_down)
    {
      do_select(action.get_coordinat(), chess_color::black);
    }
    else if (action.get_type() == control_action_type::rmb_down)
    {
      start_move_unit(
        action.get_coordinat(),
        m_options.get_mouse_user_player_color()
      );
    }
  }
  m_actions = std::vector<control_action>();

  for (auto& p: m_pieces) p.tick(m_options.get_delta_t());

}

void unselect_all_pieces(
  game & g,
  const chess_color color
)
{
  for (auto& piece: g.get_pieces())
  {
    if (piece.get_color() == color) unselect(piece);
  }
}

void resize_window(
  game& g,
  const screen_coordinat& window_size,
  const int margin_width
)
{
  resize(g.get_layout(), window_size, margin_width);
}


void test_game() //!OCLINT tests may be many
{
#ifndef NDEBUG // no tests in release
  // Actions in pieces accumulate
  {
    game g = create_king_versus_king_game();
    g.get_pieces().at(0).add_action(
      piece_action(
        piece_action_type::move,
        game_coordinat(1.0, 1.0)
      )
    );
    assert(count_piece_actions(g, chess_color::white) == 1);
    g.get_pieces().at(0).add_action(
      piece_action(
        piece_action_type::move,
        game_coordinat(2.0, 2.0)
      )
    );
    assert(count_piece_actions(g, chess_color::white) == 2);
  }
  {
    game g;
    assert(!is_piece_at(g, game_coordinat(3, 3)));
    for (const auto& piece: g.get_pieces())
    {
      assert(is_piece_at(g, piece.get_coordinat(), 1.0));
    }
  }
  // Clicking a unit once with LMB selects it
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // Clicking a unit twice with LMB selects and unselects it
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 0);
  }
  // Clicking a unit with LMB, then another unit with LMB, only the last unit is selected
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    const auto black_queen{find_pieces(g, piece_type::queen, chess_color::black).at(0)};
    assert(count_selected_units(g, chess_color::black) == 0);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
    g.add_action(create_press_lmb_action(black_queen.get_coordinat()));
    g.tick();
    assert(count_selected_units(g, chess_color::black) == 1);
  }
  // LMB then RMB makes a unit move
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 0);
    g.add_action(create_press_rmb_action(black_king.get_coordinat() + game_coordinat(1.0, 1.0)));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 1);
  }
  // 2x LMB then RMB makes a unit move 1 stretch (not 2)
  {
    game g;
    const auto black_king{find_pieces(g, piece_type::king, chess_color::black).at(0)};
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.add_action(create_press_rmb_action(black_king.get_coordinat() + game_coordinat(1.0, 1.0)));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 1);
    g.add_action(create_press_lmb_action(black_king.get_coordinat()));
    g.add_action(create_press_rmb_action(black_king.get_coordinat() + game_coordinat(2.0, 2.0)));
    g.tick();
    assert(count_piece_actions(g, chess_color::black) == 1);
  }
#endif // no tests in release
}

void toggle_player(game& g)
{
  toggle_player(g.get_options());
}
