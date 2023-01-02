#include "physical_controller.h"

#include "action_number.h"
#include "game.h"
#include "sfml_helper.h"

#include <cassert>
#include <iostream>
#include <sstream>

physical_controller::physical_controller(
  const physical_controller_type type,
  //const side player,
  const key_bindings& ks
)
  : m_key_bindings{ks},
    //m_player{player},
    m_type{type}
{

}

physical_controller create_default_keyboard_controller() noexcept
{
  return create_left_keyboard_controller();
}

physical_controller create_default_mouse_controller(/* const side player */) noexcept
{
  return physical_controller(
    physical_controller_type::mouse,
    //player,
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

physical_controller create_left_keyboard_controller() noexcept
{
  return physical_controller(
    physical_controller_type::keyboard,
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

physical_controller create_right_keyboard_controller() noexcept
{
  return physical_controller(
    physical_controller_type::keyboard,
    create_right_keyboard_key_bindings()
  );
}

sf::Keyboard::Key get_key_for_action(const physical_controller& c, const action_number& action)
{
  return get_key_for_action(c.get_key_bindings(), action);
}

std::string get_text_for_action(
  const physical_controller& c,
  const bool has_selected_units,
  const bool is_promoting_pawn,
  const bool is_king_that_may_castle_kingside,
  const bool is_king_that_may_castle_queenside,
  const action_number& action_key_number
) noexcept
{
  std::string key_str; // Can be QEZC (keyboard) or 1234 (mouse)
  if (c.get_type() == physical_controller_type::keyboard)
  {
    key_str = to_str(c.get_key_bindings().get_key_for_action(action_key_number));
  }
  else
  {
    assert(c.get_type() == physical_controller_type::mouse);
    key_str = std::to_string(action_key_number.get_number());
  }
  if (has_selected_units)
  {
    if (is_promoting_pawn)
    {
      switch (action_key_number.get_number())
      {
        case 1: return key_str + "\nPromote\nto\nqueen";
        case 2: return key_str + "\nPromote\nto\nrook";
        case 3: return key_str + "\nPromote\nto\nbishop";
        case 4:
          default:
          assert(action_key_number.get_number() == 4);
          return key_str + "\nPromote\nto\nknight";
      }
    }
    switch (action_key_number.get_number())
    {
      case 1: return key_str + "\nMove";
      case 2: return key_str + "\nAttack";
      case 3:
      {
        if (is_king_that_may_castle_kingside)
        {
          return key_str + "\nCastle\nkingside";
        }
        return key_str;
      }
      case 4:
        default:
        assert(action_key_number.get_number() == 4);
        {
          if (is_king_that_may_castle_queenside)
          {
            return key_str + "\nCastle\nqueenside";
          }
          return key_str;
        }
    }
  }
  else
  {
    switch (action_key_number.get_number())
    {
      case 1: return key_str + "\nSelect";
      case 2: return key_str;
      case 3: return key_str;
      case 4:
        default:
        assert(action_key_number.get_number() == 4);
        return key_str;
    }
  }
}

user_inputs physical_controller::process_input(
  const sf::Event& event,
  const side player_side,
  const game& g
) const
{
  if (m_type == physical_controller_type::keyboard)
  {
    if (event.type == sf::Event::KeyPressed)
    {
      return process_key_press(event, player_side);
    }
  }
  else
  {
    assert(m_type == physical_controller_type::mouse);
    if (event.type == sf::Event::MouseMoved)
    {
      return process_mouse_moved(event, player_side, g);
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
      return process_mouse_pressed(event, player_side);
    }
  }
  // Ignore the rest
  return {};
}

user_inputs physical_controller::process_key_press(
  const sf::Event& event,
  const side player_side
) const
{
  assert(event.type == sf::Event::KeyPressed);
  std::vector<user_input> v;
  const auto actions_types{m_key_bindings.create_actions(event.key.code)};
  for (const auto t: actions_types)
  {
     v.push_back(user_input(t, player_side));
  }
  return v;
}

user_inputs physical_controller::process_mouse_pressed(
  const sf::Event& event,
  const side player_side
) const
{
  assert(event.type == sf::Event::MouseButtonPressed);
  if (event.mouseButton.button == sf::Mouse::Left)
  {
    return user_inputs(
      {
        create_press_lmb_action(
          player_side
        )
      }
    );
  }
  assert(event.mouseButton.button == sf::Mouse::Right);
  return user_inputs(
    {
      create_press_rmb_action(player_side)
    }
  );
}

user_inputs physical_controller::process_mouse_moved(
  const sf::Event& event,
  const side player_side,
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
  return user_inputs(
    { create_mouse_move_action(mouse_game_pos, player_side) }
  );
}


void test_controller()
{
#ifndef NDEBUG
  // controller::get_type
  {
    const physical_controller c{create_default_mouse_controller()};
    assert(c.get_type() == physical_controller_type::mouse);
    const physical_controller d{create_left_keyboard_controller()};
    assert(d.get_type() == physical_controller_type::keyboard);
    const physical_controller e{create_right_keyboard_controller()};
    assert(e.get_type() == physical_controller_type::keyboard);
  }
  // controller::process_input, mouse moved
  {
    const game g;
    const physical_controller c{create_default_mouse_controller()};
    const user_inputs inputs(
      {
        c.process_input(
          create_mouse_moved_event(screen_coordinat()),
          side::lhs,
          g
        )
      }
    );
    assert(!is_empty(inputs));
  }
  // controller::process_input, LMB mouse pressed
  {
    const game g;
    const physical_controller c{create_default_mouse_controller()};
    const user_inputs inputs(
      {
        c.process_input(
          create_mouse_button_pressed_event(screen_coordinat(), sf::Mouse::Left),
          side::lhs,
          g
        )
      }
    );
    assert(!is_empty(inputs));
  }
  // controller::process_input, RMB mouse pressed
  {
    const game g;
    const physical_controller c{create_default_mouse_controller()};
    const user_inputs inputs(
      {
        c.process_input(
          create_mouse_button_pressed_event(screen_coordinat(), sf::Mouse::Right),
          side::lhs,
          g
        )
      }
    );
    assert(!is_empty(inputs));
  }
  // create_left_keyboard_controller
  {
    const auto c{create_left_keyboard_controller()};
    assert(c.get_type() == physical_controller_type::keyboard);
  }
  // create_right_keyboard_controller
  {
    const auto c{create_right_keyboard_controller()};
    assert(c.get_type() == physical_controller_type::keyboard);
    const auto d{create_right_keyboard_controller()};
    assert(d.get_type() == physical_controller_type::keyboard);
  }
  // get_text_for_action
  {
    assert(get_text_for_action(create_left_keyboard_controller(), false, false, false, false, action_number(1)) == "Q\nSelect");
    assert(get_text_for_action(create_left_keyboard_controller(), false, false, false, false, action_number(2)) == "E");
    assert(get_text_for_action(create_left_keyboard_controller(), false, false, false, false, action_number(3)) == "Z");
    assert(get_text_for_action(create_left_keyboard_controller(), false, false, false, false, action_number(4)) == "C");
    assert(get_text_for_action(create_left_keyboard_controller(), true, false, false, false, action_number(1)) == "Q\nMove");
    assert(get_text_for_action(create_left_keyboard_controller(), true, false, false, false, action_number(2)) == "E\nAttack");
    assert(get_text_for_action(create_left_keyboard_controller(), true, false, false, false, action_number(3)) == "Z");
    assert(get_text_for_action(create_left_keyboard_controller(), true, false, false, false, action_number(4)) == "C");

    assert(get_text_for_action(create_default_mouse_controller(), false, false, false, false, action_number(1)) == "1\nSelect");
    assert(get_text_for_action(create_default_mouse_controller(), false, false, false, false, action_number(2)) == "2");
    assert(get_text_for_action(create_default_mouse_controller(), false, false, false, false, action_number(3)) == "3");
    assert(get_text_for_action(create_default_mouse_controller(), false, false, false, false, action_number(4)) == "4");
    assert(get_text_for_action(create_default_mouse_controller(), true, false, false, false, action_number(1)) == "1\nMove");
    assert(get_text_for_action(create_default_mouse_controller(), true, false, false, false, action_number(2)) == "2\nAttack");
    assert(get_text_for_action(create_default_mouse_controller(), true, false, false, false, action_number(3)) == "3");
    assert(get_text_for_action(create_default_mouse_controller(), true, false, false, false, action_number(4)) == "4");

    // Castling
    assert(get_text_for_action(create_default_mouse_controller(), true, false, true, false, action_number(3)) == "3\nCastle\nkingside");
    assert(get_text_for_action(create_default_mouse_controller(), true, false, false, true, action_number(4)) == "4\nCastle\nqueenside");

    // Promotion
    assert(get_text_for_action(create_default_mouse_controller(), true, true, false, false, action_number(1)) == "1\nPromote\nto\nqueen");
    assert(get_text_for_action(create_default_mouse_controller(), true, true, false, false, action_number(2)) == "2\nPromote\nto\nrook");
    assert(get_text_for_action(create_default_mouse_controller(), true, true, false, false, action_number(3)) == "3\nPromote\nto\nbishop");
    assert(get_text_for_action(create_default_mouse_controller(), true, true, false, false, action_number(4)) == "4\nPromote\nto\nknight");

  }
  // press up does nothing with a mouse
  {
    const game g;
    const physical_controller c{create_default_mouse_controller()};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_up()
      )
    };
    const auto actions{c.process_input(event, side::lhs, g)};
    assert(is_empty(actions));
  }
  // press up works with a keyboard
  {
    const game g;
    const physical_controller c{create_left_keyboard_controller()};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_up()
      )
    };
    const auto actions{c.process_input(event, side::lhs, g)};
    assert(!is_empty(actions));
  }
  // press right works with a keyboard
  {
    const game g;
    const physical_controller c{create_left_keyboard_controller()};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_right()
      )
    };
    const auto actions{c.process_input(event, side::lhs, g)};
    assert(!is_empty(actions));
  }
  // press down works with a keyboard
  {
    const game g;
    const physical_controller c{create_left_keyboard_controller()};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_down()
      )
    };
    const auto actions{c.process_input(event, side::lhs, g)};
    assert(!is_empty(actions));
  }
  // press left works with a keyboard
  {
    const game g;
    const physical_controller c{create_left_keyboard_controller()};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_move_left()
      )
    };
    const auto actions{c.process_input(event, side::lhs, g)};
    assert(!is_empty(actions));
  }
  // press action 1 works with a keyboard
  {
    const game g;
    const physical_controller c{create_left_keyboard_controller()};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(action_number(1))
      )
    };
    const auto actions{c.process_input(event, side::lhs, g)};
    assert(!is_empty(actions));
  }
  // press action 2 works with a keyboard
  {
    const game g;
    const physical_controller c{create_left_keyboard_controller()};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(action_number(2))
      )
    };
    const auto actions{c.process_input(event, side::lhs, g)};
    assert(!is_empty(actions));
  }
  // press action 3 works with a keyboard
  {
    const game g;
    const physical_controller c{create_left_keyboard_controller()};
    const auto event{
      create_key_pressed_event(
        c.get_key_bindings().get_key_for_action(action_number(3))
      )
    };
    const auto actions{c.process_input(event, side::lhs, g)};
    assert(!is_empty(actions));
  }
  // press action 4 works
  {
    const game g;

    const physical_controller c{create_left_keyboard_controller()};
    const auto event{
      create_key_pressed_event(
        get_key_for_action(g, side::lhs, action_number(4))
      )
    };
    const auto actions{c.process_input(event, side::lhs, g)};
    assert(!is_empty(actions));
  }
  //
  {
    const game g;
    const physical_controller c{create_default_mouse_controller()};
    sf::Event e;
    e.type = sf::Event::KeyPressed;
    assert(c.get_type() == physical_controller_type::mouse);
    assert(is_empty(c.process_input(e, side::lhs, g)));
  }
  // operator==
  {
    const physical_controller a{create_left_keyboard_controller()};
    const physical_controller b{create_left_keyboard_controller()};
    const physical_controller c{create_right_keyboard_controller()};
    assert(a == b);
    assert(!(a == c));
  }
  // 41: operator<<
  {
    const physical_controller c{create_left_keyboard_controller()};
    std::stringstream s;
    s << c;
    assert(!s.str().empty());
  }
#endif
}

bool operator==(const physical_controller& lhs, const physical_controller& rhs) noexcept
{
  return lhs.get_key_bindings() == rhs.get_key_bindings()
    && lhs.get_type() == rhs.get_type()
  ;
}

bool operator!=(const physical_controller& lhs, const physical_controller& rhs) noexcept
{
  return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const physical_controller& c) noexcept
{
  os
    << "Key bindings: " << c.get_key_bindings() << '\n'
    << "Type: " << c.get_type()
  ;
  return os;
}
