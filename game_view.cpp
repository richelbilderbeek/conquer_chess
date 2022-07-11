#include "game_view.h"

#include "game.h"
#include "game_resources.h"
#include "screen_coordinat.h"
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
    sf::VideoMode(128 * 8, 128 * 8),
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
    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        m_game.add_action(
          create_press_lmb_action(
            screen_coordinat(
              event.mouseButton.x,
              event.mouseButton.y
            )
          )
        );
      }
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

  // Show only the squares
  show_squares();

  // Show only the pieces
  show_pieces();

  // Show the pieces' health bars
  show_health_bars();

  // Display all shapes
  m_window.display();
}

void game_view::show_health_bars()
{
  for (const auto& piece: m_game.get_pieces())
  {
    // Black box around it
    sf::RectangleShape black_box;
    black_box.setSize(sf::Vector2f(128.0 - 4.0, 16.0 - 4.0));
    black_box.setScale(1.0, 1.0);
    black_box.setFillColor(sf::Color(0, 0, 0));
    black_box.setOrigin(0.0, 0.0);
    black_box.setPosition(
      2.0 + (128.0 * piece.get_x()),
      2.0 + ((128.0 * 7.0) - (128.0 * piece.get_y())) // The first rank (e.g. with the white king) is at the bottom
    );
    black_box.setRotation(0.0);
    m_window.draw(black_box);

    // Health
    sf::RectangleShape health_bar;
    health_bar.setSize(sf::Vector2f(128.0 - 8.0, 16.0 - 8.0));
    health_bar.setScale(1.0, 1.0);
    health_bar.setFillColor(
      sf::Color(
        static_cast<sf::Uint8>(piece.get_f_health() * 255.0),
        static_cast<sf::Uint8>(piece.get_f_health() * 255.0),
        static_cast<sf::Uint8>(piece.get_f_health() * 255.0)
      )
    );
    health_bar.setOrigin(0.0, 0.0);
    health_bar.setPosition(
      4.0 + (128.0 * piece.get_x()),
      4.0 + ((128.0 * 7.0) - (128.0 * piece.get_y())) // The first rank (e.g. with the white king) is at the bottom
    );
    health_bar.setRotation(0.0);
    m_window.draw(health_bar);
  }
}

void game_view::show_pieces()
{
  for (const auto& piece: m_game.get_pieces())
  {
    sf::RectangleShape sprite;
    sprite.setSize(sf::Vector2f(128.0, 128.0));
    sprite.setScale(1.0, 1.0);
    sprite.setTexture(
      &m_game_resources.get_piece(
        piece.get_color(),
        piece.get_type()
      )
    );
    sprite.setOrigin(0.0, 0.0);
    sprite.setPosition(
      128.0 * piece.get_x(),
      (128.0 * 7.0) - (128.0 * piece.get_y()) // The first rank (e.g. with the white king) is at the bottom
    );
    sprite.setRotation(0.0);
    m_window.draw(sprite);
  }
}

void game_view::show_squares()
{
  sf::RectangleShape black_square;
  black_square.setSize(sf::Vector2f(128.0, 128.0));
  black_square.setScale(1.0, 1.0);
  black_square.setTexture(&m_game_resources.get_black_square());
  black_square.setOrigin(0.0, 0.0);
  black_square.setPosition(100.0, 200.0);
  black_square.setRotation(0.0);

  sf::RectangleShape white_square;
  white_square.setSize(sf::Vector2f(128.0, 128.0));
  white_square.setScale(1.0, 1.0);
  white_square.setTexture(&m_game_resources.get_white_square());
  white_square.setOrigin(0.0, 0.0);
  white_square.setPosition(100.0, 200.0);
  white_square.setRotation(0.0);

  for (int x = 0; x != 8; ++x)
  {
    for (int y = 0; y != 8; ++y)
    {
      sf::RectangleShape& s = (x + y) % 2 == 1 ? black_square : white_square;
      s.setPosition(128.0 * x, 128.0 * y);
      m_window.draw(s);
    }
  }
}

void test_game_view() //!OCLINT tests may be many
{
  #ifndef NDEBUG // no tests in release
  #endif //NDEBUG
}
