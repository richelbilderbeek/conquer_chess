#include "game_view.h"

#include "game.h"
#include "game_view_layout.h"
#include "game_resources.h"
#include "screen_coordinat.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cmath>
#include <string>
#include <sstream>

game_view::game_view(const screen_coordinat& screen_size)
  : m_layout{screen_size}
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
    if (event.type == sf::Event::MouseMoved)
    {
      m_game.add_action(
        create_mouse_move_action(
          screen_coordinat(
            event.mouseButton.x,
            event.mouseButton.y
          )
        )
      );

    }
    else if (event.type == sf::Event::MouseButtonPressed)
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

  // Show the mouse cursor
  show_mouse_cursor();

  // Display all shapes
  m_window.display();
}

void game_view::show_health_bars()
{
  for (const auto& piece: m_game.get_pieces())
  {
    // Black box around it
    sf::RectangleShape black_box;

    black_box.setSize(sf::Vector2f(m_layout.get_square_width() - 4.0 - 4.0, 16.0 - 4.0));
    //black_box.setScale(1.0, 1.0);
    black_box.setFillColor(sf::Color(0, 0, 0));
    black_box.setOrigin(0.0, 0.0);
    const auto black_box_pos = convert_to_screen_coordinat(piece.get_coordinat());
    black_box.setPosition(
      2.0 + black_box_pos.get_x(),
      2.0 + black_box_pos.get_y()
    );
    black_box.setRotation(0.0);
    m_window.draw(black_box);

    // Health
    sf::RectangleShape health_bar;
    health_bar.setSize(sf::Vector2f(m_layout.get_square_width() - 8.0 - 4.0, 16.0 - 8.0));
    //health_bar.setScale(1.0, 1.0);
    health_bar.setFillColor(
      sf::Color(
        static_cast<sf::Uint8>(piece.get_f_health() * 255.0),
        static_cast<sf::Uint8>(piece.get_f_health() * 255.0),
        static_cast<sf::Uint8>(piece.get_f_health() * 255.0)
      )
    );
    health_bar.setOrigin(0.0, 0.0);
    const auto health_bar_pos = convert_to_screen_coordinat(piece.get_coordinat());
    health_bar.setPosition(
      4.0 + health_bar_pos.get_x(),
      4.0 + health_bar_pos.get_y()
    );
    health_bar.setRotation(0.0);
    m_window.draw(health_bar);
  }
}

void game_view::show_mouse_cursor()
{
  sf::CircleShape cursor;
  cursor.setRadius(16.0);
  cursor.setFillColor(sf::Color::Transparent);
  cursor.setOutlineColor(sf::Color::Black);
  cursor.setScale(1.0, 1.0);
  cursor.setOrigin(0.0, 0.0);
  const screen_coordinat cursor_pos{
    convert_to_screen_coordinat(
      m_game.get_mouse_pos()
    )
  };

  cursor.setPosition(
    cursor_pos.get_x(),
    cursor_pos.get_y()
  );
  cursor.setRotation(0.0);
  m_window.draw(cursor);
}

screen_coordinat game_view::convert_to_screen_coordinat(const game_coordinat& coordinat) const
{
  const auto tl_board{m_layout.get_tl_board()};
  const auto br_board{m_layout.get_br_board()};
  const double square_width{
    static_cast<double>(br_board.get_x() - tl_board.get_x()) / 8.0
  };
  const double square_height{
    static_cast<double>(br_board.get_y() - tl_board.get_y()) / 8.0
  };
  return screen_coordinat(
    tl_board.get_x() + (square_width * coordinat.get_x()),
    br_board.get_y() - square_height - (square_height * coordinat.get_y()) // The first rank (e.g. with the white king) is at the bottom
  );
}

void game_view::show_pieces()
{
  const double square_width{m_layout.get_square_width()};
  const double square_height{m_layout.get_square_height()};
  for (const auto& piece: m_game.get_pieces())
  {
    sf::RectangleShape sprite;
    sprite.setSize(sf::Vector2f(square_width, square_height));
    sprite.setTexture(
      &m_game_resources.get_piece(
        piece.get_color(),
        piece.get_type()
      )
    );
    sprite.setOrigin(0.0, 0.0);
    const auto screen_position = convert_to_screen_coordinat(piece.get_coordinat());
    sprite.setPosition(
      screen_position.get_x(),
      screen_position.get_y()
    );
    sprite.setRotation(0.0);
    m_window.draw(sprite);
  }
}

void game_view::show_squares()
{
  const double square_width{m_layout.get_square_width()};
  const double square_height{m_layout.get_square_height()};

  sf::RectangleShape black_square;
  black_square.setSize(sf::Vector2f(square_width, square_height));
  black_square.setTexture(&m_game_resources.get_black_square());
  black_square.setOrigin(0.0, 0.0);
  black_square.setPosition(100.0, 200.0);
  black_square.setRotation(0.0);

  sf::RectangleShape white_square;
  white_square.setSize(sf::Vector2f(square_width, square_height));
  white_square.setTexture(&m_game_resources.get_white_square());
  white_square.setOrigin(0.0, 0.0);
  white_square.setPosition(100.0, 200.0);
  white_square.setRotation(0.0);

  for (int x = 0; x != 8; ++x)
  {
    for (int y = 0; y != 8; ++y)
    {
      sf::RectangleShape& s = (x + y) % 2 == 1 ? black_square : white_square;
      s.setPosition(square_width * x, square_height * y);
      m_window.draw(s);
    }
  }
}

void test_game_view() //!OCLINT tests may be many
{
  #ifndef NDEBUG // no tests in release
  #endif //NDEBUG
}
