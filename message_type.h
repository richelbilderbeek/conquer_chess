#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

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
  start_move
};

std::vector<message_type> get_all_message_types() noexcept;

#endif // MESSAGE_TYPE_H
