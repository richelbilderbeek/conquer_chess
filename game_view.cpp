#include "game_view.h"

#ifndef LOGIC_ONLY

#include "control_action_type.h"
#include "game.h"
#include "game_resources.h"
#include "game_view_layout.h"
#include "game_rect.h"
#include "screen_coordinat.h"
#include "sfml_helper.h"

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
  m_game_resources.get_ninja_gods().setVolume(
    static_cast<float>(m_game.get_options().get_volume().get_percentage())
  );
  m_game_resources.get_ninja_gods().play();
}

std::string bool_to_str(const bool b) noexcept
{
  return b ? "true" : "false";
}

sf::RectangleShape create_black_square(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const double square_width{get_square_width(layout)};
  const double square_height{get_square_height(layout)};

  sf::RectangleShape black_square;
  black_square.setSize(sf::Vector2f(square_width + 1, square_height + 1));
  black_square.setTexture(&view.get_game_resources().get_square(chess_color::black));
  black_square.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
  return black_square;
}

sf::RectangleShape create_white_square(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const double square_width{get_square_width(layout)};
  const double square_height{get_square_height(layout)};

  sf::RectangleShape white_square;
  white_square.setSize(sf::Vector2f(square_width + 1, square_height + 1));
  white_square.setTexture(&view.get_game_resources().get_square(chess_color::white));
  white_square.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
  return white_square;
}

bool do_show_selected(const game_view& view)
{
  return do_show_selected(view.get_game());
}

void game_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      m_game.get_layout().get_window_size().get_x(),
      m_game.get_layout().get_window_size().get_y()
    ),
    "Conquer Chess"
  );
  while (m_window.isOpen())
  {
    // Keep track of the FPS
    m_fps_clock.tick();

    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Do a tick
    m_game.tick(to_delta_t(m_game.get_options().get_game_speed()));

    // Read the pieces' messages and play their sounds
    process_piece_messages();

    // Show the new state
    show();
  }
}

std::string get_controls_text_1(const game_view& view)
{
  return get_controls_text(
    view,
    get_left_player_color(view.get_game().get_options()),
    get_left_player_controller(view.get_game().get_options())
  );
}

std::string get_controls_text_2(const game_view& view)
{
  return get_controls_text(
    view,
    get_right_player_color(view.get_game().get_options()),
    get_right_player_controller(view.get_game().get_options())
  );
}

std::string get_controls_text(
  const game_view& view,
  const chess_color player,
  const controller_type controller
)
{
  const auto& selected_units = get_selected_pieces(view.get_game(), player);
  std::stringstream s;
  if (controller == controller_type::keyboard)
  {
    if (selected_units.empty()) {
      s << "SPACE: select a unit";
    } else {
      s << "SPACE: select a unit\n"
        << "M: move selected unit to square"
      ;
    }
  }
  else
  {
    assert(controller == controller_type::mouse);
    if (selected_units.empty()) {
      s << "LMB: select a unit";
    } else {
      s << "LMB: select a unit\n"
        << "RMB: move selected unit to square"
      ;
    }

  }
  return s.str();
}

int get_fps(const game_view& v) noexcept
{
  return v.get_fps();
}

const game_view_layout& get_layout(const game_view& v) noexcept
{
  return get_layout(v.get_game());
}

const game_options& get_options(const game_view& v) noexcept
{
  return get_options(v.get_game());
}

const std::vector<piece>& get_pieces(const game_view& v) noexcept
{
  return get_pieces(v.get_game());
}

void game_view::play_pieces_sound_effects()
{
  for (const auto& sound_effect: collect_messages(m_game))
  {
    m_game_resources.play(sound_effect);
  }
}

const delta_t& get_time(const game_view& v) noexcept
{
  return get_time(v.get_game());
}

