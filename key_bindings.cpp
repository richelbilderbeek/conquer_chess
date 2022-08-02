#include "key_bindings.h"

#include <cassert>

key_bindings::key_bindings()
{

}

sf::Keyboard::Key key_bindings::get_key_for_action(const int action) const noexcept
{
  assert(action >= 1); // Human based counting
  assert(action <= 4); // Human based counting
  switch (action)
  {
    case 1: return sf::Keyboard::Key::Q;
    case 2: return sf::Keyboard::Key::E;
    case 3: return sf::Keyboard::Key::Z;
    case 4:
    default:
      assert(action == 4);
      return sf::Keyboard::Key::C;
  }
}

sf::Keyboard::Key key_bindings::get_key_for_move_down() const noexcept
{
  return sf::Keyboard::Key::Down;
}

sf::Keyboard::Key key_bindings::get_key_for_move_left() const noexcept
{
  return sf::Keyboard::Key::Left;
}

sf::Keyboard::Key key_bindings::get_key_for_move_right() const noexcept
{
  return sf::Keyboard::Key::Right;
}

sf::Keyboard::Key key_bindings::get_key_for_move_up() const noexcept
{
  return sf::Keyboard::Key::Up;
}

void test_key_bindings()
{
#ifndef NDEBUG
  {
    const key_bindings k;
    assert(k.get_key_for_action(1) != k.get_key_for_action(2));
    assert(k.get_key_for_action(3) != k.get_key_for_action(4));
    assert(k.get_key_for_move_up() != k.get_key_for_move_down());
    assert(k.get_key_for_move_left() != k.get_key_for_move_right());
  }
#endif // NDEBUG
}
