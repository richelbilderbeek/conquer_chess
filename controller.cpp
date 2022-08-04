#include "controller.h"

#include "game.h"
#include "sfml_helper.h"

#include <cassert>

controller::controller(
  const controller_type type,
  const side player,
  const key_bindings& ks
)
  : m_key_bindings{ks},
    m_player{player},
    m_type{type}
{

}

controller create_default_keyboard_controller(const side player) noexcept
{
  return create_left_keyboard_controller(player);
}

controller create_default_mouse_controller(const side player) noexcept
{
  return controller(
    controller_type::mouse,
    player,
    create_right_keyboard_key_bindings()
  );
}

sf::Event create_key_pressed_event(const sf::Keyboard::Key k)
{
  sf::Event e;
  e.type = sf::Event::KeyPressed;
  e.key.code = k;
  return e;
}

controller create_left_keyboard_controller(const side player) noexcept
{
  return controller(
    controller_type::keyboard,
    player,
    create_left_keyboard_key_bindings()
  );
}

sf::Event create_mouse_button_pressed_event(
  const screen_coordinat& cursor_pos,
  const sf::Mouse::Button mouse_button
)
{
  sf::Event e;
  e.type = sf::Event::MouseButtonPressed;
  e.mouseMove.x = cursor_pos.get_x();
  e.mouseMove.y = cursor_pos.get_y();
  e.mouseButton.button = mouse_button;
  return e;
}

sf::Event create_mouse_moved_event(const screen_coordinat& cursor_pos)
{
  sf::Event e;
  e.type = sf::Event::MouseMoved;
  e.mouseMove.x = cursor_pos.get_x();
  e.mouseMove.y = cursor_pos.get_y();
  return e;
}

controller create_right_keyboard_controller(const side player) noexcept
{
  return controller(
    controller_type::keyboard,
    player,
    create_right_keyboard_key_bindings()
  );
}

sf::Keyboard::Key get_key_for_action(const controller& c, const int action)
{
  assert(action >= 1);
  assert(action <= 4);
  return get_key_for_action(c.get_key_bindings(), action);
}