bool game_view::process_events()
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
      m_game.get_layout() = game_view_layout(
        screen_coordinat(
          static_cast<int>(event.size.width),
          static_cast<int>(event.size.height)
        ),
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
        m_game.add_action(create_press_up_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Right)
      {
        m_game.add_action(create_press_right_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Down)
      {
        m_game.add_action(create_press_down_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Left)
      {
        m_game.add_action(create_press_left_action());
      }
      else if (key_pressed == sf::Keyboard::Key::Space)
      {
        m_game.add_action(create_press_select_action());
      }
      else if (key_pressed == sf::Keyboard::Key::M)
      {
        m_game.add_action(create_press_move_action());
      }
      else if (key_pressed == sf::Keyboard::Key::A)
      {
        m_game.add_action(create_press_attack_action());
      }
      else if (key_pressed == sf::Keyboard::Key::F3)
      {
        // debug
        std::clog << "Debug";
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

void game_view::process_piece_messages()
{
  for (const auto& piece_message: collect_messages(m_game))
  {
    m_log.add_message(m_game.get_time(), piece_message);
  }

  // Play the new sounds to be played
  play_pieces_sound_effects();

  clear_piece_messages(m_game);
}

void game_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  // Show the layout of the screen: board and sidebars
  show_layout(*this);

  // Show the board: squares, unit paths, pieces, health bars
  show_board(*this);

  // Show the sidebar: controls, units, debug
  show_sidebar_1(*this);

  // Show the sidebar: controls, units, debug
  show_sidebar_2(*this);

  // Show the mouse cursor
  //show_mouse_cursor();

  // Display all shapes
  m_window.display();
}

void show_board(game_view& view)
{
  show_squares(view);
  if (get_options(view).do_show_occupied())
  {
    show_occupied_squares(view);
  }
  show_unit_paths(view);
  show_pieces(view);
  show_unit_health_bars(view);
}

void show_controls_1(game_view& view)
{
  const auto& layout = view.get_game().get_layout();
  sf::Text text;
  text.setFont(view.get_game_resources().get_font());
  std::stringstream s;
  text.setString(get_controls_text_1(view));
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_controls_1().get_tl().get_x(),
    layout.get_controls_1().get_tl().get_y()
  );
  view.get_window().draw(text);
}

void show_controls_2(game_view& view)
{
  const auto& layout = view.get_game().get_layout();
  sf::Text text;
  text.setFont(view.get_game_resources().get_font());
  std::stringstream s;
  text.setString(get_controls_text_2(view));
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_controls_2().get_tl().get_x(),
    layout.get_controls_2().get_tl().get_y()
  );
  view.get_window().draw(text);
}

void show_debug_1(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  sf::Text text;
  text.setFont(view.get_game_resources().get_font());
  const piece& closest_piece{
    get_closest_piece_to(game, game.get_player_1_pos())
  };
  const auto color{get_left_player_color(game.get_options())};
  std::stringstream s;
  s << "Color: " << color << '\n'
    << "Controller: " << get_left_player_controller(view.get_game().get_options()) << '\n'
    << "Game position: "
    << to_notation(game.get_player_1_pos())
    << " "
    << game.get_player_1_pos()
    << '\n'
    << "Screen position: "
    << convert_to_screen_coordinat(game.get_player_1_pos(), layout)
    << '\n'
    << "Is there a piece here: "
    << bool_to_str(is_piece_at(game, game.get_player_1_pos(), 0.5))
    << '\n'
    << "Closest piece: " << closest_piece.get_type() << ": " << closest_piece.get_coordinat() << '\n'
    << "Number of game actions: " << count_control_actions(game) << '\n'
    << "Number of selected units: " << count_selected_units(game, color) << '\n'
    << "Number of piece actions: " << count_piece_actions(game, color) << '\n'
  ;

  // Specific things
  s
    << "Time: " << get_time(view) << '\n'
    << "FPS: " << get_fps(view) << '\n'
  ;



  text.setString(s.str());
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_debug_1().get_tl().get_x(),
    layout.get_debug_1().get_tl().get_y()
  );
  view.get_window().draw(text);
}

