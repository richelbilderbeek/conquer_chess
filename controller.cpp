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
  // Must be handled someplace else
  if (event.type == sf::Event::Resized)
  {
    assert(!"Should be handled by game_view");
    return {};
  }
  if (event.type == sf::Event::Closed)
  {
    assert(!"Should be handled by game_view");
    return {};
  }
  /*

    else if (event.type == sf::Event::KeyPressed)
    {
      sf::Keyboard::Key key_pressed = event.key.code;
      if (key_pressed == sf::Keyboard::Key::Escape)
      {
        m_window.close();
        return true;
      }
      else if (key_pressed == sf::Keyboard::Key::Up)
      {
        m_game.add_action(create_press_up_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Right)
      {
        m_game.add_action(create_press_right_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Down)
      {
        m_game.add_action(create_press_down_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Left)
      {
        m_game.add_action(create_press_left_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Space)
      {
        m_game.add_action(create_press_select_action());
      }
      else if (key_pressed == sf::Keyboard::Key::M)
      {
        m_game.add_action(create_press_move_action());
      }
      else if (key_pressed == sf::Keyboard::Key::A)
      {
        m_game.add_action(create_press_attack_action());
      }
      else if (key_pressed == sf::Keyboard::Key::F3)
      {
        // debug
        std::clog << "Debug";
      }
    }
    if (event.type == sf::Event::MouseMoved)
    {
      const auto mouse_screen_pos{
        screen_coordinat(event.mouseMove.x, event.mouseMove.y)
      };
      const auto mouse_game_pos{
        convert_to_game_coordinat(
          mouse_screen_pos,
          m_game.get_layout()
        )
      };
      m_game.add_action(create_mouse_move_action(mouse_game_pos));
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
      const auto mouse_screen_pos{
        screen_coordinat(event.mouseButton.x, event.mouseButton.y)
      };
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        m_game.add_action(
          create_press_lmb_action(
            convert_to_game_coordinat(
              mouse_screen_pos,
              m_game.get_layout()
            )
          )
        );
      }
      else if (event.mouseButton.button == sf::Mouse::Right)
      {
        m_game.add_action(
          create_press_rmb_action(
            convert_to_game_coordinat(
              mouse_screen_pos,
              m_game.get_layout()
            )
          )
        );
      }
    }
    else if (event.type == sf::Event::KeyReleased)
    {
      // Maybe a player input?
      // Nothing yet
    }

  */
  if (event.type == sf::Event::MouseMoved)
  {
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
  return {};
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
