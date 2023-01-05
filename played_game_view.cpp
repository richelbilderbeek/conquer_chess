#include "played_game_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinat.h"

#include "pieces.h"
#include "sfml_helper.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

played_game_view::played_game_view(const game& g)
  : m_game{g}
{

}

void played_game_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_played_game_screen_size().get_x(),
      get_default_played_game_screen_size().get_y()
    ),
    "Conquer Chess: played game"
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

    show();

  }
}

bool played_game_view::process_events()
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

      m_layout = played_game_view_layout(
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
      else if (key_pressed == sf::Keyboard::Key::F2)
      {
        std::ofstream file("replay.pgn");
        file << to_pgn(m_game) << '\n';
        m_resources.play(message(message_type::done, chess_color::white, piece_type::king));
      }
    }
  }
  return false; // Do not close the window :-)
}

void played_game_view::set_text_style(sf::Text& text)
{
  text.setFont(get_code_squared_font(get_resources()));
  //text.setStyle(sf::Text::Bold);
  text.setCharacterSize(m_layout.get_font_size());
  text.setFillColor(sf::Color::White);
}

void played_game_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  show_layout_panels(*this);

  show_text_panel(*this);

  // Display all shapes
  m_window.display();

}

void show_layout_panels(played_game_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    v.get_window().draw(rectangle);
  }
}

void show_text_panel(played_game_view& v)
{
  const auto& g{v.get_game()};
  const auto screen_rect{v.get_layout().get_text()};
  std::stringstream s;
  s << to_pgn(g);
  if (s.str().empty()) s << "[none]";
  sf::Text text;
  text.setString(s.str().c_str());
  v.set_text_style(text);
  set_text_position(text, screen_rect);
  v.get_window().draw(text);
}

#endif // LOGIC_ONLY
