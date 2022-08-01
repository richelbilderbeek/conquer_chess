#include "game_view.h"

#ifndef LOGIC_ONLY

#include "control_action_type.h"
#include "game.h"
#include "game_resources.h"
#include "game_view_layout.h"
#include "game_rect.h"
#include "screen_coordinat.h"
#include "screen_rect.h"
#include "sfml_helper.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>

game_view::game_view(const game& game)
  : m_game{game},
    m_log{game.get_options().get_message_display_time_secs()}
{
  m_game_resources.get_ninja_gods().setVolume(
    get_music_volume_as_percentage(m_game)
  );
  m_game_resources.get_ninja_gods().play();
  m_game_resources.get_sound_effects().set_master_volume(
    m_game.get_options().get_sound_effects_volume()
  );
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
  black_square.setTexture(&view.get_resources().get_square(chess_color::black));
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
  white_square.setTexture(&view.get_resources().get_square(chess_color::white));
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

    // Disard old messages
    m_log.tick();

    // Process user input and play game until instructed to exit
    const bool must_quit{
      process_events() // main game loop
    };
    if (must_quit) return;

    // Do a tick, so that one delta_t equals one second under normal game speed
    m_game.tick(
      delta_t(1.0 / m_fps_clock.get_fps())
      * to_delta_t(m_game.get_options().get_game_speed())
    );

    // Read the pieces' messages and play their sounds
    process_piece_messages();

    // Show the new state
    show();
  }
}

std::string get_controls_text(
  const game_view& view,
  const chess_color player,
  const controller_type controller,
  const int key
)
{
  assert(key >= 1); // Human based counting
  assert(key <= 4); // Human based counting
  const auto& selected_units = get_selected_pieces(view.get_game(), player);
  std::stringstream s;
  if (controller == controller_type::keyboard)
  {
    if (selected_units.empty()) return "Spacebar\nSelect";
    switch (key)
    {
      case 1: return "M\nMove";
      case 2: return "A\nAttack";
      case 3: return ".";
      case 4:
        default:
        assert(key == 4);
        return ".";
    }
  }
  else
  {
    assert(controller == controller_type::mouse);
    if (selected_units.empty()) return "LMB\nSelect";
    switch (key)
    {
      case 1: return "Move";
      case 2: return "Attack";
      case 3: return ".";
      case 4:
        default:
        assert(key == 4);
        return ".";
    }
  }
}

double game_view::get_elapsed_time_secs() const noexcept
{
  return m_clock.getElapsedTime().asSeconds();
}

int get_fps(const game_view& v) noexcept
{
  return v.get_fps();
}

std::string get_last_log_messages(
  const game_view& view,
  const side player
) noexcept
{
  return get_last_log_messages(
    view.get_log(),
    get_player_color(view.get_game(), player)
  );
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
    m_log.add_message(piece_message);
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

  // Show the sidebars: controls (with log), units, debug
  show_sidebar_1(*this);
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
  show_square_under_cursor(view, side::lhs);
  show_square_under_cursor(view, side::rhs);
  show_possible_moves(view, side::lhs);
  show_possible_moves(view, side::rhs);
  show_unit_paths(view);
  show_pieces(view);
  show_unit_health_bars(view);
}

void show_controls(game_view& view, const side player)
{
  const auto& layout = view.get_game().get_layout();
  sf::Text text;
  text.setFont(view.get_resources().get_font());
  text.setCharacterSize(16);

  const std::vector<sf::Color> colors{
    sf::Color(255,  0,  0),
    sf::Color(255,128,  0),
    sf::Color(255,255,  0),
    sf::Color(128,255,  0)
  };

  for (int key{1}; key<=4; ++key) // Human based counting
  {
    // Rect
    sf::RectangleShape rectangle;
    set_rect(rectangle, layout.get_controls_key(player, key));
    rectangle.setFillColor(colors[key -1]);
    rectangle.setOutlineThickness(1);
    rectangle.setOutlineColor(sf::Color::White);
    view.get_window().draw(rectangle);

    // Text
    const std::string s{
      get_controls_text(
        view,
        get_player_color(view.get_game(), player),
        get_player_controller(get_options(view), player),
        key
      )
    };
    text.setString(s.c_str());
    text.setPosition(
      layout.get_controls_key(player, key).get_tl().get_x(),
      layout.get_controls_key(player, key).get_tl().get_y()
    );
    view.get_window().draw(text);
  }

}

