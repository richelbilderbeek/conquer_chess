#ifndef GAME_H
#define GAME_H

#include "action.h"
#include "game_coordinat.h"
#include "piece.h"
#include <vector>

/// Contains the game logic.
/// All data types used by this class are STL and/or Boost
class game
{
public:
  game();

  /// Add an action. These will be processed in 'tick'
  void add_action(const action a);

  /// Get the in-game mouse position
  const auto& get_mouse_pos() const noexcept { return m_mouse_pos; }

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

  /// Go to the next frame
  void tick();

private:

  std::vector<action> m_actions;

  /// The in-game coordinat of the mouse
  game_coordinat m_mouse_pos;

  std::vector<piece> m_pieces;
};

void test_game();

#endif // GAME_H
