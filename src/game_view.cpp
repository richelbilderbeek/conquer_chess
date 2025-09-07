#include "game_view.h"

#ifndef LOGIC_ONLY

#include "board_layout.h"
#include "draw.h"
#include "draw_board.h"
#include "game.h"
#include "game_options.h"
#include "game_rect.h"
#include "game_resources.h"
#include "game_statistics_widget.h"
#include "game_view_layout.h"
#include "lobby_options.h"
#include "physical_controller.h"
#include "physical_controllers.h"
#include "replay.h"
#include "render_window.h"
#include "physical_controller.h"
#include "screen_coordinate.h"
#include "screen_rect.h"
#include "screen_rect.h"
#include "sfml_helper.h"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cassert>
#include <cmath>
#include <iterator>
#include <string>
#include <sstream>

game_view::game_view(
) : m_game_controller{},
    m_log{get_default_message_display_time_secs()},
    m_statistics_output_file("tmp.txt")
{
  m_controls_bar.set_draw_up_down(false);
  m_controls_bar.set_draw_select(false);
  m_controls_bar.set_draw_player_controls(false);
}


void game_view::tick_impl(delta_t dt)
{
  assert(is_active());

  //const game& g{m_game_controller.get_game()};

  const int n_logs_per_time_unit{8};
  if (static_cast<int>(get_in_game_time(m_game_controller).get() * n_logs_per_time_unit)
    < static_cast<int>((get_in_game_time(m_game_controller) + dt).get() * n_logs_per_time_unit))
  {
    m_statistics_output_file.add_to_file(m_game_controller);
    m_statistics_in_time.add(m_game_controller);
  }

  // Disard old messages
  m_log.tick();

  if (!get_winner(m_game_controller).has_value())
  {

    // Do a tick, so that one delta_t equals one second under normal game speed
    m_game_controller.tick(dt);

    // Read the pieces' messages and play their sounds
    process_piece_messages();
  }

  // Show the new state
  draw_impl();
}

const physical_controller& get_physical_controller(const game_view& view, const side player_side)
{
  return view.get_physical_controllers().get_controller(player_side);
}

physical_controller_type get_physical_controller_type(const game_view& view, const side player_side)
{
  return get_physical_controller(view, player_side).get_type();
}

double game_view::get_elapsed_time_secs() const noexcept
{
  return m_clock.getElapsedTime().asSeconds();
}


std::string get_last_log_messages(
  const game_view& view,
  const side player
) noexcept
{
  return get_last_log_messages(
    view.get_log(),
    view.get_game_controller().get_lobby_options().get_color(player)
  );
}

const game_view_layout& get_layout(const game_view& v) noexcept
{
  return v.get_layout();
}

const std::vector<piece>& get_pieces(const game_view& v) noexcept
{
  return get_pieces(v.get_game_controller().get_game());
}

replay game_view::get_replay() const
{
  return replay(
    create_action_history_from_game(m_game_controller.get_game()),
    m_game_controller
  );
}

void game_view::play_pieces_sound_effects()
{
  for (const auto& sound_effect: collect_messages(m_game_controller.get_game()))
  {
    game_resources::get().get_sound_effects().play(sound_effect);
  }
}

const game_coordinate& get_cursor_pos(const game_view& view, const side player) noexcept
{
  return get_cursor_pos(view.get_game_controller(), player);
}

std::vector<std::string> get_controls_texts(
  const game_controller& c,
  const side player_side
)
{
  const auto actions{get_piece_actions(c, player_side)};
  std::vector<std::string> texts;
  texts.reserve(actions.size());
  std::transform(
    std::begin(actions),
    std::end(actions),
    std::back_inserter(texts),
    [](const auto& a) { return to_human_str(a); }
  );
  assert(actions.size() == texts.size());
  return texts;
}


const in_game_time& get_time(const game_view& v) noexcept
{
  return get_time(v.get_game());
}

bool game_view::process_event_impl(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
      set_next_state(program_state::lobby);
      return false;
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      set_next_state(program_state::lobby);
      return false;
    }
    if (key_pressed == sf::Keyboard::Key::F7)
    {
      // Use a lower resolution
      m_layout = game_view_layout(
        screen_rect(
          screen_coordinate(0,0),
          screen_coordinate(1600, 900)
        )
      );
    }
  }

  // Become unresponsive when there is a winner
  if (!m_game_controller.get_game().get_winner().has_value())
  {
    ::process_event(m_game_controller, m_pc, event, m_layout);
    m_game_controller.apply_user_inputs_to_game();
  }
  return false;
}

