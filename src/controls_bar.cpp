#include "controls_bar.h"

#ifndef LOGIC_ONLY

#include "physical_controllers.h"
#include "draw.h"
#include "action_number.h"

#include <SFML/Graphics/Color.hpp>

#include <cassert>

controls_bar::controls_bar(const physical_controllers& pc)
  : m_background{get_default_screen_rect()},
    m_pc{pc}
{

}

void controls_bar::draw()
{
  const auto controls_bar_rect{create_controls_bar_area(m_background.get_br())};
  assert(get_height(controls_bar_rect) == 64);
  const int y1{controls_bar_rect.get_tl().get_y()};
  const int y2{controls_bar_rect.get_br().get_y()};
  const int font_size{26};
  const int label_up_width{50};
  const int label_down_width{90};
  const int label_invert_width{100};
  const int label_select_width{100};
  const int label_increase_width{120};
  const int label_decrease_width{120};
  int label_quit_width{0};
  if (m_escape_button_text == "Quit")
  {
    label_quit_width = 80;
  }
  else
  {
    assert(m_escape_button_text == "Reject changes and go back");
    label_quit_width = 350;
  }
  assert(label_quit_width > 0);


  const sf::Color semi_transparent(128, 128, 128, 128);

  // x coordinate of the last element
  int x{controls_bar_rect.get_tl().get_x()};
  // Width of the last element
  int w = 0;

  // Show the symbol of a key and move to the right
  auto show_key = [&x, &w, y1, y2, semi_transparent](const sf::Keyboard::Key k)
  {
    x += w;
    w = 64;
    const screen_rect r(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(k, r);
  };

  // Show the symbol of a key and move to the right
  auto show_button = [&x, &w, y1, y2, semi_transparent](const sf::Mouse::Button b)
  {
    x += w;
    w = 64;
    const screen_rect r(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_input_prompt_symbol(b, r);
  };

  auto show_physical_controller_type_symbol = [&x, &w, y1, y2, semi_transparent](const physical_controller_type t)
  {
    x += w;
    w = 64;
    const screen_rect r(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_physical_controller_symbol(t, r);
  };

  // Show text and move to the right
  auto show_text = [&x, &w, y1, y2, semi_transparent](const sf::String& s, const int text_width)
  {
    x += w;
    w = text_width;
    const screen_rect r(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
    draw_text(s, r, font_size);
  };

  // Draw a spacer and move to the right
  auto show_spacer = [&x, &w, y1, y2, semi_transparent]()
  {
    x += w;
    w = 16;
    const screen_rect r(screen_coordinate(x, y1), screen_coordinate(x + w, y2));
    draw_rectangle(r, semi_transparent);
  };

  // Up
  if (m_draw_up_down)
  {
    show_key(sf::Keyboard::Key::Up);
    show_text("Up", label_up_width);
    show_spacer();
  }

  // Down
  if (m_draw_up_down)
  {
    show_key(sf::Keyboard::Key::Down);
    show_text("Down", label_down_width);
    show_spacer();
  }

  // Select
  if (m_draw_select)
  {
    show_key(sf::Keyboard::Key::Space);
    show_key(sf::Keyboard::Key::Enter);
    show_button(sf::Mouse::Button::Left);
    show_text("Select", label_select_width);
    show_spacer();
  }
  if (m_draw_left_right_increase_descrease)
  {
    show_key(sf::Keyboard::Key::Left);
    show_button(sf::Mouse::Button::Right);
    show_text("Decrease", label_decrease_width);
    show_spacer();
    show_key(sf::Keyboard::Key::Right);
    show_button(sf::Mouse::Button::Left);
    show_text("Increse", label_increase_width);
    show_spacer();
  }

  // Invert
  if (m_draw_invert)
  {
    show_key(sf::Keyboard::Key::LShift);
    show_text("Invert", label_invert_width);
    show_spacer();
  }

  if (m_draw_player_controls)
  {
    for (const auto player_side: { side::lhs, side::rhs })
    {
      const auto& c{m_pc.get_controller(player_side)};

      if (player_side == side::rhs) x = get_width(m_background) / 2;

      // Symbol
      show_physical_controller_type_symbol(c.get_type());

      // Up
      show_key(c.get_key_bindings().get_key_for_move_up());
      show_text("Up", label_up_width);
      show_spacer();

      // Down
      show_key(c.get_key_bindings().get_key_for_move_down());
      show_text("Down", label_down_width);
      show_spacer();

      // Select
      show_key(c.get_key_bindings().get_key_for_action(action_number(1)));
      show_text("Select", label_select_width);
      show_spacer();
    }
  }

  // Enter
  if (m_draw_enter)
  {
      show_key(sf::Keyboard::Key::Return);
      const int label_width{250};
      show_text("Accept and go back", label_width);
      show_spacer();
  }

  // Quit
  show_key(sf::Keyboard::Key::Escape);
  show_key(sf::Keyboard::Key::F4);
  show_text(m_escape_button_text, label_quit_width);
  show_spacer();
}

screen_rect create_controls_bar_area(const screen_coordinate& window_size) noexcept
{

  const int controls_bar_height{64};
  const int x1{0 + 16};
  const int x2{window_size.get_x() - 16};
  const int y2{window_size.get_y() - 16};
  const int y1{y2 - controls_bar_height};
  return screen_rect(screen_coordinate(x1, y1), screen_coordinate(x2, y2));
}

#endif // LOGIC_ONLY
