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
        piece.get_coordinat() + game_coordinat(0.5, -0.5) // Center on piece
      );
    }
  );
  assert(distances.size() == pieces.size());
  return distances;
}

void game::do_lmb_down(const game_coordinat& coordinat)
{
  if (get_selected_pieces(*this).empty()) {
    if (is_piece_at(*this, coordinat)) {
      select(get_closest_piece_to(coordinat));
    }
  } else {
    // There is a piece there? Select it
    if (is_piece_at(*this, coordinat)) {
      toggle_select(get_closest_piece_to(coordinat));
    } else {
      // Move selected pieces there
      for (auto& p: m_pieces)
      {
        if (p.is_selected()) p.add_action(
          piece_action(piece_action_type::move, coordinat)
        );
      }
    }
  }

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
  }
  m_actions = std::vector<action>();

  for (auto& p: m_pieces) p.tick();

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
#endif // no tests in release
}


