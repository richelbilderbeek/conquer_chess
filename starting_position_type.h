#ifndef STARTING_POSITION_TYPE_H
#define STARTING_POSITION_TYPE_H

#include <string>

/// The starting position type
enum class starting_position_type
{
  standard,
  kings_only
};

constexpr starting_position_type get_default_starting_position() { return starting_position_type::standard; }

std::string to_str(const starting_position_type t) noexcept;

#endif // STARTING_POSITION_TYPE_H
