#include "controls_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinat.h"
#include "game_view.h"
#include "options_view.h"

#include "sfml_helper.h"
#include <cassert>
#include <cmath>
#include <iostream>

controls_view::controls_view(controller c)
  : m_controller(c),
    m_selected{controls_view_item::type}
{

}

void controls_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_controls_screen_size().get_x(),
      get_default_controls_screen_size().get_y()
    ),
    "Conquer Chess: controls menu"
  );

  // Center
  auto desktop = sf::VideoMode::getDesktopMode();
  m_window.setPosition(
    sf::Vector2i(
      (desktop.width / 2) - (m_window.getSize().x /2),
      (desktop.height/ 2) - (m_window.getSize().y /2)
    )
  );

  while (m_window.isOpen())
  {
    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Show the new state
    show();

  }
}

std::string get_key_str_for_action_1(const controls_view& v)
{
  return to_str(v.get_controller().get_key_bindings().get_key_for_action(1));
}

std::string get_key_str_for_action_2(const controls_view& v)
{
  return to_str(v.get_controller().get_key_bindings().get_key_for_action(2));
}

std::string get_key_str_for_action_3(const controls_view& v)
{
  return to_str(v.get_controller().get_key_bindings().get_key_for_action(3));
}

std::string get_key_str_for_action_4(const controls_view& v)
{
  return to_str(v.get_controller().get_key_bindings().get_key_for_action(4));
}

std::string get_key_str_for_move_down(const controls_view& v)
{
  return to_str(v.get_controller().get_key_bindings().get_key_for_move_down());
}
std::string get_key_str_for_move_left(const controls_view& v)
{
  return to_str(v.get_controller().get_key_bindings().get_key_for_move_left());
}
std::string get_key_str_for_move_right(const controls_view& v)
{
  return to_str(v.get_controller().get_key_bindings().get_key_for_move_right());
}
std::string get_key_str_for_move_up(const controls_view& v)
{
  return to_str(v.get_controller().get_key_bindings().get_key_for_move_up());
}

bool controls_view::process_events()
{
  // User interaction
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    if (event.type == sf::Event::Resized)
    {
      // From https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
      const sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
      m_window.setView(sf::View(visible_area));
      m_layout = controls_view_layout(
        screen_coordinat(event.size.width, event.size.height),
        get_default_margin_width()
      );
    }
    else if (event.type == sf::Event::Closed)
    {
      m_window.close();
      return true; // Menu is done
    }
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
        m_selected = get_previous(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Right)
      {
        m_selected = get_next(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Down)
      {
        m_selected = get_next(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Left)
      {
        m_selected = get_previous(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Space)
      {
        switch (m_selected)
        {
          case controls_view_item::type:
            m_controller.set_type(
              get_next(m_controller.get_type())
            );
          break;
          default:
            std::clog << "TODO\n";
        }
      }
      else if (key_pressed == sf::Keyboard::Key::Q)
      {
        m_window.close();
        return true;
      }
      else if (key_pressed == sf::Keyboard::Key::F3)
      {
        // debug
        std::clog << "Debug";
      }
    }
    /*
    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        const auto mouse_screen_pos{
          screen_coordinat(event.mouseButton.x, event.mouseButton.y)
        };
        if (is_in(mouse_screen_pos, m_layout.get_start())) exec_game();
        else if (is_in(mouse_screen_pos, m_layout.get_options())) exec_options();
        else if (is_in(mouse_screen_pos, m_layout.get_quit()))
        {
          m_window.close();
          return true;
        }
      }
    }
    */
  }
  return false; // Do not close the window :-)
}

void controls_view::set_text_style(sf::Text& text)
{
  text.setFont(get_font(get_resources()));
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(32);
  text.setFillColor(sf::Color::Black);
}
void controls_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  show_panels(*this);

  show_type_panel(*this);
  show_keyboard_panel(*this);
  //show_mouse_panel(*this);
  show_selected_panel(*this);

  // Display all shapes
  m_window.display();

}

void show_keyboard_panel(controls_view& v)
{
  const auto& layout = v.get_layout();
  std::vector<std::pair<screen_rect, std::string>> labels =
  {
    std::make_pair(layout.get_up_label(), "up"),
    std::make_pair(layout.get_right_label(), "right"),
    std::make_pair(layout.get_down_label(), "down"),
    std::make_pair(layout.get_left_label(), "left"),
    std::make_pair(layout.get_action_1_label(), "1"),
    std::make_pair(layout.get_action_2_label(), "2"),
    std::make_pair(layout.get_action_3_label(), "3"),
    std::make_pair(layout.get_action_4_label(), "4")
  };
  chess_color color{chess_color::black};
  for (const auto& p: labels)
  {
    const auto& screen_rect = p.first;
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), color)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString(p.second);
    set_text_position(text, screen_rect);
    v.get_window().draw(text);

    color = get_other_color(color);
  }

  std::vector<std::pair<screen_rect, std::string>> values =
  {
    std::make_pair(layout.get_up_value(), get_key_str_for_move_up(v)),
    std::make_pair(layout.get_right_value(), get_key_str_for_move_right(v)),
    std::make_pair(layout.get_down_value(), get_key_str_for_move_down(v)),
    std::make_pair(layout.get_left_value(), get_key_str_for_move_left(v)),
    std::make_pair(layout.get_action_1_value(), get_key_str_for_action_1(v)),
    std::make_pair(layout.get_action_2_value(), get_key_str_for_action_2(v)),
    std::make_pair(layout.get_action_3_value(), get_key_str_for_action_3(v)),
    std::make_pair(layout.get_action_4_value(), get_key_str_for_action_4(v))
  };
  color = get_other_color(color);
  for (const auto& p: values)
  {
    const auto& screen_rect = p.first;
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), color)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString(p.second);
    set_text_position(text, screen_rect);
    v.get_window().draw(text);

    color = get_other_color(color);
  }

}


void show_panels(controls_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    v.get_window().draw(rectangle);
  }
}

void show_selected_panel(controls_view& v)
{
  const auto select_rect{
    v.get_layout().get_selectable_rect(
      v.get_selected()
    )
  };
  sf::RectangleShape rectangle;
  set_rect(rectangle, select_rect);
  rectangle.setOrigin(
    get_width(select_rect) / 2,
    get_height(select_rect) / 2
  );
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(5);
  v.get_window().draw(rectangle);
}

void show_type_panel(controls_view& v)
{
  const auto& layout = v.get_layout();
  // game speed label
  {
    const auto& screen_rect = layout.get_controller_type_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::white)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString("Controller");
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  // game speed value
  {
    const auto& screen_rect = layout.get_controller_type_value();
    sf::RectangleShape rectangle;
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::black)
    );
    set_rect(rectangle, screen_rect);
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);

    text.setString(to_str(v.get_controller().get_type()));
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
}

#endif // LOGIC_ONLY
