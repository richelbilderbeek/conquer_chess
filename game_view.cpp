#include "game_view.h"

#include "game.h"
#include "game_view_layout.h"
#include "game_resources.h"
#include "screen_coordinat.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>

game_view::game_view(const game& game)
  : m_game{game}
{

}

void game_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      m_game.get_layout().get_window_size().get_x(),
      m_game.get_layout().get_window_size().get_y()
    ),
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
      const auto mouse_screen_pos{
        screen_coordinat(event.mouseMove.x, event.mouseMove.y)
      };
      //std::clog << "(" << mouse_screen_pos.get_x() << ", " << mouse_screen_pos.get_y() << ")\n";
      const auto mouse_game_pos{
        convert_to_game_coordinat(
          mouse_screen_pos,
          m_game.get_layout()
        )
      };
      //std::clog << "(" << mouse_game_pos.get_x() << ", " << mouse_game_pos.get_y() << ")\n";
      m_game.add_action(create_mouse_move_action(mouse_game_pos));
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
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

  // Show the sidebar: controls, units, debug
  show_sidebar();

  // Show the pieces' health bars
  show_unit_health_bars();

  // Show the mouse cursor
  show_mouse_cursor();

  // Display all shapes
  m_window.display();
}

void game_view::show_controls()
{
  const auto& layout = m_game.get_layout();
  sf::Text text;
  text.setFont(m_game_resources.get_font());
  std::stringstream s;
  const auto& selected_units = get_selected_pieces(m_game);
  if (selected_units.empty()) {
    s << "LMB: to select a unit";
  } else {
    s << "LMB: move to square\n"
      << "RMB: attack square"
    ;
  }
  text.setString(s.str());
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_tl_controls().get_x(),
    layout.get_tl_controls().get_y()
  );
  m_window.draw(text);
}

void game_view::show_debug()
{
  const auto& layout = m_game.get_layout();
  sf::Text text;
  text.setFont(m_game_resources.get_font());
  std::stringstream s;
  s << "Game position: ("
    << m_game.get_mouse_pos().get_x()
    << ", "
    << m_game.get_mouse_pos().get_y()
    << ")"
    << '\n'
    << "Screen position: ("
    << convert_to_screen_coordinat(m_game.get_mouse_pos(), layout).get_x()
    << ", "
    << convert_to_screen_coordinat(m_game.get_mouse_pos(), layout).get_y()
    << ")"  ;
  text.setString(s.str());
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_tl_debug().get_x(),
    layout.get_tl_debug().get_y()
  );
  m_window.draw(text);
}

void game_view::show_mouse_cursor()
{
  const auto& layout = m_game.get_layout();
  sf::CircleShape cursor;
  cursor.setRadius(16.0);
  cursor.setFillColor(sf::Color::Transparent);
  cursor.setOutlineColor(sf::Color::Black);
  cursor.setOutlineThickness(2.0);
  cursor.setScale(1.0, 1.0);
  cursor.setOrigin(16.0, 16.0);
  const screen_coordinat cursor_pos{
    convert_to_screen_coordinat(
      m_game.get_mouse_pos(),
      layout
    )
  };
  cursor.setPosition(
    cursor_pos.get_x(),
    cursor_pos.get_y()
  );
  cursor.setRotation(0.0);
  m_window.draw(cursor);
}

void game_view::show_pieces()
{
  const auto& layout = m_game.get_layout();
  const double square_width{layout.get_square_width()};
  const double square_height{layout.get_square_height()};
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
    if (piece.is_selected())
    {
      sprite.setFillColor(sf::Color::Red);
    }
    sprite.setOrigin(0.0, 0.0);
    const auto screen_position = convert_to_screen_coordinat(piece.get_coordinat(), layout);
    sprite.setPosition(
      screen_position.get_x(),
      screen_position.get_y()
    );
    sprite.setRotation(0.0);
    m_window.draw(sprite);
  }
}

void game_view::show_sidebar()
{
  show_controls();
  show_unit_sprites();
  show_debug();
}

void game_view::show_squares()
{
  const auto& layout = m_game.get_layout();
  const double square_width{layout.get_square_width()};
  const double square_height{layout.get_square_height()};

  sf::RectangleShape black_square;
  black_square.setSize(sf::Vector2f(square_width + 1, square_height + 1));
  black_square.setTexture(&m_game_resources.get_black_square());
  black_square.setOrigin(0.0, 0.0);
  black_square.setPosition(100.0, 200.0);
  black_square.setRotation(0.0);

  sf::RectangleShape white_square;
  white_square.setSize(sf::Vector2f(square_width + 1, square_height + 1));
  white_square.setTexture(&m_game_resources.get_white_square());
  white_square.setOrigin(0.0, 0.0);
  white_square.setPosition(100.0, 200.0);
  white_square.setRotation(0.0);

  for (int x = 0; x != 8; ++x)
  {
    for (int y = 0; y != 8; ++y)
    {
      sf::RectangleShape& s = (x + y) % 2 == 1 ? black_square : white_square;
      const screen_coordinat square_pos{
        convert_to_screen_coordinat(
          game_coordinat(x, y),
          layout
        )
      };
      s.setPosition(square_pos.get_x(), square_pos.get_y());
      m_window.draw(s);
    }
  }
}