std::string get_text_for_action(
  const controller& c,
  const bool has_selected_units,
  const int action_key_number
) noexcept
{
  assert(action_key_number >= 1); // Human based counting
  assert(action_key_number <= 4); // Human based counting

  std::string key_str; // Can be QEZC (keyboard) or 1234 (mouse)
  if (c.get_type() == controller_type::keyboard)
  {
    key_str = to_str(c.get_key_bindings().get_key_for_action(action_key_number));
  }
  else
  {
    assert(c.get_type() == controller_type::mouse);
    key_str = std::to_string(action_key_number);
  }

  if (has_selected_units)
  {
    switch (action_key_number)
    {
      case 1: return key_str + "\nMove";
      case 2: return key_str + "\nAttack";
      case 3: return key_str;
      case 4:
        default:
        assert(action_key_number == 4);
        return key_str;
    }
  }
  else
  {
    switch (action_key_number)
    {
      case 1: return key_str + "\nSelect";
      case 2: return key_str;
      case 3: return key_str;
      case 4:
        default:
        assert(action_key_number== 4);
        return key_str;
    }
  }
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
  std::vector<control_action> v;
  const auto actions_types{m_key_bindings.create_actions(event.key.code)};
  for (const auto t: actions_types)
  {
     v.push_back(control_action(t, m_player, game_coordinat()));
  }
  return v;
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
        ),
        m_player
      )
    };
  }
  assert(event.mouseButton.button == sf::Mouse::Right);
  return {
    create_press_rmb_action(
      convert_to_game_coordinat(
        mouse_screen_pos,
        g.get_layout()
      ),
      m_player
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
  return { create_mouse_move_action(mouse_game_pos, m_player) };
}

void test_controller()
{
#ifndef NDEBUG
  // create_default_keyboard_controller
  {
    const controller c{create_default_keyboard_controller(side::lhs)};
    assert(c.get_player() == side::lhs);
    const controller d{create_default_keyboard_controller(side::rhs)};
    assert(d.get_player() == side::rhs);
    assert(c != d);
  }
  // controller::get_player
  {
    const controller c{create_left_keyboard_controller(side::lhs)};
    assert(c.get_player() == side::lhs);
    const controller d{create_left_keyboard_controller(side::rhs)};
    assert(d.get_player() == side::rhs);
  }
  // controller::get_type
  {
    const controller c{create_default_mouse_controller(side::lhs)};
    assert(c.get_type() == controller_type::mouse);
    const controller d{create_left_keyboard_controller(side::lhs)};
    assert(d.get_type() == controller_type::keyboard);
    const controller e{create_right_keyboard_controller(side::lhs)};
    assert(e.get_type() == controller_type::keyboard);
  }
  // controller::process_input, mouse moved
  {
    game g;
    const controller c{create_default_mouse_controller(side::lhs)};
    std::vector<control_action> actions{
      c.process_input(
        create_mouse_moved_event(screen_coordinat()),
        g
      )
    };
    assert(!actions.empty());
  }
  // controller::process_input, LMB mouse pressed
  {
    game g;
    const controller c{create_default_mouse_controller(side::lhs)};
    std::vector<control_action> actions{
      c.process_input(
        create_mouse_button_pressed_event(screen_coordinat(), sf::Mouse::Left),
        g
      )
    };
    assert(!actions.empty());
  }
  // controller::process_input, RMB mouse pressed
  {
    game g;
    const controller c{create_default_mouse_controller(side::lhs)};
    std::vector<control_action> actions{
      c.process_input(
        create_mouse_button_pressed_event(screen_coordinat(), sf::Mouse::Right),
        g
      )
    };
    assert(!actions.empty());
  }
  // create_left_keyboard_controller
  {
    const auto c{create_left_keyboard_controller(side::lhs)};
    assert(c.get_type() == controller_type::keyboard);
    assert(c.get_player() == side::lhs);
    const auto d{create_left_keyboard_controller(side::rhs)};
    assert(d.get_type() == controller_type::keyboard);
    assert(d.get_player() == side::rhs);
  }
  // create_right_keyboard_controller
  {
    const auto c{create_right_keyboard_controller(side::lhs)};
    assert(c.get_type() == controller_type::keyboard);
    assert(c.get_player() == side::lhs);
    const auto d{create_right_keyboard_controller(side::rhs)};
    assert(d.get_type() == controller_type::keyboard);
    assert(d.get_player() == side::rhs);
  }
  // get_text_for_action
  {
    assert(get_text_for_action(create_left_keyboard_controller(side::lhs), false, 1) == "Q\nSelect");
    assert(get_text_for_action(create_left_keyboard_controller(side::lhs), false, 2) == "E");
    assert(get_text_for_action(create_left_keyboard_controller(side::lhs), false, 3) == "Z");
    assert(get_text_for_action(create_left_keyboard_controller(side::lhs), false, 4) == "C");
    assert(get_text_for_action(create_left_keyboard_controller(side::lhs), true, 1) == "Q\nMove");
    assert(get_text_for_action(create_left_keyboard_controller(side::lhs), true, 2) == "E\nAttack");
    assert(get_text_for_action(create_left_keyboard_controller(side::lhs), true, 3) == "Z");
    assert(get_text_for_action(create_left_keyboard_controller(side::lhs), true, 4) == "C");

    assert(get_text_for_action(create_default_mouse_controller(side::lhs), false, 1) == "1\nSelect");
    assert(get_text_for_action(create_default_mouse_controller(side::lhs), false, 2) == "2");
    assert(get_text_for_action(create_default_mouse_controller(side::lhs), false, 3) == "3");
    assert(get_text_for_action(create_default_mouse_controller(side::lhs), false, 4) == "4");
    assert(get_text_for_action(create_default_mouse_controller(side::lhs), true, 1) == "1\nMove");
    assert(get_text_for_action(create_default_mouse_controller(side::lhs), true, 2) == "2\nAttack");
    assert(get_text_for_action(create_default_mouse_controller(side::lhs), true, 3) == "3");
    assert(get_text_for_action(create_default_mouse_controller(side::lhs), true, 4) == "4");
  }
  // press up does nothing with a mouse
  {
    const game g;
    const controller c{create_default_mouse_controller(side::lhs)};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_up()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(actions.empty());
  }
  // press up works with a keyboard
  {
    const game g;
    const controller c{create_left_keyboard_controller(side::lhs)};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_up()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press right works with a keyboard
  {
    const game g;
    const controller c{create_left_keyboard_controller(side::lhs)};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_right()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press down works with a keyboard
  {
    const game g;
    const controller c{create_left_keyboard_controller(side::lhs)};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_down()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press left works with a keyboard
  {
    const game g;
    const controller c{create_left_keyboard_controller(side::lhs)};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_left()
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press action 1 works with a keyboard
  {
    const game g;
    const controller c{create_left_keyboard_controller(side::lhs)};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(1)
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press action 2 works with a keyboard
  {
    const game g;
    const controller c{create_left_keyboard_controller(side::lhs)};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(2)
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press action 3 works with a keyboard
  {
    const game g;
    const controller c{create_left_keyboard_controller(side::lhs)};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(3)
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  // press action 4 works
  {
    const game g;

    const controller c{create_left_keyboard_controller(side::lhs)};
    const auto event{
      create_key_pressed_event(
        get_key_for_action(g, side::lhs, 4)
      )
    };
    const auto actions{c.process_input(event, g)};
    assert(!actions.empty());
  }
  //
  {
    const game g;
    const controller c{create_default_mouse_controller(side::lhs)};
    sf::Event e;
    e.type = sf::Event::KeyPressed;
    assert(c.get_type() == controller_type::mouse);
    assert(c.process_input(e, g).empty());
  }
  // operator==
  {
    const controller a{create_left_keyboard_controller(side::lhs)};
    const controller b{create_left_keyboard_controller(side::lhs)};
    const controller c{create_right_keyboard_controller(side::lhs)};
    assert(a == b);
    assert(!(a == c));
  }
#endif
}

bool operator==(const controller& lhs, const controller& rhs) noexcept
{
  return lhs.get_key_bindings() == rhs.get_key_bindings()
    && lhs.get_player() == rhs.get_player()
    && lhs.get_type() == rhs.get_type()
  ;
}

bool operator!=(const controller& lhs, const controller& rhs) noexcept
{
  return !(lhs == rhs);
}
