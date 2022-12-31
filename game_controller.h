#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "game_coordinat.h"
#include "side.h"

/// The class that acts as a controller for \link{game}.
class game_controller
{
public:
  game_controller();

  /// Get the player position
  const game_coordinat& get_player_pos(const side player) const noexcept;

  /// Get the player position
  game_coordinat& get_player_pos(const side player) noexcept;

private:
  /// The in-game coordinat of the LHS user's cursor
  game_coordinat m_player_lhs_pos;

  /// The in-game coordinat of the RHS user's cursor
  game_coordinat m_player_rhs_pos;
};

#endif // GAME_CONTROLLER_H
