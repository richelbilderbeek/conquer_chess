#include "controller.h"

#include "game.h"

#include <cassert>

controller::controller(const controller_type type)
  : m_type{type}
{

}

std::vector<control_action> controller::process_input(
  const sf::Event& event,
  const game& g
) const
{
  if (m_type == controller_type::keyboard)
  {
    if (event.type == sf::Event::KeyPressed)
    {
      return process_key_press(event);
    }
  }
  else
  {
    assert(m_type == controller_type::mouse);
    if (event.type == sf::Event::MouseMoved)
    {
      return process_mouse_moved(event, g);
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
      return process_mouse_pressed(event, g);
    }
  }
  // Ignore the rest
  return {};
}

std::vector<control_action> controller::process_key_press(
  const sf::Event& event
) const
{
  assert(event.type == sf::Event::KeyPressed);

  sf::Keyboard::Key key_pressed = event.key.code;
  if (key_pressed == sf::Keyboard::Key::Escape)
  {
    // There is no 'quit' action, this must be handled by 'game_view'
    return {};
  }
  else if (key_pressed == sf::Keyboard::Key::Up)
  {
    return { create_press_up_action() };
  }
  else if (key_pressed == sf::Keyboard::Key::Right)
  {
    return { create_press_right_action() };
  }
  else if (key_pressed == sf::Keyboard::Key::Down)
  {
    return { create_press_down_action() };
  }
  else if (key_pressed == sf::Keyboard::Key::Left)
  {
    return { create_press_left_action() };
  }
  else if (key_pressed == sf::Keyboard::Key::Space)
  {
    return { create_press_select_action() };
  }
  else if (key_pressed == sf::Keyboard::Key::M)
  {
    return { create_press_move_action() };
  }
  else if (key_pressed == sf::Keyboard::Key::A)
  {
    return { create_press_attack_action() };
  }
  return {};
}

std::vector<control_action> controller::process_mouse_pressed(
  const sf::Event& event,
  const game& g
) const
{
  assert(event.type == sf::Event::MouseButtonPressed);
  const auto mouse_screen_pos{
    screen_coordinat(event.mouseButton.x, event.mouseButton.y)
  };
  if (event.mouseButton.button == sf::Mouse::Left)
  {
    return {
      create_press_lmb_action(
        convert_to_game_coordinat(
          mouse_screen_pos,
          g.get_layout()
        )
      )
    };
  }
  assert(event.mouseButton.button == sf::Mouse::Right);
  return {
    create_press_rmb_action(
      convert_to_game_coordinat(
        mouse_screen_pos,
        g.get_layout()
      )
    )
  };
}

std::vector<control_action> controller::process_mouse_moved(
  const sf::Event& event,
  const game& g
) const
{
  assert(event.type == sf::Event::MouseMoved);
  const auto mouse_screen_pos{
    screen_coordinat(event.mouseMove.x, event.mouseMove.y)
  };
  const auto mouse_game_pos{
    convert_to_game_coordinat(
      mouse_screen_pos,
      g.get_layout()
    )
  };
  return { create_mouse_move_action(mouse_game_pos) };
}

void test_controller()
{
#ifndef NDEBUG
  // controller::get_type
  {
    const controller c(controller_type::mouse);
    assert(c.get_type() == controller_type::mouse);
  }
  //
  {
    const game g;
    const controller c(controller_type::mouse);
    sf::Event e;
    e.type = sf::Event::KeyPressed;
    assert(c.get_type() == controller_type::mouse);
    assert(c.process_input(e, g).empty());
  }


#endif
}
