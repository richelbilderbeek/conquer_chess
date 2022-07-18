#include "menu_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinat.h"
#include "game_view.h"
#include "options_view.h"
#include <iostream>

menu_view::menu_view()
  : m_margin_size{10}
{

}

void menu_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_screen_size().get_x(),
      get_default_screen_size().get_y()
    ),
    "Conquer Chess: main menu"
  );
  while (m_window.isOpen())
  {
    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Do a tick
    //m_game.tick();

    // Show the new state
    show();

  }
}

bool menu_view::process_events()
{
  // User interaction
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    if (event.type == sf::Event::Resized)
    {
      // From https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
      const sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
      m_window.setView(sf::View(visibleArea));
      /*
      resize_window(
        m_game,
        screen_coordinat(
          static_cast<int>(event.size.width),
          static_cast<int>(event.size.height)
        ),
        get_default_margin_width()
      );
      */
    }
    else if (event.type == sf::Event::Closed)
    {
        m_window.close();
        return true; // Game is done
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
        //m_game.add_action(create_press_up_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Right)
      {
        //m_game.add_action(create_press_right_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Down)
      {
        //m_game.add_action(create_press_down_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Left)
      {
        //m_game.add_action(create_press_left_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Space)
      {
        //m_game.add_action(create_press_select_action());
      }
      else if (key_pressed == sf::Keyboard::Key::O)
      {
        options_view view;
        view.exec();
      }
      else if (key_pressed == sf::Keyboard::Key::S)
      {
        game_view view;
        view.exec();
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
      else if (key_pressed == sf::Keyboard::Key::F4)
      {
        game_view view;
        view.exec();
      }
      else if (key_pressed == sf::Keyboard::Key::F5)
      {
        options_view view;
        view.exec();
      }
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
      /*
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
      */
    }
  }
  return false; // Do not close the window :-)
}

void menu_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  show_panel_1(*this);
  show_panel_2(*this);
  show_panel_3(*this);
  show_panel_4(*this);

  // Display all shapes
  m_window.display();

}

void show_panel_1(menu_view& v)
{
  const int height{static_cast<int>(v.get_window().getSize().y)};
  const int width{static_cast<int>(v.get_window().getSize().x)};
  const int margin{v.get_margin_size()};

  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      width - (2 * margin),
      (height  - (5 * margin)) / 4
    )
  );
  rectangle.setFillColor(sf::Color::Black);
  rectangle.setOutlineThickness(1);
  rectangle.setOutlineColor(sf::Color::White);
  rectangle.setPosition(margin, margin);
  v.get_window().draw(rectangle);

  sf::Text text;
  text.setFont(v.get_resources().get_font());
  text.setString(" Conquer Chess ");
  text.setCharacterSize((height  - (5 * margin)) / 4);
  text.setOrigin(0.0, 0.0);
  text.setFillColor(sf::Color::Red);
  v.get_window().draw(text);
}

void show_panel_2(menu_view& v)
{
  const int height{static_cast<int>(v.get_window().getSize().y)};
  const int width{static_cast<int>(v.get_window().getSize().x)};
  const int margin{v.get_margin_size()};

  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      width - (2 * margin),
      (height  - (5 * margin)) / 4
    )
  );
  rectangle.setFillColor(sf::Color::Black);
  rectangle.setOutlineThickness(1);
  rectangle.setOutlineColor(sf::Color::White);
  rectangle.setPosition(margin, margin + (1 * height / 4));
  v.get_window().draw(rectangle);

  sf::Text text;
  text.setFont(v.get_resources().get_font());
  text.setString(" (S)tart ");
  text.setCharacterSize(((height  - (5 * margin)) / 4) * 8 / 10);
  text.setOrigin(0.0, 0.0);
  text.setFillColor(sf::Color::Red);
  text.setPosition(rectangle.getPosition());
  v.get_window().draw(text);
}

void show_panel_3(menu_view& v)
{
  const int height{static_cast<int>(v.get_window().getSize().y)};
  const int width{static_cast<int>(v.get_window().getSize().x)};
  const int margin{v.get_margin_size()};

  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      width - (2 * margin),
      (height  - (5 * margin)) / 4
    )
  );
  rectangle.setFillColor(sf::Color::Black);
  rectangle.setOutlineThickness(1);
  rectangle.setOutlineColor(sf::Color::White);

  rectangle.setPosition(margin, margin + (2 * height / 4));
  v.get_window().draw(rectangle);

  sf::Text text;
  text.setFont(v.get_resources().get_font());
  text.setString(" (O)ptions ");
  text.setCharacterSize(((height  - (5 * margin)) / 4) * 8 / 10);
  text.setOrigin(0.0, 0.0);
  text.setFillColor(sf::Color::Red);
  text.setPosition(rectangle.getPosition());
  v.get_window().draw(text);
}

void show_panel_4(menu_view& v)
{
  const int height{static_cast<int>(v.get_window().getSize().y)};
  const int width{static_cast<int>(v.get_window().getSize().x)};
  const int margin{v.get_margin_size()};

  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      width - (2 * margin),
      (height  - (5 * margin)) / 4
    )
  );
  rectangle.setFillColor(sf::Color::Black);
  rectangle.setOutlineThickness(1);
  rectangle.setOutlineColor(sf::Color::White);

  rectangle.setPosition(margin, margin + (3 * height / 4));
  v.get_window().draw(rectangle);

  sf::Text text;
  text.setFont(v.get_resources().get_font());
  text.setString(" (Q)uit ");
  text.setCharacterSize(((height  - (5 * margin)) / 4) * 8 / 10);
  text.setOrigin(0.0, 0.0);
  text.setFillColor(sf::Color::Red);
  text.setPosition(rectangle.getPosition());
  v.get_window().draw(text);
}

#endif // LOGIC_ONLY
