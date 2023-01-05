#ifndef LOBBY_OPTIONS_H
#define LOBBY_OPTIONS_H

#include "chess_color.h"
#include "race.h"
#include "side.h"

#include <iosfwd>

/// The options that are set in the lobby
class lobby_options
{
public:
  lobby_options();

  /// Get the chess color of a player
  chess_color get_color(const side player_side) const noexcept;

  /// Get the chess color of a player
  race get_race(const side player_side) const noexcept;

  /// Get the chess color of a player
  void set_color(const chess_color color, const side player_side) noexcept;

  /// Get the chess color of a player
  void set_race(const race r, const side player_side) noexcept;

private:

  /// The selected color for the LHS.
  /// RHS has the other color
  chess_color m_lhs_color;

  /// The selected race
  race m_lhs_race;
  race m_rhs_race;
};

chess_color get_color(const lobby_options& options, const side player_side) noexcept;

/// Test this class and its free functions
void test_lobby_options();

std::ostream& operator<<(std::ostream& os, const lobby_options& options) noexcept;

#endif // LOBBY_OPTIONS_H