void show_debug_1(game_view& view)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  sf::Text text;
  text.setFont(view.get_resources().get_font());
  const piece& closest_piece{
    get_closest_piece_to(game, get_player_pos(game, side::lhs))
  };
  const auto color{get_left_player_color(game.get_options())};
  std::stringstream s;
  s << "Color: " << color << '\n'
    << "Controller: " << get_left_player_controller(view.get_game().get_options()) << '\n'
    << "Game position: "
    << to_notation(get_player_pos(game, side::lhs))
    << " "
    << get_player_pos(game, side::lhs)
    << '\n'
    << "Screen position: "
    << convert_to_screen_coordinat(get_player_pos(game, side::lhs), layout)
    << '\n'
    << "Is there a piece here: "
    << bool_to_str(is_piece_at(game, get_player_pos(game, side::lhs), 0.5))
    << '\n'
    << "Closest piece: " << closest_piece.get_type() << ": " << to_coordinat(closest_piece.get_current_square()) << '\n'
    << "Number of game actions: " << count_control_actions(game) << '\n'
    << "Number of selected units: " << count_selected_units(game, color) << '\n'
    << "Number of piece actions: " << count_piece_actions(game, color) << '\n'
  ;

  // Specific things
  s
    << "Wall-clock time: " << view.get_elapsed_time_secs() << " (secs)" << '\n'
    << "Game time: " << get_time(view) << " (moves)" << '\n'
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
  text.setFont(view.get_resources().get_font());
  const piece& closest_piece{
    get_closest_piece_to(game, get_player_pos(game, side::rhs))
  };
  const auto color{get_right_player_color(game.get_options())};
  std::stringstream s;
  s << "Color: " << color << '\n'
    << "Controller: " << get_right_player_controller(game.get_options()) << '\n'
    << "Game position: "
    << to_notation(get_player_pos(game, side::rhs))
    << " "
    << get_player_pos(game, side::rhs)
    << '\n'
    << "Screen position: "
    << convert_to_screen_coordinat(get_player_pos(game, side::rhs), layout)
    << '\n'
    << "Is there a piece here: "
    << bool_to_str(is_piece_at(game, get_player_pos(game, side::rhs), 0.5))
    << '\n'
    << "Closest piece: "
      << closest_piece.get_type() << ": "
      << to_coordinat(closest_piece.get_current_square()) << '\n'
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

void show_log(game_view& view, const side player)
{
  const auto& layout = view.get_game().get_layout();
  sf::Text text;
  text.setFont(view.get_resources().get_font());
  std::stringstream s;
  s << get_last_log_messages(view, player);
  text.setString(s.str().c_str());
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_log(player).get_tl().get_x(),
    layout.get_log(player).get_tl().get_y()
  );
  view.get_window().draw(text);
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
    if (1 + 1 == 2)
    {
      s.setTexture(
        &view.get_resources().get_textures().get_occupied_square(
          to_color(square),
          piece.get_color()
        )
      );


    }
    else
    {
      s.rotate(45);
      s.scale(0.375, 0.375);
      s.setFillColor(to_sfml_color(piece.get_color()));
      s.setOutlineColor(to_sfml_color(get_other_color(piece.get_color())));
      s.setOutlineThickness(1.0);
    }
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
      &view.get_resources().get_piece(
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
      to_coordinat(piece.get_current_square()) + game_coordinat(0.0, 0.1),
      layout
    );
    sprite.setPosition(
      screen_position.get_x(),
      screen_position.get_y()
    );
    view.get_window().draw(sprite);
  }
}

void show_possible_moves(game_view& view, const side player)
{
  const auto& g{view.get_game()};
  const auto&layout{g.get_layout()};
  const auto color{get_player_color(g, player)};
  if (count_selected_units(g, color) == 0) return;

  const std::vector<square> possible_moves{
    get_possible_moves(g, player)
  };
  if (possible_moves.empty()) return;

  assert(are_all_unique(possible_moves));
  for (const auto& square: possible_moves)
  {
    sf::RectangleShape rectangle;
    set_rect(
      rectangle,
      convert_to_screen_rect(square, layout)
    );
    rectangle.setOutlineColor(to_sfml_color(color));
    rectangle.setOutlineThickness(3);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setScale(0.5, 0.5);
    rectangle.setRotation(player == side::lhs ? 30 : -30);
    view.get_window().draw(rectangle);
  }
}

