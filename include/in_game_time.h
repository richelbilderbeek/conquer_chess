#ifndef IN_GAME_TIME_H
#define IN_GAME_TIME_H

#include "ccfwd.h"

#include <iosfwd>
#include <string>

/// The in-game time, in chess moves.
///
/// The in-game time, where
/// - 0.0 is at the start of a game
/// - 1.0 is the earliest time a first move can have finished
/// - 2.0 is the earliest time a piece can have moved twice (assuming
///   the piece completes both moves=
class in_game_time
{
public:
  explicit in_game_time(const double in_game_time);

  double get() const noexcept { return m_in_game_time; }

private:

  double m_in_game_time;
};

/// Test this class and its free functions
void test_in_game_time();

/// Convert to literal string, with all the digits behind the comma.
///
/// For example: 1.23456789
std::string to_str(const in_game_time& t) noexcept;

/// Convert to string, rounded off to two digits
///
/// For example: 1.23
std::string to_human_str(const in_game_time& t) noexcept;

bool operator==(const in_game_time& lhs, const in_game_time& rhs) noexcept;
bool operator<(const in_game_time& lhs, const in_game_time& rhs) noexcept;
bool operator<=(const in_game_time& lhs, const in_game_time& rhs) noexcept;
bool operator>(const in_game_time& lhs, const in_game_time& rhs) noexcept;
bool operator>=(const in_game_time& lhs, const in_game_time& rhs) noexcept;

in_game_time& operator+=(in_game_time& game_time, const delta_t& dt) noexcept;
in_game_time operator+(const in_game_time& game_time, const delta_t& dt) noexcept;
in_game_time operator-(const in_game_time& game_time, const delta_t& dt) noexcept;
delta_t operator-(const in_game_time& lhs, const in_game_time& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const in_game_time& dt) noexcept;

#endif // IN_GAME_TIME_H
