#ifndef STARTING_POSITION_TYPE_H
#define STARTING_POSITION_TYPE_H

#include <string>
#include <vector>

/// The starting position type
enum class starting_position_type
{
  before_scholars_mate,
  bishop_and_knight_end_game,
  kasparov_vs_topalov,
  kings_only,
  pawn_all_out_assault,
  pawns_at_promotion,
  pawns_near_promotion,
  queen_end_game,
  ready_to_castle,
  standard
};

/// Get all the starting position types
std::vector<starting_position_type> get_all_starting_position_types() noexcept;

constexpr starting_position_type get_default_starting_position() { return starting_position_type::standard; }

/// Get the next starting position,
/// i.e. when the user presses right
starting_position_type get_next(const starting_position_type starting_position) noexcept;

/// Test this class and its free functions
void test_starting_position_type();

std::string to_str(const starting_position_type t) noexcept;

#endif // STARTING_POSITION_TYPE_H
