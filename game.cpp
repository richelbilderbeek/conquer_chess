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
  // Zero or one pieces to work on
  const std::vector<double> distances{
    calc_distances(m_pieces, coordinat)
  };
  const auto iter = std::min_element(std::begin(distances), std::end(distances));
  const auto index = std::distance(std::begin(distances), iter);
  piece& p = m_pieces[index];
  const auto lowest_distance = distances[index];
  if (lowest_distance > 0.5) return;
  p.do_lmb_down();
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
}

void test_game() //!OCLINT tests may be many
{
#ifndef NDEBUG // no tests in release
#endif // no tests in release
}


