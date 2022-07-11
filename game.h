#ifndef GAME_H
#define GAME_H

#include "piece.h"
#include <vector>

/// Contains the game logic.
/// All data types used by this class are STL and/or Boost
class game
{
public:
  game();

  /// Get all the pieces
  const auto& get_pieces() const noexcept { return m_pieces; }

  /// Go to the next frame
  void tick();

private:

  std::vector<piece> m_pieces;
};

void test_game();

#endif // GAME_H
