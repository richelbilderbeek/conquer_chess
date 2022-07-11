#ifndef GAME_H
#define GAME_H

/// Contains the game logic.
/// All data types used by this class are STL and/or Boost
class game
{
public:
  game();

  /// Go to the next frame
  void tick();
};

void test_game();

#endif // GAME_H
