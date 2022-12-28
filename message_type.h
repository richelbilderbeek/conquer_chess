#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

#include <iosfwd>
#include <string>
#include <vector>

/// The types of sound effects,
/// as used by 'sound_effect'
/// as played by 'sound_effects'
enum class message_type
{
  cannot,
  done,
  select,
  start_attack,
  start_move,
  unselect
};

/// Get all the message_type values
std::vector<message_type> get_all_message_types() noexcept;

/// Test this class and its free functions
void test_message_type();

std::string to_str(const message_type t) noexcept;

std::string to_str(const std::vector<message_type>& v) noexcept;

std::ostream& operator<<(std::ostream& os, const message_type t) noexcept;

#endif // MESSAGE_TYPE_H
