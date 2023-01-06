#ifndef RACE_H
#define RACE_H

#include <iosfwd>
#include <string>
#include <vector>

enum class race
{
  classic,
  protoss,
  terran,
  zerg
};

std::vector<race> get_all_races() noexcept;

race get_next(const race r) noexcept;
race get_previous(const race r) noexcept;

/// Test the race functions
void test_race();

std::string to_str(const race c) noexcept;

std::ostream& operator<<(std::ostream& os, const race c) noexcept;

#endif // RACE_H
