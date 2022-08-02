#include "controllers.h"

#include <cassert>

std::vector<controller> get_default_controllers() noexcept
{
  return
  {
    controller(controller_type::keyboard, side::lhs),
    controller(controller_type::mouse, side::rhs)
  };
}
side get_keyboard_user_player_side(const std::vector<controller>& controllers)
{
  const auto i{
    std::find_if(
      std::begin(controllers),
      std::end(controllers),
      [](const auto& c)
      {
        return c.get_type() == controller_type::keyboard;
      }
    )
  };
  assert(i != std::end(controllers));
  return (*i).get_player();
}

side get_mouse_user_player_side(const std::vector<controller>& controllers)
{
  const auto i{
    std::find_if(
      std::begin(controllers),
      std::end(controllers),
      [](const auto& c)
      {
        return c.get_type() == controller_type::mouse;
      }
    )
  };
  assert(i != std::end(controllers));
  return (*i).get_player();
}
