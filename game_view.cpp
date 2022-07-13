#include "game_view.h"

#include "game.h"
#include "game_view_layout.h"
#include "game_resources.h"
#include "screen_coordinat.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

game_view::game_view(const game& game)
  : m_game{game}
{

}

std::string bool_to_str(const bool b) noexcept
{
  return b ? "true" : "false";
}

sf::RectangleShape create_black_square(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const double square_width{layout.get_square_width()};
  const double square_height{layout.get_square_height()};

  sf::RectangleShape black_square;
  black_square.setSize(sf::Vector2f(square_width + 1, square_height + 1));
  black_square.setTexture(&view.get_game_resources().get_black_square());
  black_square.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
  return black_square;
}

sf::RectangleShape create_white_square(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const double square_width{layout.get_square_width()};
  const double square_height{layout.get_square_height()};

  sf::RectangleShape white_square;
  white_square.setSize(sf::Vector2f(square_width + 1, square_height + 1));
  white_square.setTexture(&view.get_game_resources().get_white_square());
  white_square.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
  return white_square;
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

    // Show the new state
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
      if (key_pressed == sf::Keyboard::Key::F3)
      {
        // debug
        std::clog << "Debug";
      }
      if (key_pressed == sf::Keyboard::Key::F4)
      {
        toggle_player(*this);
      }
    }
    if (event.type == sf::Event::MouseMoved)
    {
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

  // Show the board: squares, unit paths, pieces, health bars
  show_board(*this);

  // Show the sidebar: controls, units, debug
  show_sidebar(*this);

  // Show the mouse cursor
  show_mouse_cursor();

  // Display all shapes
  m_window.display();
}

void show_board(game_view& view)
{
  show_squares(view);
  show_unit_paths(view);
  show_pieces(view);
  show_unit_health_bars(view);
}

void show_controls(game_view& view)
{
  const auto& layout = view.get_game().get_layout();
  sf::Text text;
  text.setFont(view.get_game_resources().get_font());
  std::stringstream s;
  const auto& selected_units = get_selected_pieces(view.get_game());
  if (selected_units.empty()) {
    s << "LMB: select a unit";
  } else {
    s << "LMB: select a unit\n"
      << "RMB: move selected unit to square"
    ;
  }
  text.setString(s.str());
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_tl_controls().get_x(),
    layout.get_tl_controls().get_y()
  );
  view.get_window().draw(text);
}

void show_debug(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  sf::Text text;
  text.setFont(view.get_game_resources().get_font());
  const piece& closest_piece{
    game.get_closest_piece_to(game.get_mouse_pos())
  };
  std::stringstream s;
  s << "Game position: "
    << game.get_mouse_pos()
    << '\n'
    << "Screen position: "
    << convert_to_screen_coordinat(game.get_mouse_pos(), layout)
    << '\n'
    << "Is there a piece here: "
    << bool_to_str(is_piece_at(game, game.get_mouse_pos(), 0.5))
    << '\n'
    << "Closest piece: " << closest_piece.get_type() << ": " << closest_piece.get_coordinat() << '\n'
    << "Number of game actions: " << count_game_actions(game) << '\n'
    << "Number of selected units: " << count_selected_units(game) << '\n'
    << "Number of piece actions: " << count_piece_actions(game) << '\n'
  ;

  text.setString(s.str());
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_tl_debug().get_x(),
    layout.get_tl_debug().get_y()
  );
  view.get_window().draw(text);
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

void show_pieces(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const double square_width{layout.get_square_width()};
  const double square_height{layout.get_square_height()};
  for (const auto& piece: game.get_pieces())
  {
    sf::RectangleShape sprite;
    sprite.setSize(sf::Vector2f(square_width, square_height));
    sprite.setTexture(
      &view.get_game_resources().get_piece(
        piece.get_color(),
        piece.get_type()
      )
    );
    if (piece.is_selected())
    {
      sprite.setFillColor(sf::Color(255, 196, 196));
    }
    sprite.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
    const auto screen_position = convert_to_screen_coordinat(piece.get_coordinat(), layout);
    sprite.setPosition(
      screen_position.get_x(),
      screen_position.get_y()
    );
    sprite.setRotation(0.0);
    view.get_window().draw(sprite);
  }
}

void show_sidebar(game_view& view)
{
  show_controls(view);
  show_unit_sprites(view);
  show_debug(view);
}

