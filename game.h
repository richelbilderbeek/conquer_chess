#ifndef GAME_H
#define GAME_H

#include "action.h"
#include "game_coordinat.h"
#include "game_view_layout.h"
#include "piece.h"
#include <vector>

/// Contains the game logic.
/// All data types used by this class are STL and/or Boost
class game
{
public:
  game(const screen_coordinat& screen_size);

  /// Add an action. These will be processed in 'tick'
  void add_action(const action a);

  /// Get the layout of the screen
  const auto& get_layout() const noexcept { return m_layout; }

  /// Get the in-game mouse position
  const auto& get_mouse_pos() const noexcept { return m_mouse_pos; }

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

  /// Go to the next frame
  void tick();

private:

  std::vector<action> m_actions;

  /// The layout of the screen, e.g. the top-left of the sidebar
  game_view_layout m_layout;

  /// The in-game coordinat of the mouse
  game_coordinat m_mouse_pos;

  std::vector<piece> m_pieces;


  /// Process a left-mouse-button down
  void do_lmb_down(const game_coordinat& coordinat);
};

/// Calculate the distances that each piece has to a coordinat
std::vector<double> calc_distances(
  const std::vector<piece>& pieces,
  const game_coordinat& coordinat
);

void test_game();

#endif // GAME_H
