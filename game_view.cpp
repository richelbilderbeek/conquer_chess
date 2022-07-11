#include "game_view.h"

#include "game.h"
#include "game_resources.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>
#include <cmath>
#include <string>
#include <sstream>

game_view::game_view()
{

}

void game_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(1000, 600),
    "knokchess"
  );
  while (m_window.isOpen())
  {
    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Do a tick
    m_game.tick();

    // Show thw new state
    show();
  }
}

bool game_view::process_events()
{
  // User interaction
  sf::Event event;
  while (m_window.pollEvent(event))
  {
    if (event.type == sf::Event::Closed)
    {
        m_window.close();
        return true; // Game is done
    }
    if (event.type == sf::Event::KeyPressed)
    {
      sf::Keyboard::Key key_pressed = event.key.code;
      if (key_pressed == sf::Keyboard::Key::Escape)
      {
        m_window.close();
        return true;
      }
      // Maybe a player input?
      // Nothing yet
    }
    else if (event.type == sf::Event::KeyReleased)
    {
      // Maybe a player input?
      // Nothing yet
    }
  }
  return false; // if no events proceed with tick
}

void game_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  sf::RectangleShape square;
  square.setSize(sf::Vector2f(100.0, 100.0));
  square.setScale(1.0, 1.0);
  square.setFillColor(sf::Color(255, 128, 0));
  square.setTexture(&m_game_resources.get_white_square());
  square.setOrigin(0.0, 0.0);
  square.setPosition(100.0, 200.0);
  square.setRotation(0.0);

  m_window.draw(square);

  // Display all shapes
  m_window.display();
}

void test_game_view() //!OCLINT tests may be many
{
  #ifndef NDEBUG // no tests in release
  #endif //NDEBUG
}