void show_sidebar_1(game_view& view)
{
  show_unit_sprites_1(view);
  show_controls(view, side::lhs);
  show_log(view, side::lhs);
  show_debug_1(view);
}

void show_sidebar_2(game_view& view)
{
  show_unit_sprites_2(view);
  show_controls(view, side::rhs);
  show_log(view, side::rhs);
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
}

void show_square_under_cursor(
  game_view& view,
  const side player)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const int x{static_cast<int>(std::trunc(get_player_pos(game, player).get_x()))};
  if (x < 0 || x >= 8) return;
  const int y{static_cast<int>(std::trunc(get_player_pos(game, player).get_y()))};
  if (y < 0 || y >= 8) return;

  assert(x >= 0 && x < 8 && y >= 0 && y < 8);
  sf::RectangleShape s;
  const double square_width{get_square_width(layout)};
  const double square_height{get_square_height(layout)};
  s.setSize(sf::Vector2f(square_width + 1, square_height + 1));
  s.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
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
  const auto player_color{get_player_color(game.get_options(), player)};
  s.setOutlineColor(to_sfml_color(player_color));
  s.setFillColor(sf::Color::Transparent);
  const bool valid{would_be_valid(view, player_color)};
  if (valid)
  {
    s.setOutlineThickness(4);
  }
  else
  {
    s.setOutlineThickness(2);
  }
  view.get_window().draw(s);
  s.setFillColor(old_fill_color);
  s.setOutlineColor(old_outline_color);
  s.setOutlineThickness(old_thickness);
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
      to_coordinat(piece.get_current_square()) + game_coordinat(-0.5, -0.5),
      layout
    );
    black_box.setPosition(
      2.0 + black_box_pos.get_x(),
      2.0 + black_box_pos.get_y()
    );
    view.get_window().draw(black_box);

    // Health
    sf::RectangleShape health_bar;
    const double max_width{get_square_width(layout) - 8.0}; // with full health
    health_bar.setSize(
      sf::Vector2f(
      max_width * get_f_health(piece),
      16.0 - 8.0)
    );

    health_bar.setFillColor(f_health_to_color(get_f_health(piece)));
    health_bar.setOrigin(0.0, 0.0);
    const auto health_bar_pos = convert_to_screen_coordinat(
      to_coordinat(piece.get_current_square()) + game_coordinat(-0.5, -0.5),
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

    // Collect all the coordinats for the path
    std::vector<screen_coordinat> coordinats;
    coordinats.reserve(piece.get_actions().size() + 1); // +1 for current position
    coordinats.push_back(
      convert_to_screen_coordinat(
        to_coordinat(piece.get_current_square()),
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

    // Draw circle at current in-progress movement
    assert(!actions.empty());
    const auto& first_action{actions[0]};
    if (first_action.get_action_type() == piece_action_type::move)
    {
      const auto from_pixel{
        convert_to_screen_coordinat(
          to_coordinat(piece.get_current_square()),
          layout
        )
      };
      const auto to_pixel{
        convert_to_screen_coordinat(
          to_coordinat(first_action.get_to()),
          layout
        )
      };
      const auto f{piece.get_current_action_time().get()};
      assert(f >= 0.0);
      assert(f <= 1.0);
      const auto delta_pixel{to_pixel - from_pixel};
      const auto now_pixel{from_pixel + (delta_pixel * f)};

      const double full_diameter{get_square_width(layout)};
      const double diameter{0.25 * full_diameter};
      const double radius{diameter / 2.0};
      const double half_radius{radius/ 2.0};
      circle.setPosition(
        sf::Vector2f(
          now_pixel.get_x() - half_radius,
          now_pixel.get_y() - half_radius
        )
      );
      circle.setOutlineColor(to_sfml_color(get_other_color(piece.get_color())));
      circle.setOutlineThickness(std::max(2.0, radius / 10.0));
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
      &view.get_resources().get_piece_portrait(
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
    text.setFont(view.get_resources().get_font());
    std::stringstream s;

    s << piece.get_type() << ": "
      << piece.get_health() << "/"
      << piece.get_max_health() << '\n'
      << piece.get_current_square() << '\n'
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
      &view.get_resources().get_piece_portrait(
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
    text.setFont(view.get_resources().get_font());
    std::stringstream s;

    s << piece.get_type() << ": "
      << piece.get_health() << "/"
      << piece.get_max_health() << '\n'
      << piece.get_current_square() << '\n'
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
