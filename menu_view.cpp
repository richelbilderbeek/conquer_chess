#include "menu_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinat.h"
#include "game_view.h"
#include "options_view.h"
#include <iostream>

menu_view::menu_view()
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

      m_layout = menu_view_layout(
        screen_coordinat(visibleArea.width, visibleArea.height),
        get_default_margin_width()
      );
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

  show_panels(*this);

  show_title_panel(*this);
  show_subtitle_panel(*this);
  show_start_panel(*this);
  show_options_panel(*this);
  show_about_panel(*this);
  show_quit_panel(*this);

  // Display all shapes
  m_window.display();

}

void show_about_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_about()};
  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      get_width(screen_rect),
      get_height(screen_rect)
    )
  );
  rectangle.setTexture(
    &get_about(v.get_resources())
  );
  rectangle.setPosition(
    screen_rect.get_tl().get_x(),
    screen_rect.get_tl().get_y()
  );
  v.get_window().draw(rectangle);
}

void show_options_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_options()};
  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      get_width(screen_rect),
      get_height(screen_rect)
    )
  );
  rectangle.setTexture(
    &get_options(v.get_resources())
  );
  rectangle.setPosition(
    screen_rect.get_tl().get_x(),
    screen_rect.get_tl().get_y()
  );
  v.get_window().draw(rectangle);
}

void show_panels(menu_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    rectangle.setOrigin(0.0, 0.0);
    rectangle.setSize(
      sf::Vector2f(
        get_width(screen_rect),
        get_height(screen_rect)
      )
    );
    rectangle.setFillColor(sf::Color(32, 32, 32));
    //rectangle.setOutlineThickness(1);
    //rectangle.setOutlineColor(sf::Color::White);
    rectangle.setPosition(
      screen_rect.get_tl().get_x(),
      screen_rect.get_tl().get_y()
    );
    v.get_window().draw(rectangle);
  }
}

void show_quit_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_quit()};
  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      get_width(screen_rect),
      get_height(screen_rect)
    )
  );
  rectangle.setTexture(
    &get_quit(v.get_resources())
  );
  rectangle.setPosition(
    screen_rect.get_tl().get_x(),
    screen_rect.get_tl().get_y()
  );
  v.get_window().draw(rectangle);
}

void show_start_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_start()};
  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      get_width(screen_rect),
      get_height(screen_rect)
    )
  );
  rectangle.setTexture(
    &get_start(v.get_resources())
  );
  rectangle.setPosition(
    screen_rect.get_tl().get_x(),
    screen_rect.get_tl().get_y()
  );
  v.get_window().draw(rectangle);
}

void show_subtitle_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_subtitle()};
  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      get_width(screen_rect),
      get_height(screen_rect)
    )
  );
  rectangle.setTexture(
    &get_subtitle(v.get_resources())
  );
  rectangle.setPosition(
    screen_rect.get_tl().get_x(),
    screen_rect.get_tl().get_y()
  );
  v.get_window().draw(rectangle);
}

void show_title_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_title()};
  sf::RectangleShape rectangle;
  rectangle.setOrigin(0.0, 0.0);
  rectangle.setSize(
    sf::Vector2f(
      get_width(screen_rect),
      get_height(screen_rect)
    )
  );
  rectangle.setTexture(
    &get_title(v.get_resources())
  );
  rectangle.setPosition(
    screen_rect.get_tl().get_x(),
    screen_rect.get_tl().get_y()
  );
  v.get_window().draw(rectangle);
}

#endif // LOGIC_ONLY
