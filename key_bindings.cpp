#include "key_bindings.h"

#include <cassert>
#include <iostream>
#include <sstream>

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

std::vector<user_input_type> key_bindings::create_actions(
  const sf::Keyboard::Key key_pressed
) const noexcept
{
  if (key_pressed == get_key_for_move_up())
  {
    return { user_input_type::press_up };
  }
  else if (key_pressed == get_key_for_move_right())
  {
    return { user_input_type::press_right };
  }
  else if (key_pressed == get_key_for_move_down())
  {
    return { user_input_type::press_down };
  }
  else if (key_pressed == get_key_for_move_left())
  {
    return { user_input_type::press_left };
  }
  else if (key_pressed == get_key_for_action(1))
  {
    return { user_input_type::press_action_1 };
  }
  else if (key_pressed == get_key_for_action(2))
  {
    return { user_input_type::press_action_2 };
  }
  else if (key_pressed == get_key_for_action(3))
  {
    return { user_input_type::press_action_3 };
  }
  else if (key_pressed == get_key_for_action(4))
  {
    return { user_input_type::press_action_4 };
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
  // 44: operator<<
  {
    const auto keys{create_left_keyboard_key_bindings()};
    std::stringstream s;
    s << keys;
    assert(!s.str().empty());
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

std::ostream& operator<<(std::ostream& os, const key_bindings& keys) noexcept
{
  os
    << "Key for move up: " << keys.get_key_for_move_up() << '\n'
    << "Key for move right: " << keys.get_key_for_move_right() << '\n'
    << "Key for move down: " << keys.get_key_for_move_down() << '\n'
    << "Key for move left: " << keys.get_key_for_move_left() << '\n'
    << "Key for action 1: " << keys.get_key_for_action(1) << '\n'
    << "Key for action 2: " << keys.get_key_for_action(2) << '\n'
    << "Key for action 3: " << keys.get_key_for_action(3) << '\n'
    << "Key for action 4: " << keys.get_key_for_action(4)
  ;
  return os;
}
