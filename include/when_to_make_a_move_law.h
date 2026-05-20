#ifndef WHEN_TO_MAKE_A_MOVE_LAW
#define WHEN_TO_MAKE_A_MOVE_LAW

#include <string>
#include <vector>

/// @file The law when to make a move
/// Extension to rule 1.2
enum class when_to_make_a_move_law {
  classic,
  rts,
  white_first
};

/// Get all the message_type values
std::vector<when_to_make_a_move_law> get_all_when_to_make_a_move_laws() noexcept;

/// Get the next law when to make a move
when_to_make_a_move_law get_next(const when_to_make_a_move_law law) noexcept;

/// Get the previous law when to make a move
when_to_make_a_move_law get_previous(const when_to_make_a_move_law law) noexcept;

/// Test this class and its free functions
void test_when_to_make_a_move_law();

/// Convert the type to a string that spelled identical to the C++ value
/// @example
/// assert(to_str(when_to_make_a_move_law::classic) == "classic");
std::string to_str(const when_to_make_a_move_law law) noexcept;

/// Convert the type to a string that could be spelled by a human
/// @example
/// assert(to_str(when_to_make_a_move_law::white_first) == "White first");
std::string to_human_str(const when_to_make_a_move_law law) noexcept;

#endif // WHEN_TO_MAKE_A_MOVE_LAW