void game_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  const screen_rect w(
    screen_coordinate(0, 0),
    screen_coordinate(event.size.width, event.size.height)
  );
  m_layout = game_view_layout(w);
  m_controls_bar.set_screen_rect(w);
}

void process_event(
  game_controller& c,
  const physical_controllers& pc,
  const sf::Event& event,
  const game_view_layout& layout
)
{
  for (const auto s: get_all_sides())
  {
    const physical_controller& p{pc.get_controller(s)};
    const user_inputs& inputs{p.process_input(event, s, layout)};
    for (const auto& a: inputs.get_user_inputs())
    {
      add_user_input(c, a);
    }
  }
}

void game_view::process_piece_messages()
{
  for (const auto& piece_message: collect_messages(m_game_controller.get_game()))
  {
    m_log.add_message(piece_message);
  }

  // Play the new sounds to be played
  play_pieces_sound_effects();

  clear_piece_messages(m_game_controller.get_game());
}

void game_view::draw_impl()
{
  // Show the layout of the screen: board and sidebars
  draw_background(*this);

  // Show the board: squares, unit paths, pieces, health bars
  draw_board(*this, m_game_options.get_show_occupied());

  // In-game statistics widget
  draw_game_statistics_widget(*this);

  // Show the sidebars: controls (with log), units, debug
  for (const side s: get_all_sides())
  {
    // Unit portrait, name, health, etc
    draw_unit_info(*this, s);

    // Navigational controls and action keys
    draw_controls(*this, s);

    // Show the messages
    draw_log(*this, s);

    if (m_game_options.get_show_debug_info())
    {
      show_debug(*this, s);
    }
  }

  // Draw winner
  if (m_game_controller.get_game().get_winner().has_value())
  {

    draw_text(
      "Winner: " + to_human_str(m_game_controller.get_game().get_winner().value()),
      m_layout.get_background(),
      200
    );
  }
  // General controls bar
  m_controls_bar.draw();
}

void draw_board(
  game_view& view,
  const bool show_occupied
)
{
  draw_squares(view);
  if (show_occupied)
  {
    show_occupied_squares(view);
  }
  draw_possible_moves(view);
  draw_unit_paths(view);
  draw_pieces(view);

  // Cursor goes over the pieces, under the health bar
  draw_cursor(view, side::lhs);
  draw_cursor(view, side::rhs);

  draw_unit_health_bars(view);


}

void draw_controls(
  game_view& view,
  const side player_side
)
{
  // Stub for keyboard only
  const auto& c{ view.get_physical_controllers().get_controller(player_side)};

  const auto& layout{view.get_layout().get_controls(player_side)};

  // Blur the entire background here
  draw_rectangle(
    layout.get_background(),
    sf::Color(128, 128, 128, 128)
  );


  draw_navigation_controls(
    view.get_physical_controllers(),
    layout.get_navigation_controls(),
    player_side
  );

  const std::vector<std::string> texts{
      get_controls_texts(view.get_game_controller(), player_side)
  };

  for (const auto n: get_all_action_numbers())
  {
    const screen_rect symbol_rect{layout.get_action_key_symbol(n)};
    if (c.get_type() == physical_controller_type::keyboard)
    {
      draw_input_prompt_symbol(
        view.get_physical_controllers().get_controller(player_side).get_key_bindings().get_key_for_action(n),
        symbol_rect
      );
    }
    else
    {
      if (n.get_number() < 3)
      {
        draw_input_prompt_symbol(
          view.get_physical_controllers().get_controller(player_side).get_mouse_bindings().get_button_for_action(n),
          symbol_rect
        );
      }
    }
    // Black can do nothing for one chess move
    if (view.get_game_controller().get_lobby_options().get_color(player_side) == chess_color::black
      && view.get_game().get_in_game_time() < in_game_time(1.0)
    ) continue;

    const int i{n.get_number() - 1};
    assert(i >= 0);
    if (i < static_cast<int>(texts.size()))
    {
      draw_text(
        texts[i],
        layout.get_action_key_text(n),
        26
      );
    }
  }
}

