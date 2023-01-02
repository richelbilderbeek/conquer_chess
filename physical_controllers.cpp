#include "physical_controllers.h"

#include <algorithm>
#include <cassert>

physical_controllers::physical_controllers(
  const std::vector<physical_controller> controllers
) : m_physical_controllers{controllers}
{

}

int count_keyboard_controllers(const physical_controllers& controllers) noexcept
{
  const auto& cs{controllers.get_controllers()};
  return std::count_if(
    std::begin(cs),
    std::end(cs),
    [](const auto& c)
    {
      return c.get_type() == physical_controller_type::keyboard;
    }
  );
}

int count_mouse_controllers(const physical_controllers& controllers) noexcept
{
  const auto& cs{controllers.get_controllers()};
  return std::count_if(
    std::begin(cs),
    std::end(cs),
    [](const auto& c)
    {
      return c.get_type() == physical_controller_type::mouse;
    }
  );
}

int count_n_controllers(const physical_controllers& controllers) noexcept
{
  return controllers.get_controllers().size();
}

physical_controllers create_default_controllers() noexcept
{
  return create_keyboard_mouse_controllers();
}

physical_controllers create_keyboard_mouse_controllers() noexcept
{
  return physical_controllers(
    {
      create_left_keyboard_controller(),
      create_default_mouse_controller()
    }
  );
}

physical_controllers create_mouse_keyboard_controllers() noexcept
{
  return physical_controllers(
    {
      create_default_mouse_controller(),
      create_right_keyboard_controller()
    }
  );
}

physical_controllers create_two_keyboard_controllers() noexcept
{
  return physical_controllers(
    {
      create_left_keyboard_controller(),
      create_right_keyboard_controller()
    }
  );
}

const physical_controller& physical_controllers::get_controller(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    assert(count_n_controllers(m_physical_controllers) >= 1);
    return m_physical_controllers[0];
  }
  assert(player_side == side::rhs);
  assert(count_n_controllers(m_physical_controllers) >= 2);
  return m_physical_controllers[1];
}

side get_keyboard_user_player_side(const physical_controllers& controllers)
{
  const auto& cs{controllers.get_controllers()};
  if (cs[0].get_type() == physical_controller_type::keyboard)
  {
    return side::lhs;
  }
  assert(cs[1].get_type() == physical_controller_type::keyboard);
  return side::rhs;
}

side get_mouse_user_player_side(const physical_controllers& controllers)
{
  const auto& cs{controllers.get_controllers()};
  if (cs[0].get_type() == physical_controller_type::mouse)
  {
    return side::lhs;
  }
  assert(cs[1].get_type() == physical_controller_type::mouse);
  return side::rhs;
}

bool has_keyboard_controller(const physical_controllers& controllers) noexcept
{
  return count_keyboard_controllers(controllers);
}

bool has_mouse_controller(const physical_controllers& controllers) noexcept
{
  return count_mouse_controllers(controllers);
}

const std::vector<physical_controller>& physical_controllers::get_controllers() const noexcept
{
  return m_physical_controllers;
}

void physical_controllers::set(const side player_side, const physical_controller& controller)
{
  if (player_side == side::lhs)
  {
    assert(m_physical_controllers.size() >= 1);
    m_physical_controllers[0] = controller;
    return;
  }
  assert(player_side == side::rhs);
  assert(m_physical_controllers.size() >= 2);
  m_physical_controllers[1] = controller;
}

void test_physical_controllers()
{
#ifndef NDEBUG
  {
    const physical_controllers c;
  }
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
    assert(count_n_controllers(cs) == 2);
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
    assert(count_n_controllers(cs) == 2);
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
  // operator==
  {
    const physical_controllers a{create_default_controllers()};
    const physical_controllers b{create_default_controllers()};
    const physical_controllers c{create_two_keyboard_controllers()};
    assert(a == b);
    assert(!(a == c));
  }
#endif // NDEBUG
}

bool operator==(const physical_controllers& lhs, const physical_controllers& rhs) noexcept
{
  return lhs.get_controllers() == rhs.get_controllers();
}