void show_debug_2(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  sf::Text text;
  text.setFont(view.get_game_resources().get_font());
  const piece& closest_piece{
    get_closest_piece_to(game, game.get_player_2_pos())
  };
  const auto color{get_right_player_color(game.get_options())};
  std::stringstream s;
  s << "Color: " << color << '\n'
    << "Controller: " << get_right_player_controller(game.get_options()) << '\n'
    << "Game position: "
    << to_notation(game.get_player_2_pos())
    << " "
    << game.get_player_2_pos()
    << '\n'
    << "Screen position: "
    << convert_to_screen_coordinat(game.get_player_2_pos(), layout)
    << '\n'
    << "Is there a piece here: "
    << bool_to_str(is_piece_at(game, game.get_player_2_pos(), 0.5))
    << '\n'
    << "Closest piece: " << closest_piece.get_type() << ": " << closest_piece.get_coordinat() << '\n'
    << "Number of game actions: " << count_control_actions(game) << '\n'
    << "Number of selected units: " << count_selected_units(game, color) << '\n'
    << "Number of piece actions: " << count_piece_actions(game, color) << '\n'
  ;

  text.setString(s.str());
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_debug_2().get_tl().get_x(),
    layout.get_debug_2().get_tl().get_y()
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
      get_mouse_player_pos(m_game),
      layout
    )
  };
  cursor.setPosition(
    cursor_pos.get_x(),
    cursor_pos.get_y()
  );
  m_window.draw(cursor);
}

void show_layout(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  for (const auto& panel: get_panels(layout))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, panel);
    rectangle.setFillColor(sf::Color::Black);
    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(sf::Color::White);
    view.get_window().draw(rectangle);
  }
}

void show_occupied_squares(game_view& view)
{
  assert(get_options(view).do_show_occupied());
  const auto& pieces{get_pieces(view)};
  for (const auto& piece: pieces)
  {
    const auto& square{get_occupied_square(piece)};
    const screen_rect square_rect{
      convert_to_screen_rect(
        to_game_rect(square),
        get_layout(view)
      )
    };
    sf::RectangleShape s;
    set_rect(s, square_rect);
    s.rotate(45);
    s.scale(0.375, 0.375);
    s.setFillColor(to_sfml_color(piece.get_color()));
    s.setOutlineColor(to_sfml_color(get_other_color(piece.get_color())));
    s.setOutlineThickness(1.0);
    view.get_window().draw(s);
  }
}

void show_pieces(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const double square_width{get_square_width(layout)};
  const double square_height{get_square_height(layout)};
  for (const auto& piece: game.get_pieces())
  {
    sf::RectangleShape sprite;
    sprite.setSize(sf::Vector2f(0.9 * square_width, 0.9 * square_height));
    sprite.setTexture(
      &view.get_game_resources().get_piece(
        piece.get_color(),
        piece.get_type()
      )
    );
    if (do_show_selected(view) && piece.is_selected())
    {
      sprite.setOutlineColor(sf::Color(255, 0, 0));
      sprite.setOutlineThickness(2);
    }
    sprite.setOrigin(sf::Vector2f(0.45 * square_width, 0.45 * square_height));
    const auto screen_position = convert_to_screen_coordinat(
      piece.get_coordinat() + game_coordinat(0.0, 0.1),
      layout
    );
    sprite.setPosition(
      screen_position.get_x(),
      screen_position.get_y()
    );
    view.get_window().draw(sprite);
  }
}

void show_sidebar_1(game_view& view)
{
  show_controls_1(view);
  show_unit_sprites_1(view);
  show_debug_1(view);
}

void show_sidebar_2(game_view& view)
{
  show_controls_2(view);
  show_unit_sprites_2(view);
  show_debug_2(view);
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
      sf::RectangleShape& s = (x + y) % 2 == 0 ? black_square : white_square;
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
  show_square_under_cursor_1(view);
  show_square_under_cursor_2(view);
}