void draw_game_statistics_widget(game_view& view)
{
  draw_game_statistics_widget(
    view.get_layout().get_game_info(),
    view.get_game_controller()
  );
}

void draw_navigation_controls(game_view& view)
{
  for (const auto s: get_all_sides())
  {
    draw_navigation_controls(
      view.get_physical_controllers(),
      view.get_layout().get_navigation_controls(s),
      s
    );
  }

}

void show_debug(game_view& view, const side player_side)
{
  const auto& g{view.get_game()};
  const auto& c{view.get_game_controller()};
  const auto& layout{view.get_layout()};
  sf::Text text;
  text.setFont(game_resources::get().get_fonts().get_arial_font());
  const piece& closest_piece{
    get_closest_piece_to(g, get_cursor_pos(c, player_side))
  };

  std::vector<sf::String> texts =
  {
    "Game position: " + to_notation(get_cursor_pos(c, player_side)),
    "Screen position: " + to_str(convert_to_screen_coordinate(get_cursor_pos(c, player_side), layout)),
    "Cursor position: " + to_str(get_cursor_pos(c, player_side)),
    "Closest piece: " + to_str(closest_piece.get_type()) + ": " + to_str(to_coordinat(closest_piece.get_current_square()))
  };
  draw_texts(
    texts,
    layout.get_debug(player_side),
    20,
    sf::Color::White
  );
}

void game_view::show_mouse_cursor()
{
  const auto& layout = m_layout;

  sf::CircleShape cursor;
  cursor.setRadius(16.0);
  cursor.setFillColor(sf::Color::Transparent);
  cursor.setOutlineColor(sf::Color::Black);
  cursor.setOutlineThickness(2.0);
  cursor.setScale(1.0, 1.0);
  cursor.setOrigin(16.0, 16.0);
  const screen_coordinate cursor_pos{
    convert_to_screen_coordinate(
      get_cursor_pos(m_game_controller, side::rhs),
      layout
    )
  };
  cursor.setPosition(
    cursor_pos.get_x(),
    cursor_pos.get_y()
  );
  get_render_window().draw(cursor);
}

void draw_log(game_view& view, const side player)
{
  const auto& layout = view.get_layout();
  sf::Text text;
  text.setFont(game_resources::get().get_fonts().get_arial_font());
  std::stringstream s;
  s << get_last_log_messages(view, player);
  text.setString(s.str().c_str());
  text.setCharacterSize(20);
  text.setPosition(
    layout.get_log(player).get_tl().get_x(),
    layout.get_log(player).get_tl().get_y()
  );
  get_render_window().draw(text);
}

void draw_background(game_view& view)
{
  const auto& layout{view.get_layout()};
  draw_texture(
    get_map_texture(view.get_game_controller().get_lobby_options().get_race(chess_color::white)),
    screen_rect(layout.get_background())
  );
}


void show_occupied_squares(game_view& view)
{
  assert(view.get_game_options().get_show_occupied());
  const auto& pieces{get_pieces(view)};
  for (const auto& piece: pieces)
  {
    const auto& square{get_occupied_square(piece)};
    const screen_rect square_rect{
      convert_to_screen_rect(
        game_rect{to_game_rect(square)},
        get_layout(view)
      )
    };
    sf::RectangleShape s;
    set_rect(s, square_rect);
    s.setTexture(
      &game_resources::get().get_textures().get_occupied_square(
        to_color(square),
        piece.get_color()
      )
    );
    get_render_window().draw(s);
  }
}

void draw_pieces(game_view& view)
{
  const bool indicate_protectedness{true};
  draw_pieces(
    view.get_game_controller(),
    view.get_layout().get_board(),
    indicate_protectedness
  );

}

void draw_possible_moves(game_view& view)
{
  const auto& g{view.get_game()};
  const auto& c{view.get_game_controller()};
  const auto& layout{view.get_layout()};
  const auto actions{collect_all_piece_actions(g)};
  for (const auto& action: actions)
  {
    if (!is_selected(get_piece_at(g, action.get_from()), c)) continue;
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
    get_render_window().draw(rectangle);
  }
}


void draw_squares(game_view& view)
{
  draw_squares(view.get_layout().get_board().get_board());
}

