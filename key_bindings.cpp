#include "key_bindings.h"

#include <cassert>

key_bindings::key_bindings(
  const sf::Keyboard::Key up,
  const sf::Keyboard::Key right,
  const sf::Keyboard::Key down,
  const sf::Keyboard::Key left,
  const sf::Keyboard::Key action_1,
  const sf::Keyboard::Key action_2,
  const sf::Keyboard::Key action_3,
  const sf::Keyboard::Key action_4
) : m_actions{action_1, action_2, action_3, action_4},
  m_down{down},
  m_left{left},
  m_right{right},
  m_up{up}
{

}

std::vector<control_action_type> key_bindings::create_actions(
  const sf::Keyboard::Key key_pressed
) const noexcept
{
  if (key_pressed == get_key_for_move_up())
  {
    return { control_action_type::press_up };
  }
  else if (key_pressed == get_key_for_move_right())
  {
    return { control_action_type::press_right };
  }
  else if (key_pressed == get_key_for_move_down())
  {
    return { control_action_type::press_down };
  }
  else if (key_pressed == get_key_for_move_left())
  {
    return { control_action_type::press_left };
  }
  else if (key_pressed == get_key_for_action(1))
  {
    return { control_action_type::press_action_1 };
  }
  else if (key_pressed == get_key_for_action(2))
  {
    return { control_action_type::press_action_2 };
  }
  else if (key_pressed == get_key_for_action(3))
  {
    return { control_action_type::press_action_3 };
  }
  else if (key_pressed == get_key_for_action(4))
  {
    return { control_action_type::press_action_4 };
  }
  return {};

}

key_bindings create_left_keyboard_key_bindings() noexcept
{
  return key_bindings(
    sf::Keyboard::Key::W,
    sf::Keyboard::Key::D,
    sf::Keyboard::Key::S,
    sf::Keyboard::Key::A,
    sf::Keyboard::Key::Q,
    sf::Keyboard::Key::E,
    sf::Keyboard::Key::Z,
    sf::Keyboard::Key::C
  );
}

key_bindings create_right_keyboard_key_bindings() noexcept
{
  return key_bindings(
    sf::Keyboard::Key::O,
    sf::Keyboard::Key::Semicolon,
    sf::Keyboard::Key::L,
    sf::Keyboard::Key::K,
    sf::Keyboard::Key::I,
    sf::Keyboard::Key::P,
    sf::Keyboard::Key::Comma,
    sf::Keyboard::Key::Slash
  );
}

sf::Keyboard::Key key_bindings::get_key_for_action(const int action) const noexcept
{
  assert(action >= 1); // Human based counting
  assert(action <= 4); // Human based counting
  const int i{action - 1};
  assert(i >= 0);
  assert(i < static_cast<int>(m_actions.size()));
  return m_actions[i];
}

sf::Keyboard::Key get_key_for_action(const key_bindings& k, const int action) noexcept
{
  assert(action >= 1); // Human based counting
  assert(action <= 4); // Human based counting
  return k.get_key_for_action(action);
}

void test_key_bindings()
{
#ifndef NDEBUG
  // key_bindings::key_bindings
  {
    const key_bindings k = create_left_keyboard_key_bindings();
    assert(k.get_key_for_action(1) != k.get_key_for_action(2));
    assert(k.get_key_for_action(3) != k.get_key_for_action(4));
    assert(k.get_key_for_move_up() != k.get_key_for_move_down());
    assert(k.get_key_for_move_left() != k.get_key_for_move_right());
  }
  // A key that is not in the bindings results in no actions
  {
    const key_bindings k = create_left_keyboard_key_bindings();
    assert(k.create_actions(sf::Keyboard::X).empty());
  }
  {
    const auto a{create_left_keyboard_key_bindings()};
    const auto b{create_left_keyboard_key_bindings()};
    const auto c{create_right_keyboard_key_bindings()};
    assert(a == b);
    assert(!(a == c));
  }
#endif // NDEBUG
}

bool operator==(const key_bindings& lhs, const key_bindings& rhs) noexcept
{
  return lhs.get_key_for_action(1) == rhs.get_key_for_action(1)
    && lhs.get_key_for_action(2) == rhs.get_key_for_action(2)
    && lhs.get_key_for_action(3) == rhs.get_key_for_action(3)
    && lhs.get_key_for_action(4) == rhs.get_key_for_action(4)
    && lhs.get_key_for_move_down() == rhs.get_key_for_move_down()
    && lhs.get_key_for_move_left() == rhs.get_key_for_move_left()
    && lhs.get_key_for_move_right() == rhs.get_key_for_move_right()
    && lhs.get_key_for_move_up() == rhs.get_key_for_move_up()
  ;
}