void show_square_under_cursor_1(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const int x{static_cast<int>(std::trunc(game.get_player_1_pos().get_x()))};
  const int y{static_cast<int>(std::trunc(game.get_player_1_pos().get_y()))};

  if (x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    sf::RectangleShape s{
      (x + y) % 2 == 0
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
    const auto player_color{get_left_player_color(game.get_options())};
    s.setOutlineColor(to_sfml_color(player_color));
    if (1 == 2) // Show if a piece can be interacted with
    {
      const bool valid{would_be_valid(view, player_color)};
      if (valid)
      {
        s.setFillColor(to_sfml_color(player_color));
      }
      else
      {
        s.setFillColor(sf::Color(255, 128, 128));
      }
    }
    s.setOutlineThickness(4);
    view.get_window().draw(s);
    s.setFillColor(old_fill_color);
    s.setOutlineColor(old_outline_color);
    s.setOutlineThickness(old_thickness);
  }
}

void show_square_under_cursor_2(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const int x{static_cast<int>(std::trunc(game.get_player_2_pos().get_x()))};
  const int y{static_cast<int>(std::trunc(game.get_player_2_pos().get_y()))};

  if (x >= 0 && x < 8 && y >= 0 && y < 8)
  {
    sf::RectangleShape s{
      (x + y) % 2 == 0
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
    const auto player_color{get_right_player_color(game.get_options())};
    s.setOutlineColor(to_sfml_color(player_color));
    if (1 == 2) // Show if a piece can be interacted with
    {
      const bool valid{would_be_valid(view, player_color)};
      if (valid)
      {
        s.setFillColor(to_sfml_color(player_color));
      }
      else
      {
        s.setFillColor(sf::Color(255, 128, 128));
      }
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

    black_box.setSize(sf::Vector2f(get_square_width(layout) - 4.0, 16.0 - 4.0));
    //black_box.setScale(1.0, 1.0);
    black_box.setFillColor(sf::Color(0, 0, 0));
    black_box.setOrigin(0.0, 0.0);
    const auto black_box_pos = convert_to_screen_coordinat(
      piece.get_coordinat() + game_coordinat(-0.5, -0.5),
      layout
    );
    black_box.setPosition(
      2.0 + black_box_pos.get_x(),
      2.0 + black_box_pos.get_y()
    );
    view.get_window().draw(black_box);

    // Health
    sf::RectangleShape health_bar;
    health_bar.setSize(sf::Vector2f(get_square_width(layout) - 8.0, 16.0 - 8.0));
    //health_bar.setScale(1.0, 1.0);
    health_bar.setFillColor(
      sf::Color(
        static_cast<sf::Uint8>(get_f_health(piece) * 255.0),
        static_cast<sf::Uint8>(get_f_health(piece) * 255.0),
        static_cast<sf::Uint8>(get_f_health(piece) * 255.0)
      )
    );
    health_bar.setOrigin(0.0, 0.0);
    const auto health_bar_pos = convert_to_screen_coordinat(
      piece.get_coordinat() + game_coordinat(-0.5, -0.5),
      layout
    );
    health_bar.setPosition(
      4.0 + health_bar_pos.get_x(),
      4.0 + health_bar_pos.get_y()
    );
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
      [layout](const auto& control_action)
      {
        return convert_to_screen_coordinat(
          to_coordinat(control_action.get_to()),
          layout
        );
      }
    );
    // Draw lines between the subgoals
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
      lines[i].color = to_sfml_color(piece.get_color());
    }
    view.get_window().draw(lines);

    // Draw circles at the subgoals
    sf::CircleShape circle;


    for (const auto coordinat: coordinats)
    {
      const double full_diameter{get_square_width(layout)};
      const double diameter{0.25 * full_diameter};
      const double radius{diameter / 2.0};
      const double half_radius{radius/ 2.0};
      circle.setPosition(
        sf::Vector2f(
          coordinat.get_x() - half_radius,
          coordinat.get_y() - half_radius
        )
      );
      circle.setFillColor(to_sfml_color(piece.get_color()));
      circle.setRadius(radius);
      circle.setOrigin(half_radius, half_radius);
      view.get_window().draw(circle);
    }
  }
}

void show_unit_sprites_1(game_view& view)
{
  const auto& layout = view.get_game().get_layout();
  const double square_width{get_square_width(layout)};
  const double square_height{get_square_height(layout)};
  screen_coordinat screen_position = layout.get_units_1().get_tl();
  const auto player_color{get_left_player_color(view.get_game().get_options())};

  for (const auto& piece: get_selected_pieces(view.get_game(), player_color))
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

void show_unit_sprites_2(game_view& view)
{
  const auto& layout = view.get_game().get_layout();
  const double square_width{get_square_width(layout)};
  const double square_height{get_square_height(layout)};
  screen_coordinat screen_position = layout.get_units_2().get_tl();
  const auto player_color{get_right_player_color(view.get_game().get_options())};
  for (const auto& piece: get_selected_pieces(view.get_game(), player_color))
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

void toggle_left_player_color(game_view& view)
{
  toggle_left_player_color(view.get_game());
}

bool would_be_valid(
  const game_view& view,
  chess_color player_color
)
{
  const auto& game{view.get_game()};
  return can_player_select_piece_at_cursor_pos(game, player_color);
}

#endif // LOGIC_ONLY
