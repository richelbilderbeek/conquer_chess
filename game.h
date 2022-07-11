#ifndef GAME_H
#define GAME_H

#include "action.h"
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

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

  /// Go to the next frame
  void tick();

private:

  std::vector<action> m_actions;
  std::vector<piece> m_pieces;
};

void test_game();

#endif // GAME_H