void game_view::show_unit_health_bars()
{
  const auto& layout = m_game.get_layout();
  for (const auto& piece: m_game.get_pieces())
  {
    // Black box around it
    sf::RectangleShape black_box;

    black_box.setSize(sf::Vector2f(layout.get_square_width() - 4.0, 16.0 - 4.0));
    //black_box.setScale(1.0, 1.0);
    black_box.setFillColor(sf::Color(0, 0, 0));
    black_box.setOrigin(0.0, 0.0);
    const auto black_box_pos = convert_to_screen_coordinat(
      piece.get_coordinat(),
      layout
    );
    black_box.setPosition(
      2.0 + black_box_pos.get_x(),
      2.0 + black_box_pos.get_y()
    );
    black_box.setRotation(0.0);
    m_window.draw(black_box);

    // Health
    sf::RectangleShape health_bar;
    health_bar.setSize(sf::Vector2f(layout.get_square_width() - 8.0, 16.0 - 8.0));
    //health_bar.setScale(1.0, 1.0);
    health_bar.setFillColor(
      sf::Color(
        static_cast<sf::Uint8>(piece.get_f_health() * 255.0),
        static_cast<sf::Uint8>(piece.get_f_health() * 255.0),
        static_cast<sf::Uint8>(piece.get_f_health() * 255.0)
      )
    );
    health_bar.setOrigin(0.0, 0.0);
    const auto health_bar_pos = convert_to_screen_coordinat(piece.get_coordinat(), layout);
    health_bar.setPosition(
      4.0 + health_bar_pos.get_x(),
      4.0 + health_bar_pos.get_y()
    );
    health_bar.setRotation(0.0);
    m_window.draw(health_bar);
  }
}

void game_view::show_unit_paths()
{
  const auto& layout = m_game.get_layout();
  for (const auto& piece: get_pieces(m_game))
  {
    if (is_idle(piece)) continue;
    const auto piece_pos{
      convert_to_screen_coordinat(piece.get_coordinat(), layout)
    };
    const auto target_pos{
      convert_to_screen_coordinat(
        piece.get_actions().front().get_coordinat(),
        layout
      )
    };
    sf::VertexArray line(sf::LinesStrip, 2);
    line[0].position = sf::Vector2f(piece_pos.get_x(), piece_pos.get_y());
    line[0].color  = sf::Color::Red;
    line[1].position = sf::Vector2f(target_pos.get_x(), target_pos.get_y());
    line[1].color = sf::Color::Red;
    m_window.draw(line);
  }
}

void game_view::show_unit_sprites()
{
  const auto& layout = m_game.get_layout();
  const double square_width{layout.get_square_width()};
  const double square_height{layout.get_square_height()};
  screen_coordinat screen_position = layout.get_tl_units();
  for (const auto& piece: get_selected_pieces(m_game))
  {
    // sprite of the piece
    sf::RectangleShape sprite;
    sprite.setSize(sf::Vector2f(square_width, square_height));
    sprite.setTexture(
      &m_game_resources.get_piece_portrait(
        piece.get_color(),
        piece.get_type()
      )
    );
    sprite.setOrigin(0.0, 0.0);
    sprite.setPosition(
      screen_position.get_x(),
      screen_position.get_y()
    );
    sprite.setRotation(0.0);
    m_window.draw(sprite);
    // text
    sf::Text text;
    text.setFont(m_game_resources.get_font());
    std::stringstream s;

    s << piece.get_type() << ": "
      << piece.get_health() << "/"
      << piece.get_max_health()
    ;
    text.setString(s.str());
    text.setCharacterSize(20);
    const auto text_position{
      screen_position + screen_coordinat(square_width + 10, square_height / 2)
    };
    text.setPosition(
      text_position.get_x(),
      text_position.get_y()
    );
    m_window.draw(text);
    screen_position += screen_coordinat(0, square_height);
  }
}

void test_game_view() //!OCLINT tests may be many
{
  #ifndef NDEBUG // no tests in release
  #endif //NDEBUG
}
