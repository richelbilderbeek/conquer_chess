#include "game.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>

game::game(
  const screen_coordinat& screen_size,
  const int margin_width
)
  : m_layout{screen_size, margin_width},
    m_pieces{get_starting_pieces()}
{

}

void game::add_action(const action a)
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

int count_selected_units(const game& g)
{
  return std::count_if(
    std::begin(g.get_pieces()),
    std::end(g.get_pieces()),
    [](const auto& piece) { return piece.is_selected(); }
  );
}

void game::do_lmb_down(const game_coordinat& coordinat)
{
  // #|Has selected pieces? |Clicking on what?|Do what?
  // -|---------------------|-----------------|----------------
  // 1|Yes                  |Selected unit    |Unselect unit
  // 2|Yes                  |Unselected unit  |Select unit
  // 3|Yes                  |Empty square     |Unselect all units
  // 4|No                   |Selected unit    |NA
  // 5|No                   |Unselected unit  |Select unit
  // 6|No                   |Empty square     |Nothing

  if (has_selected_pieces(*this))
  {
    if (is_piece_at(*this, coordinat)) {
      auto& piece{get_closest_piece_to(coordinat)};
      if (piece.get_color() == chess_color::white)
      {
        if (piece.is_selected())
        {
          unselect(piece); // 1
        }
        else
        {
          unselect_all_pieces(*this);
          select(piece); // 2
        }
      }
    }
    else
    {
      unselect_all_pieces(*this); // 3
    }
  }
  else
  {
    if (is_piece_at(*this, coordinat)) {
      auto& piece{get_closest_piece_to(coordinat)};
      if (piece.get_color() == chess_color::white)
      {
        if (piece.is_selected())
        {
          assert(!"Should never happen, as there are no selected pieces at all");
          unselect(piece); // 4
        }
        else
        {
          select(piece); // 5
        }
      }
    }
    else
    {
      // 6
    }
  }
}

void game::do_rmb_down(const game_coordinat& coordinat)
{
  for (auto& p: m_pieces)
  {
    if (p.is_selected())
    {
      p.add_action(
        piece_action(
          piece_action_type::move,
          center_on_center(coordinat)
        )
      );
    }
  }
  unselect_all_pieces(*this);
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

game get_default_game() noexcept
{
  return game{
    get_default_screen_size(),
    get_default_margin_width()
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

std::vector<piece> get_selected_pieces(const game& g)
{
  std::vector<piece> pieces;
  const auto& all_pieces = g.get_pieces();
  std::copy_if(
    std::begin(all_pieces),
    std::end(all_pieces),
    std::back_inserter(pieces),
    [](const auto& piece) { return piece.is_selected(); }
  );
  return pieces;
}

const std::vector<piece>& get_pieces(const game& g) noexcept
{
  return g.get_pieces();
}

bool has_selected_pieces(const game& g)
{
  return !get_selected_pieces(g).empty();
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
    if (action.get_type() == action_type::mouse_move)
    {
      m_mouse_pos = action.get_coordinat();
    }
    else if (action.get_type() == action_type::lmb_down)
    {
      do_lmb_down(action.get_coordinat());
    }
    else if (action.get_type() == action_type::rmb_down)
    {
      do_rmb_down(action.get_coordinat());
    }
  }
  m_actions = std::vector<action>();

  for (auto& p: m_pieces) p.tick();

}

void unselect_all_pieces(game & g)
{
  for (auto& piece: g.get_pieces())
  {
    unselect(piece);
  }
}

void test_game() //!OCLINT tests may be many
{
#ifndef NDEBUG // no tests in release
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
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    assert(count_selected_units(g) == 0);
    g.add_action(create_press_lmb_action(white_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g) == 1);
  }
  // Clicking a unit twice with LMB selects and unselects it
  {
    game g;
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    assert(count_selected_units(g) == 0);
    g.add_action(create_press_lmb_action(white_king.get_coordinat()));
    g.tick();
    g.add_action(create_press_lmb_action(white_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g) == 0);
  }
  // Clicking a unit with LMB, then another unit with LMB, only the last unit is selected
  {
    game g;
    const auto white_king{find_pieces(g, piece_type::king, chess_color::white).at(0)};
    const auto white_queen{find_pieces(g, piece_type::queen, chess_color::white).at(0)};
    assert(count_selected_units(g) == 0);
    g.add_action(create_press_lmb_action(white_king.get_coordinat()));
    g.tick();
    assert(count_selected_units(g) == 1);
    g.add_action(create_press_lmb_action(white_queen.get_coordinat()));
    g.tick();
    assert(count_selected_units(g) == 1);
  }
#endif // no tests in release
}


