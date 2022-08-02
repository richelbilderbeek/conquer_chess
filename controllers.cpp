#include "controllers.h"

std::vector<controller> get_default_controllers() noexcept
{
  return
  {
    controller(controller_type::keyboard, side::lhs),
    controller(controller_type::mouse, side::rhs)
  };
}
