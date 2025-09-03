#include "mouse_bindings.h"

#include "action_number.h"

#include <cassert>
#include <iostream>
#include <sstream>

mouse_bindings::mouse_bindings(
  const sf::Mouse::Button button_1,
  const sf::Mouse::Button button_2
)
{
  m_buttons[action_number(1)] = button_1;
  m_buttons[action_number(2)] = button_2;
}

const sf::Mouse::Button& mouse_bindings::get_button_for_action(const action_number& n) const
{
  return m_buttons.at(n);
}

void test_mouse_bindings()
{
#ifndef NDEBUG
  // get_button_for_action
  {
    const mouse_bindings k;
    assert(k.get_button_for_action(action_number(1)) == sf::Mouse::Button::Left);
    assert(k.get_button_for_action(action_number(2)) == sf::Mouse::Button::Right);
  }
  // operator==
  {
    const mouse_bindings mbs1(sf::Mouse::Button::Left, sf::Mouse::Button::Right);
    const mouse_bindings mbs2(sf::Mouse::Button::Left, sf::Mouse::Button::Right);
    const mouse_bindings mbs3(sf::Mouse::Button::Left, sf::Mouse::Button::Middle);
    const mouse_bindings mbs4(sf::Mouse::Button::Right, sf::Mouse::Button::Left);
    assert(mbs1 == mbs2);
    assert(!(mbs1 == mbs3));
    assert(!(mbs1 == mbs4));
    assert(!(mbs3 == mbs4));
  }
  // operator<<
  {
    const mouse_bindings mbs;
    std::stringstream s;
    s << mbs;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const mouse_bindings& lhs, const mouse_bindings& rhs) noexcept
{
  return lhs.get_button_for_action(action_number(1)) == rhs.get_button_for_action(action_number(1))
    && lhs.get_button_for_action(action_number(2)) == rhs.get_button_for_action(action_number(2))
  ;
}

std::ostream& operator<<(std::ostream& os, const mouse_bindings& mbs) noexcept
{
  os
    << "Button 1: " << mbs.get_button_for_action(action_number(1)) << '\n'
    << "Button 2: " << mbs.get_button_for_action(action_number(2)) << '\n'
  ;
  return os;
}
