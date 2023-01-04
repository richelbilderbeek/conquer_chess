#include "loading_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinat.h"
#include "sfml_helper.h"
#include "menu_view.h"
#include <cassert>
#include <cmath>
#include <iostream>

loading_view::loading_view()
{

}

void loading_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_loading_screen_size().get_x(),
      get_default_loading_screen_size().get_y()
    ),
    "Conquer Chess: loading data"
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
    // Process user input
    const bool must_quit{process_events()};
    if (must_quit) return;

    if (m_resource_loader.is_done())
    {
      exec_menu();
    }
    else
    {
      m_resource_loader.process_next(m_resources);
    }

    // Show the new state
    show();

    sf::sleep(sf::seconds(0.1));

  }
}

void loading_view::exec_menu()
{
  m_window.setVisible(false);
  menu_view v(
    create_default_game_options(),
    create_two_keyboard_controllers()
  );
  v.exec();
  m_window.close();
}

bool loading_view::process_events()
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
    }
  }
  return false; // Do not close the window :-)
}

void loading_view::set_text_style(sf::Text& text)
{
  text.setFont(get_arial_font(get_resources()));
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(44);
  text.setFillColor(sf::Color::White);
}

void loading_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  const auto window_size{m_window.getSize()};

  const screen_rect window_rect{
    screen_coordinat(0, 0),
    screen_coordinat(
      window_size.x, window_size.y
    )
  };
  // background
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, window_rect);
    rectangle.setTexture(
      &m_resources.get_textures().get_all_races_1()
    );
    get_window().draw(rectangle);
  }
  // loading text
  {
    sf::Text text;
    const auto text_rect{
      get_upper_half(get_lower_eighth(window_rect))
    };
    set_text_style(text);
    text.setString(m_resource_loader.get_current());
    set_text_position(text, text_rect);
    get_window().draw(text);
  }
  // progress bar rectangle background
  {
    sf::RectangleShape rectangle;
    const auto progress_bar_rect{
      get_lower_half(get_lower_eighth(window_rect))
    };
    set_rect(rectangle, progress_bar_rect);
    rectangle.setFillColor(sf::Color::White);
    get_window().draw(rectangle);
  }
  // progress bar rectangle
  {
    sf::RectangleShape rectangle;
    const auto full_progress_bar_rect{
      get_lower_half(get_lower_eighth(window_rect))
    };
    const double f_progress{get_progress(m_resource_loader)};
    const screen_rect progress_bar_rect{
      screen_rect(
        full_progress_bar_rect.get_tl(),
        screen_coordinat(
          full_progress_bar_rect.get_tl().get_x()
          + (f_progress * get_width(full_progress_bar_rect)),
          full_progress_bar_rect.get_br().get_y()
        )
      )
    };
    set_rect(rectangle, progress_bar_rect);
    rectangle.setFillColor(sf::Color::Black);
    get_window().draw(rectangle);
  }

  // Display all shapes
  m_window.display();

}

#endif // LOGIC_ONLY