void draw_cursor(
  game_view& view,
  const side player
)
{
  const auto& c{view.get_game_controller()};
  const auto& layout{view.get_layout()};
  const int x{
    static_cast<int>(std::trunc(get_cursor_pos(c, player).get_x()))
  };
  if (x < 0 || x >= 8) return;
  const int y{
    static_cast<int>(std::trunc(get_cursor_pos(c, player).get_y()))
  };
  if (y < 0 || y >= 8) return;

  assert(x >= 0 && x < 8 && y >= 0 && y < 8);
  sf::RectangleShape s;
  const double square_width{get_square_width(layout)};
  const double square_height{get_square_height(layout)};
  s.setSize(sf::Vector2f(square_width + 1, square_height + 1));
  s.setOrigin(sf::Vector2f(square_width / 2.0, square_height / 2.0));
  const screen_coordinate square_pos{
    convert_to_screen_coordinate(
      game_coordinate(x + 0.5, y + 0.5),
      layout
    )
  };
  s.setPosition(square_pos.get_x(), square_pos.get_y());
  const auto old_fill_color = s.getFillColor();
  const auto old_outline_color = s.getOutlineColor();
  const auto old_thickness = s.getOutlineThickness();
  const auto player_color{view.get_game_controller().get_lobby_options().get_color(player)};
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
  get_render_window().draw(s);
  s.setFillColor(old_fill_color);
  s.setOutlineColor(old_outline_color);
  s.setOutlineThickness(old_thickness);
}

void draw_unit_health_bars(game_view& view)
{
  draw_unit_health_bars(
    view.get_game(),
    view.get_layout().get_board()
  );
}


void draw_unit_paths(game_view& view)
{
  draw_unit_paths(
    view.get_game().get_pieces(),
    view.get_layout().get_board()
  );
}

void draw_unit_info(game_view& view, const side player_side)
{
  const auto& layout{view.get_layout()};
  const auto& r{layout.get_unit_info(player_side)};
  const auto& c{view.get_game_controller()};
  const auto player_color{view.get_game_controller().get_lobby_options().get_color(player_side)};

  const auto selected_pieces{get_selected_pieces(c, player_color)};
  if (selected_pieces.empty()) return;
  assert(selected_pieces.size() == 1);
  const auto& piece{selected_pieces[0]};

  // sprite of the piece
  draw_texture(get_piece_portrait_texture(
      piece.get_race(),
      piece.get_color(),
      piece.get_type()
    ),
    r
  );
  // text
  std::stringstream s;
  s << piece.get_type() << ": "
    << piece.get_health() << "/"
    << piece.get_max_health() << ", "
    << piece.get_current_square() << ", "
    << describe_actions(piece)
  ;
  const auto text_rect{
    screen_rect(
      screen_coordinate(r.get_tl().get_x(), r.get_br().get_y() - 32),
      screen_coordinate(r.get_br())
    )
  };
  draw_rectangle(text_rect, sf::Color(128, 128, 128, 128));
  draw_text(s.str(), text_rect, 20);
}

void game_view::set_lobby_options(const lobby_options& lo) noexcept
{
  m_lobby_options = lo;
}


void game_view::start_impl()
{
  m_clock.restart();
  game_resources::get().get_songs().get_wonderful_time().setVolume(
    get_music_volume_as_percentage(m_game_options)
  );
  game_resources::get().get_sound_effects().set_master_volume(
    get_sound_effects_volume(m_game_options)
  );
  m_game_controller = game_controller(
    create_game_with_starting_position(
      m_game_options.get_starting_position(),
      m_lobby_options.get_race(side::lhs),
      m_lobby_options.get_race(side::rhs)
    ),
    m_lobby_options
  );
  assert(!is_active());
  set_is_active(true);

}

void game_view::stop_impl()
{
  assert(is_active());
  m_clock.restart();
  game_resources::get().get_songs().get_wonderful_time().stop();
  clear_next_state();
  set_is_active(false);
}

void test_game_view() //!OCLINT tests may be many
{
  #ifndef NDEBUG // no tests in release
  #endif //NDEBUG
}

bool would_be_valid(
  const game_view& view,
  chess_color player_color
)
{
  const auto& c{view.get_game_controller()};
  return can_player_select_piece_at_cursor_pos(c, player_color);
}

#endif // LOGIC_ONLY
