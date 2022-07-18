#include "options_view.h"

#ifndef LOGIC_ONLY

#include "options_view.h"

#include "screen_coordinat.h"
#include "game_view.h"
#include <iostream>

options_view::options_view()
{

}

void options_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_screen_size().get_x(),
      get_default_screen_size().get_y()
    ),
    "Conquer Chess: options menu"
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

bool options_view::process_events()
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
      else if (key_pressed == sf::Keyboard::Key::M)
      {
        //m_game.add_action(create_press_move_action());
      }
      else if (key_pressed == sf::Keyboard::Key::A)
      {
        //m_game.add_action(create_press_attack_action());
      }
      else if (key_pressed == sf::Keyboard::Key::F3)
      {
        // debug
        std::clog << "Debug";
      }
      else if (key_pressed == sf::Keyboard::Key::F4)
      {
        // debug
        game_view view;
        view.exec();
      }
      else if (key_pressed == sf::Keyboard::Key::F5)
      {
        // debug
        options_view view;
        view.exec();
      }
    }
    if (event.type == sf::Event::MouseMoved)
    {
      /*
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
      */
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
      /*
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
      */
    }
    else if (event.type == sf::Event::KeyReleased)
    {
      // Maybe a player input?
      // Nothing yet
    }
  }
  return false; // if no events proceed with tick
}

void options_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setPosition(100, 100);
  rectangle.setSize(sf::Vector2f(400, 200));
  rectangle.setFillColor(sf::Color::Black);
  rectangle.setOutlineThickness(2);
  rectangle.setOutlineColor(sf::Color::White);
  m_window.draw(rectangle);

  // Display all shapes
  m_window.display();

}

#endif // LOGIC_ONLY
