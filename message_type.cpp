#include "message_type.h"

std::vector<message_type> get_all_message_types() noexcept
{
  return
  {
    message_type::cannot,
    message_type::done,
    message_type::select,
    message_type::start_attack,
    message_type::start_move
  };
}
