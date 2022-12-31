#include "physical_controllers.h"

#include <algorithm>
#include <cassert>

int count_keyboard_controllers(const std::vector<physical_controller>& controllers) noexcept
{
  return std::count_if(
    std::begin(controllers),
    std::end(controllers),
    [](const auto& c)
    {
      return c.get_type() == physical_controller_type::keyboard;
    }
  );
}

int count_mouse_controllers(const std::vector<physical_controller>& controllers) noexcept
{
  return std::count_if(
    std::begin(controllers),
    std::end(controllers),
    [](const auto& c)
    {
      return c.get_type() == physical_controller_type::mouse;
    }
  );
}


std::vector<physical_controller> create_default_controllers() noexcept
{
  return create_keyboard_mouse_controllers();
}

std::vector<physical_controller> create_keyboard_mouse_controllers() noexcept
{
  return
  {
    create_left_keyboard_controller(side::lhs),
    create_default_mouse_controller(side::rhs)
  };
}

std::vector<physical_controller> create_mouse_keyboard_controllers() noexcept
{
  return
  {
    create_default_mouse_controller(side::lhs),
    create_right_keyboard_controller(side::rhs)
  };
}

std::vector<physical_controller> create_two_keyboard_controllers() noexcept
{
  return
  {
    create_left_keyboard_controller(side::lhs),
    create_right_keyboard_controller(side::rhs)
  };
}

side get_keyboard_user_player_side(const std::vector<physical_controller>& controllers)
{
  assert(has_keyboard_controller(controllers));
  const auto i{
    std::find_if(
      std::begin(controllers),
      std::end(controllers),
      [](const auto& c)
      {
        return c.get_type() == physical_controller_type::keyboard;
      }
    )
  };
  assert(i != std::end(controllers));
  return (*i).get_player();
}

side get_mouse_user_player_side(const std::vector<physical_controller>& controllers)
{
  assert(has_mouse_controller(controllers));
  const auto i{
    std::find_if(
      std::begin(controllers),
      std::end(controllers),
      [](const auto& c)
      {
        return c.get_type() == physical_controller_type::mouse;
      }
    )
  };
  assert(i != std::end(controllers));
  return (*i).get_player();
}

bool has_keyboard_controller(const std::vector<physical_controller>& controllers) noexcept
{
  return count_keyboard_controllers(controllers);
}

bool has_mouse_controller(const std::vector<physical_controller>& controllers) noexcept
{
  return count_mouse_controllers(controllers);
}

void test_controllers()
{
#ifndef NDEBUG
  // count_keyboard_controllers
  {
    assert(count_keyboard_controllers(create_default_controllers()) == 1);
    assert(count_keyboard_controllers(create_two_keyboard_controllers()) == 2);
  }
  // count_mouse_controllers
  {
    assert(count_mouse_controllers(create_default_controllers()) == 1);
    assert(count_mouse_controllers(create_two_keyboard_controllers()) == 0);
  }
  // get_default_controllers
  {
    const auto cs{create_default_controllers()};
    assert(cs.size() == 2);
  }
  // get_keyboard_user_player_side
  {
    assert(get_keyboard_user_player_side(create_default_controllers()) == side::lhs);
  }
  // get_mouse_user_player_side
  {
    assert(get_mouse_user_player_side(create_default_controllers()) == side::rhs);
  }
  // get_two_keyboard_controllers
  {
    const auto cs{create_two_keyboard_controllers()};
    assert(cs.size() == 2);
  }
  // has_keyboard_controllers
  {
    assert(has_keyboard_controller(create_default_controllers()));
    assert(has_keyboard_controller(create_two_keyboard_controllers()));
  }
  // has_mouse_controller
  {
    assert(has_mouse_controller(create_default_controllers()));
    assert(!has_mouse_controller(create_two_keyboard_controllers()));
  }
#endif // NDEBUG
}
