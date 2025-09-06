#ifndef GAME_SPEED_H
#define GAME_SPEED_H

#include <iosfwd>
#include <vector>
#include <string>

/// The game speed.
///
/// The game speed, expressed in human language.
enum class game_speed
{
  slowest, slower, slow, normal, fast, faster, fastest
};

/// Get all the game_speeds
std::vector<game_speed> get_all_game_speeds() noexcept;

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

/// Convert the game speed to a speed multiplier,
/// where `game_speed::normal` denotes the baseline
/// speed of 1 chess move per second
double get_speed_multiplier(const game_speed speed) noexcept;

std::string to_human_str(const game_speed speed) noexcept;

std::string to_str(const game_speed speed) noexcept;

std::ostream& operator<<(std::ostream& os, const game_speed speed) noexcept;

#endif // GAME_SPEED_H