void show_squares(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();

  sf::RectangleShape black_square = create_black_square(view);
  sf::RectangleShape white_square = create_white_square(view);

  for (int x = 0; x != 8; ++x)
  {
    for (int y = 0; y != 8; ++y)
    {
      sf::RectangleShape& s = (x + y) % 2 == 1 ? black_square : white_square;
      const screen_coordinat square_pos{
        convert_to_screen_coordinat(
          game_coordinat(x + 0.5, y + 0.5),
          layout
        )
      };
      s.setPosition(square_pos.get_x(), square_pos.get_y());
      view.get_window().draw(s);
    }
  }
  show_square_under_cursor(view);
}
void show_square_under_cursor(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const int x{static_cast<int>(std::trunc(game.get_mouse_pos().get_x()))};
  const int y{static_cast<int>(std::trunc(game.get_mouse_pos().get_y()))};

  if (x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    sf::RectangleShape s{
      (x + y) % 2 == 1
      ? create_black_square(view)
      : create_white_square(view)
    };
    const screen_coordinat square_pos{
      convert_to_screen_coordinat(
        game_coordinat(x + 0.5, y + 0.5),
        layout
      )
    };
    s.setPosition(square_pos.get_x(), square_pos.get_y());
    const auto old_fill_color = s.getFillColor();
    const auto old_outline_color = s.getOutlineColor();
    const auto old_thickness = s.getOutlineThickness();
    const bool valid{would_be_valid(view)};
    if (valid)
    {
      assert(1 == 2); // TODO
      s.setFillColor(sf::Color(196, 255, 196));
      s.setOutlineColor(sf::Color::Green);
    }
    else
    {
      s.setFillColor(sf::Color(255, 196, 196));
      s.setOutlineColor(sf::Color::Red);
    }
    s.setOutlineThickness(4);
    view.get_window().draw(s);
    s.setFillColor(old_fill_color);
    s.setOutlineColor(old_outline_color);
    s.setOutlineThickness(old_thickness);
  }
}

void show_unit_health_bars(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  for (const auto& piece: game.get_pieces())
  {
    // Black box around it
    sf::RectangleShape black_box;

    black_box.setSize(sf::Vector2f(layout.get_square_width() - 4.0, 16.0 - 4.0));
    //black_box.setScale(1.0, 1.0);
    black_box.setFillColor(sf::Color(0, 0, 0));
    black_box.setOrigin(0.0, 0.0);
    const auto black_box_pos = convert_to_screen_coordinat(
      piece.get_coordinat() + game_coordinat(-0.5, 0.5), // to top-left
      layout
    );
    black_box.setPosition(
      2.0 + black_box_pos.get_x(),
      2.0 + black_box_pos.get_y()
    );
    black_box.setRotation(0.0);
    view.get_window().draw(black_box);

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
    const auto health_bar_pos = convert_to_screen_coordinat(
      piece.get_coordinat() + game_coordinat(-0.5, 0.5), // to top-left,
      layout
    );
    health_bar.setPosition(
      4.0 + health_bar_pos.get_x(),
      4.0 + health_bar_pos.get_y()
    );
    health_bar.setRotation(0.0);
    view.get_window().draw(health_bar);
  }
}

void show_unit_paths(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  for (const auto& piece: get_pieces(game))
  {
    if (is_idle(piece)) continue;
    std::vector<screen_coordinat> coordinats;
    coordinats.reserve(piece.get_actions().size() + 1); // +1 for current position
    coordinats.push_back(
      convert_to_screen_coordinat(
        piece.get_coordinat(),
        layout
      )
    );
    const auto& actions{piece.get_actions()};
    std::transform(
      std::begin(actions),
      std::end(actions),
      std::back_inserter(coordinats),
      [layout](const auto& action)
      {
        return convert_to_screen_coordinat(
          action.get_coordinat(),
          layout
        );
      }
    );
    sf::VertexArray lines(sf::LineStrip, coordinats.size());
    assert(coordinats.size() == actions.size() + 1);
    const int n_coordinats{static_cast<int>(coordinats.size())};
    for (int i = 0; i != n_coordinats; ++i)
    {
      assert(i < static_cast<int>(coordinats.size()));
      lines[i].position = sf::Vector2f(
        coordinats[i].get_x(),
        coordinats[i].get_y()
      );
      lines[i].color = sf::Color::Blue;
      //lines[i].color = actions[i].get_type() == piece_action_type::move
      //  ? sf::Color::Blue : sf::Color::Red
      //;
    }
    view.get_window().draw(lines);
  }
}

void show_unit_sprites(game_view& view)
{
  const auto& layout = view.get_game().get_layout();
  const double square_width{layout.get_square_width()};
  const double square_height{layout.get_square_height()};
  screen_coordinat screen_position = layout.get_tl_units();
  for (const auto& piece: get_selected_pieces(view.get_game()))
  {
    // sprite of the piece
    sf::RectangleShape sprite;
    sprite.setSize(sf::Vector2f(square_width, square_height));
    sprite.setTexture(
      &view.get_game_resources().get_piece_portrait(
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
    view.get_window().draw(sprite);
    // text
    sf::Text text;
    text.setFont(view.get_game_resources().get_font());
    std::stringstream s;

    s << piece.get_type() << ": "
      << piece.get_health() << "/"
      << piece.get_max_health() << '\n'
      << piece.get_coordinat() << '\n'
      << describe_actions(piece)
    ;
    text.setString(s.str());
    text.setCharacterSize(20);
    const auto text_position{
      screen_position + screen_coordinat(square_width + 10, 0)
    };
    text.setPosition(
      text_position.get_x(),
      text_position.get_y()
    );
    view.get_window().draw(text);
    screen_position += screen_coordinat(0, square_height);
  }
}

void test_game_view() //!OCLINT tests may be many
{
  #ifndef NDEBUG // no tests in release
  #endif //NDEBUG
}

void toggle_player(game_view& view)
{
  toggle_player(view.get_game());
}

bool would_be_valid(const game_view& view)
{
  const auto& game{view.get_game()};
  if (count_selected_units(game) == 0)
  {
    return can_select_piece_at_mouse_pos(game);
  }
  return false;
}
