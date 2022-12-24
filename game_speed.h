#ifndef GAME_SPEED_H
#define GAME_SPEED_H

#include "delta_t.h"

#include <iosfwd>

/// The game speed
enum class game_speed
{
  slowest, slow, normal, fast, fastest
};

/// Get the default game speed
constexpr game_speed get_default_game_speed() { return game_speed::slowest; }

/// Get the next game speed,
/// i.e. when the user presses right
game_speed get_next(const game_speed speed) noexcept;

/// Get the previous game speed,
/// i.e. when the user presses left
game_speed get_previous(const game_speed speed) noexcept;

/// Test this class and its free functions
void test_game_speed();

delta_t to_delta_t(const game_speed speed) noexcept;

std::string to_str(const game_speed speed) noexcept;

std::ostream& operator<<(std::ostream& os, const game_speed speed) noexcept;

#endif // GAME_SPEED_H
