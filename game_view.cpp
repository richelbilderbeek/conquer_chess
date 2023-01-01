#include "game_view.h"

#ifndef LOGIC_ONLY

#include "physical_controller.h"
#include "physical_controllers.h"
#include "user_input_type.h"
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
#include <numeric>
#include <string>
#include <sstream>

game_view::game_view(const game& game)
  : m_controllers{create_default_controllers()},
    m_game{game},
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
    if (must_quit)
    {
      break;
    }

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

  std::clog << collect_action_history(m_game) << '\n';
}

const auto& game_view::get_controller(const side player) const noexcept
{
  if (player == side::lhs)
  {
    assert(m_controllers.size() >= 1);
    assert(m_controllers[0].get_player() == player);
    return m_controllers[0];
  }
  assert(player == side::rhs);
  assert(m_controllers.size() >= 2);
  assert(m_controllers[1].get_player() == player);
  return m_controllers[1];
}

const physical_controller& get_physical_controller(const game_view& view, const side player_side)
{
  return get_physical_controller(view.get_game(), player_side);
}

physical_controller_type get_physical_controller_type(const game_view& view, const side player_side)
{
  return get_physical_controller(view, player_side).get_type();
}

std::string get_controls_text(
  const game_view& view,
  const physical_controller& c,
  const int key
)
{
  assert(key >= 1); // Human based counting
  assert(key <= 4); // Human based counting
  return get_text_for_action(view, c, key);
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

chess_color get_player_color(
  const game_view& v,
  const side player
) noexcept
{
  return get_player_color(v.get_game(), player);
}

const game_coordinat& get_cursor_pos(const game_view& view, const side player) noexcept
{
  return get_cursor_pos(view.get_game(), player);
}

std::string get_text_for_action(
  const game_view& view,
  const physical_controller& c,
  const int key
)
{
  const auto& g{view.get_game()};
  const auto default_action{
    get_default_piece_action(g, c.get_player())
  };
  if (!default_action) return "";
  if (key == 1)
  {
    return to_str(default_action.value());
  }
  else
  {
    if (default_action.value() == piece_action_type::promote_to_queen)
    {
      switch (key)
      {
        case 2: return "Promote to Rook";
        case 3: return "Promote to bishop";
        case 4:
        default:
          assert(key == 4);
          return "Promote to knight";
      }
    }
    return "";
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
      return false;
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
    process_event(m_game, event);
  }
  return false; // if no events proceed with tick
}

void process_event(game& g, const sf::Event& event)
{

  for (const auto& controller:
    {
      get_physical_controller(g, side::lhs),
      get_physical_controller(g, side::rhs),
    }
  )
  {
     for (const auto a: controller.process_input(event, g))
     {
       add_user_input(g, a);
     }
  }
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
  show_sidebar(*this, side::lhs);
  show_sidebar(*this, side::rhs);

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
  show_possible_moves(view);
  show_unit_paths(view);
  show_pieces(view);
  show_unit_health_bars(view);
}

void show_controls(game_view& view, const side player)
{
  const auto& layout = view.get_game().get_layout();
  const auto player_color{get_player_color(view, player)};
  const std::vector<sf::Color> colors{
    sf::Color(255,  0,  0),
    sf::Color(255,128,  0),
    sf::Color(255,255,  0),
    sf::Color(128,255,  0)
  };
  // Determine maybe_mouse_user_selector
  std::optional<action_number> maybe_mouse_user_selector;
  {
    if (has_mouse_controller(view.get_game().get_controller())
      && player == get_mouse_user_player_side(view.get_game().get_controller()))
    {
      maybe_mouse_user_selector = view.get_game().get_controller().get_mouse_user_selector();
    }
  }
  const bool is_mouse_user{maybe_mouse_user_selector.has_value()};
  std::vector<std::optional<piece_action_type>> maybe_actions(4);
  {
    // The default action at key 1
    const std::optional<piece_action_type> maybe_first_action{
      get_default_piece_action(view.get_game(), player)
    };
    if (maybe_first_action)
    {
      maybe_actions[0] = maybe_first_action.value();
      if (maybe_first_action.value() == piece_action_type::promote_to_queen)
      {
         maybe_actions[1] = piece_action_type::promote_to_rook;
         maybe_actions[2] = piece_action_type::promote_to_bishop;
         maybe_actions[3] = piece_action_type::promote_to_knight;
      }
    }
  }
  std::vector<std::string> key_descriptions(4);
  if (is_mouse_user)
  {
    key_descriptions[maybe_mouse_user_selector.value().get_number() - 1] = "LMB";
    key_descriptions[get_next(maybe_mouse_user_selector.value()).get_number() - 1] = "RMB";
  }
  else
  {
    key_descriptions[0] = to_one_char_str(view.get_controller(player).get_key_bindings().get_key_for_action(action_number(1)));
    key_descriptions[1] = to_one_char_str(view.get_controller(player).get_key_bindings().get_key_for_action(action_number(2)));
    key_descriptions[2] = to_one_char_str(view.get_controller(player).get_key_bindings().get_key_for_action(action_number(3)));
    key_descriptions[3] = to_one_char_str(view.get_controller(player).get_key_bindings().get_key_for_action(action_number(4)));
  }

  for (const auto& number: get_all_action_numbers())
  {
    const int key{number.get_number()};
    // Background tile
    sf::RectangleShape background;
    set_rect(background, layout.get_controls_key(player, number));
    background.setFillColor(colors[key - 1]);
    background.setOutlineThickness(1);
    background.setOutlineColor(sf::Color::White);
    view.get_window().draw(background);

    // Determine the piece action type
    const auto& maybe_action{maybe_actions[key - 1]};

    // Tile that is a sprite
    const bool show_icon{true};
    if (show_icon)
    {
      sf::RectangleShape sprite;
      set_rect(sprite, layout.get_controls_key_icon(player, number));
      if (maybe_action)
      {
        const piece_action_type action{maybe_action.value()};
        sprite.setTexture(
          &view.get_resources().get_textures().get_action_icon(player_color, action)
        );
        view.get_window().draw(sprite);
      }
    }
    const bool show_input{true};
    if (show_input)
    {
      // Draw user input text
      sf::RectangleShape text_background;
      const screen_rect corner_rect{
        layout.get_controls_key_input(player, number)
      };
      set_rect(text_background, corner_rect);
      text_background.setFillColor(sf::Color::Black);
      text_background.setOutlineThickness(1);
      text_background.setOutlineColor(sf::Color::White);
      view.get_window().draw(text_background);

      // The text there
      sf::Text text;
      //text.setFont(view.get_resources().get_futuristic_font());
      text.setFont(view.get_resources().get_arial_font());
      text.setFillColor(colors[key - 1]);
      text.setString(key_descriptions[key - 1]);
      text.setCharacterSize(get_height(corner_rect) * 2 / 3);
      text.setPosition(
        corner_rect.get_tl().get_x() + 2,
        corner_rect.get_tl().get_y() + 2
      );
      view.get_window().draw(text);
    }
    const bool show_name{true};
    if (show_name)
    {
      // Draw name of the action
      sf::RectangleShape text_background;
      const screen_rect half_rect{
        layout.get_controls_key_name(player, number)
      };
      set_rect(text_background, half_rect);
      text_background.setFillColor(sf::Color::Black);
      text_background.setOutlineThickness(1);
      text_background.setOutlineColor(sf::Color::White);
      view.get_window().draw(text_background);

      // The text there
      sf::Text text;
      //text.setFont(view.get_resources().get_futuristic_font());
      text.setFont(view.get_resources().get_arial_font());
      text.setFillColor(colors[key - 1]);
      if (maybe_action)
      {
        const std::string s{to_str(maybe_action.value())};
        text.setString(s);
        const int font_size{
          get_height(half_rect) * 2 / 3
        };
        assert(font_size > 0);
        text.setCharacterSize(font_size);
      }
      text.setPosition(
        half_rect.get_tl().get_x() + 2,
        half_rect.get_tl().get_y() + 2
      );
      view.get_window().draw(text);
    }
  }
  // 46: for the mouse player, draw the selected active action
  if (is_mouse_user)
  {
    const auto number{maybe_mouse_user_selector.value()};
    // Re-use background tile
    sf::RectangleShape selector_square;
    set_rect(selector_square, layout.get_controls_key(player, number));
    selector_square.setFillColor(sf::Color::Transparent);
    selector_square.setOutlineThickness(5);
    selector_square.setOutlineColor(sf::Color::White);
    view.get_window().draw(selector_square);
  }
}

void show_debug(game_view& view, const side player_side)
{
  const auto& game{view.get_game()};
  const auto& layout{game.get_layout()};
  sf::Text text;
  text.setFont(view.get_resources().get_arial_font());
  const piece& closest_piece{
    get_closest_piece_to(game, get_cursor_pos(game, player_side))
  };

  const auto color{get_player_color(view, player_side)};
  std::stringstream s;
  s << "Color: " << color << '\n'
    << "Controller type: " << get_physical_controller_type(view, player_side) << '\n'
    << "Game position: "
    << to_notation(get_cursor_pos(game, player_side))
    << " "
    << get_cursor_pos(game, player_side)
    << '\n'
    << "Screen position: "
    << convert_to_screen_coordinat(get_cursor_pos(game, player_side), layout)
    << '\n'
    << "Is there a piece here: "
    << bool_to_str(is_piece_at(game, get_cursor_pos(game, player_side), 0.5))
    << '\n'
    << "Closest piece: " << closest_piece.get_type() << ": " << to_coordinat(closest_piece.get_current_square()) << '\n'
    << "Number of game actions: " << count_user_inputs(game) << '\n'
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
    layout.get_debug(player_side).get_tl().get_x(),
    layout.get_debug(player_side).get_tl().get_y()
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
      get_cursor_pos(m_game, side::rhs),
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
  text.setFont(view.get_resources().get_arial_font());
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
    // Transparency effect when moving
    if (!piece.get_actions().empty()
      && piece.get_actions()[0].get_action_type() == piece_action_type::move
    )
    {
      const double f{piece.get_current_action_time().get()};
      int alpha{0};
      if (f < 0.5)
      {
        alpha = 255 - static_cast<int>(f * 255.0);
      }
      else
      {
        alpha = static_cast<int>(f * 255.0);
      }
      sprite.setFillColor(sf::Color(255, 255, 255, alpha));
    }
    else
    {
      //sprite.setFillColor(sf::Color(0, 0, 0, 255));
    }
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

void show_possible_moves(game_view& view)
{
  const auto& g{view.get_game()};
  const auto& layout{g.get_layout()};
  const auto actions{collect_all_piece_actions(g)};
  for (const auto& action: actions)
  {
    if (!get_piece_at(g, action.get_from()).is_selected()) continue;
    sf::RectangleShape rectangle;
    set_rect(
      rectangle,
      convert_to_screen_rect(action.get_to(), layout)
    );
    rectangle.setOutlineColor(
      to_sfml_color(
        action.get_color(),
        action.get_action_type()
      )
    );
    rectangle.setOutlineThickness(get_square_width(layout) / 10.0);
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setScale(0.5, 0.5);
    rectangle.setRotation(action.get_color() == chess_color::white ? 30 : -30);
    view.get_window().draw(rectangle);
  }
}

void show_sidebar(game_view& view, const side player_side)
{
  show_unit_sprites(view, player_side);
  show_controls(view, player_side);
  show_log(view, player_side);
  show_debug(view, player_side);
}

void show_squares(game_view& view)
{
  show_squares(
    view.get_window(),
    view.get_game().get_layout().get_board(),
    view.get_resources()
  );
}

void show_square_under_cursor(
  game_view& view,
  const side player)
{
  const auto& game = view.get_game();
  const auto& layout = game.get_layout();
  const int x{static_cast<int>(std::trunc(get_cursor_pos(game, player).get_x()))};
  if (x < 0 || x >= 8) return;
  const int y{static_cast<int>(std::trunc(get_cursor_pos(game, player).get_y()))};
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
    s.setOutlineThickness(std::max(4, static_cast<int>(square_height / 10)));
  }
  else
  {
    s.setOutlineThickness(std::max(2, static_cast<int>(square_height / 20)));
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
    const auto& actions{piece.get_actions()};
    for (const auto& action: actions)
    {
      const auto from_pixel{
        convert_to_screen_coordinat(
          to_coordinat(action.get_from()),
          layout
        )
      };
      const auto to_pixel{
        convert_to_screen_coordinat(
          to_coordinat(action.get_to()),
          layout
        )
      };
      const auto center_pixel{(to_pixel + from_pixel) / 2.0};
      const double length{calc_distance(from_pixel, to_pixel)};
      const double angle_degrees{calc_angle_degrees(center_pixel, to_pixel)};
      sf::RectangleShape rect;
      const double max_square_height{get_square_width(layout)};
      const double height{std::max(2.0, max_square_height * 0.05)};
      rect.setSize(sf::Vector2f(length, height));
      rect.setOrigin(length / 2, height / 2);
      rect.setPosition(
        center_pixel.get_x(),
        center_pixel.get_y()
      );
      rect.rotate(-angle_degrees);
      rect.setOutlineColor(to_sfml_color(get_other_color(piece.get_color())));
      rect.setOutlineThickness(2);
      rect.setFillColor(to_sfml_color(piece.get_color()));
      view.get_window().draw(rect);
    }



    // Collect all the coordinats for the path
    std::vector<screen_coordinat> coordinats;
    coordinats.reserve(piece.get_actions().size() + 1); // +1 for current position
    coordinats.push_back(
      convert_to_screen_coordinat(
        to_coordinat(piece.get_current_square()),
        layout
      )
    );
    std::transform(
      std::begin(actions),
      std::end(actions),
      std::back_inserter(coordinats),
      [layout](const auto& user_input)
      {
        return convert_to_screen_coordinat(
          to_coordinat(user_input.get_to()),
          layout
        );
      }
    );

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
          to_coordinat(first_action.get_from()),
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

void show_unit_sprites(game_view& view, const side player_side)
{
  const auto& layout = view.get_game().get_layout();
  const double square_width{get_square_width(layout)};
  const double square_height{get_square_height(layout)};
  const auto player_color{get_player_color(view, player_side)};
  screen_coordinat screen_position = layout.get_units(player_side).get_tl();

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
    text.setFont(view.get_resources().get_arial_font());
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
