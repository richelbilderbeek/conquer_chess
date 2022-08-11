#include "about_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinat.h"

#include "pieces.h"
#include "sfml_helper.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>

about_view::about_view()
{

}

void about_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_about_screen_size().get_x(),
      get_default_about_screen_size().get_y()
    ),
    "Conquer Chess: about"
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
    m_rotation = 5.0 * std::sin(m_clock.getElapsedTime().asMilliseconds() / 1000.0);

    show();

  }
}

bool about_view::process_events()
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

      m_layout = about_view_layout(
        screen_coordinat(event.size.width, event.size.height),
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
    }
  }
  return false; // Do not close the window :-)
}

void about_view::set_text_style(sf::Text& text)
{
  text.setFont(get_font(get_resources()));
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(m_layout.get_font_size());
  text.setFillColor(sf::Color::Black);
}

void about_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  show_panels(*this);

  show_title_panel(*this);
  show_subtitle_panel(*this);
  show_text_panel(*this);

  // Display all shapes
  m_window.display();

}

void show_panels(about_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    v.get_window().draw(rectangle);
  }
}

void show_subtitle_panel(about_view& v)
{
  const auto screen_rect{v.get_layout().get_subtitle()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_subtitle(v.get_resources())
  );
  v.get_window().draw(rectangle);
}

void show_text_panel(about_view& v)
{
  const auto screen_rect{v.get_layout().get_text()};
  show_squares(
    v.get_window(),
    screen_rect,
    v.get_resources()
  );
  /*
  show_pieces(
    get_standard_starting_pieces(),
    v.get_window(),
    screen_rect,
    v.get_resources(),
    false
  );
  */

  /*
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_strip(v.get_resources(), chess_color::white)
  );
  v.get_window().draw(rectangle);
  */

  std::stringstream s;
  s
    << "Conquer Chess\n"
    << "\n"
    << "(C) 2022 Richel Bilderbeek\n"
    << "\n"
    << "https://github.com/\n"
    << "  richelbilderbeek/\n"
    << "  conquer_chess\n"
    << "\n"
    << "Contributors:\n"
    << "\n"
    << " * Johanna Soederstroem\n"
    << " * Leonid Rutkowski\n"
  ;
  sf::Text text;
  text.setString(s.str().c_str());
  v.set_text_style(text);
  set_text_position(text, screen_rect);
  text.setFillColor(sf::Color::Black);
  text.setRotation(v.get_rotation());
  v.get_window().draw(text);
}

void show_title_panel(about_view& v)
{
  const auto screen_rect{v.get_layout().get_title()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_title(v.get_resources())
  );
  v.get_window().draw(rectangle);
}

#endif // LOGIC_ONLY
