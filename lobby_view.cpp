#include "lobby_view.h"

#ifndef LOGIC_ONLY

#include "about_view.h"
#include "screen_coordinat.h"
#include "game_view.h"
#include "played_game_view.h"
#include "options_view.h"

#include "sfml_helper.h"
#include <cassert>
#include <cmath>
#include <iostream>

lobby_view::lobby_view(
  const game_options& go,
  const physical_controllers& pcs
)
  : m_game_options{go},
    m_lhs_cursor{lobby_view_item::color},
    m_lhs_start{false},
    m_lobby_options{create_default_lobby_options()},
    m_physical_controllers{pcs},
    m_rhs_cursor{lobby_view_item::color},
    m_rhs_start{false}
{
  m_resources.get_songs().get_soothing().setVolume(
    get_music_volume_as_percentage(m_game_options)
  );
  m_resources.get_sound_effects().set_master_volume(
    m_game_options.get_sound_effects_volume()
  );
  m_resources.get_songs().get_soothing().setLoop(true);
  m_resources.get_songs().get_soothing().play();

}

void lobby_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_lobby_screen_size().get_x(),
      get_default_lobby_screen_size().get_y()
    ),
    "Conquer Chess: lobby"
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

    // Show the new state
    show();

    if (m_clock)
    {
      if (m_clock.value().getElapsedTime().asSeconds() > m_countdown_secs)
      {
        m_clock = {};
        exec_game();
        m_lhs_start = false;
        m_rhs_start = false;
      }
    }

  }
}

void lobby_view::exec_game()
{
  const auto cur_pos{m_window.getPosition()};
  m_resources.get_songs().get_soothing().stop();
  m_window.setVisible(false);
  game_view view{
    game(m_game_options, m_lobby_options),
    game_controller(m_physical_controllers)
  };
  view.exec();
  m_window.setVisible(true);
  m_window.setPosition(cur_pos);
  m_resources.get_songs().get_soothing().play();
}


lobby_view_item lobby_view::get_selected(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_cursor;
  }
  assert(player_side == side::rhs);
  return m_rhs_cursor;
}

bool lobby_view::get_start(const side player_side) const noexcept
{
  if (player_side == side::lhs)
  {
    return m_lhs_start;
  }
  assert(player_side == side::rhs);
  return m_rhs_start;
}

bool lobby_view::process_events()
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

      m_layout = lobby_view_layout(
        screen_coordinat(event.size.width, event.size.height),
        get_default_margin_width()
      );
    }
    else if (event.type == sf::Event::Closed)
    {
      m_window.close();
      return true; // Close lobby
    }
    else if (event.type == sf::Event::KeyPressed)
    {
      sf::Keyboard::Key key_pressed = event.key.code;
      if (key_pressed == sf::Keyboard::Key::Escape)
      {
        m_window.close();
        return true;
      }
      else if (key_pressed == sf::Keyboard::Key::Q)
      {
        switch (m_lhs_cursor)
        {
          case lobby_view_item::color:
            m_lobby_options.set_color(
              get_next(m_lobby_options.get_color(side::lhs)),
              side::lhs
            );
            m_lhs_start = false;
            m_rhs_start = false;
            break;
          case lobby_view_item::race:
            m_lobby_options.set_race(
              get_next(m_lobby_options.get_race(side::lhs)),
              side::lhs
            );
            m_lhs_start = false;
            m_rhs_start = false;
            break;
          default:
          case lobby_view_item::start:
            assert(m_lhs_cursor == lobby_view_item::start);
            m_lhs_start = !m_lhs_start;
            break;
        }
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::W)
      {
        m_lhs_cursor = get_previous(m_lhs_cursor);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::S)
      {
        m_lhs_cursor = get_next(m_lhs_cursor);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::O)
      {
        m_rhs_cursor = get_previous(m_rhs_cursor);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::L)
      {
        m_rhs_cursor = get_next(m_rhs_cursor);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::I)
      {
        switch (m_rhs_cursor)
        {
          case lobby_view_item::color:
            m_lobby_options.set_color(
              get_next(m_lobby_options.get_color(side::rhs)),
              side::rhs
            );
            m_lhs_start = false;
            m_rhs_start = false;
            break;
          case lobby_view_item::race:
            m_lobby_options.set_race(
              get_next(m_lobby_options.get_race(side::rhs)),
              side::rhs
            );
            m_lhs_start = false;
            m_rhs_start = false;
            break;
          default:
          case lobby_view_item::start:
            assert(m_rhs_cursor == lobby_view_item::start);
            m_rhs_start = !m_rhs_start;
            break;
        }
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Q)
      {
        m_window.close();
        return true;
      }
    }
  }
  if (m_lhs_start && m_rhs_start && !m_clock.has_value())
  {
    m_clock = sf::Clock();
    m_resources.get_sound_effects().play_countdown();

  }
  else if (!(m_lhs_start && m_rhs_start) && m_clock.has_value())
  {
    m_clock = {};
  }

  return false; // Do not close the lobby :-)
}

void lobby_view::set_text_style(sf::Text& text)
{
  text.setFont(get_arial_font(get_resources()));
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(m_layout.get_font_size());
  text.setFillColor(sf::Color::Black);
}

void lobby_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  show_layout_panels(*this);

  show_image_panel(*this);
  show_color_panel(*this, side::lhs);
  show_race_panel(*this, side::lhs);
  show_start_panel(*this, side::lhs);
  show_color_panel(*this, side::rhs);
  show_race_panel(*this, side::rhs);
  show_start_panel(*this, side::rhs);
  show_selected_panel(*this, side::lhs);
  show_selected_panel(*this, side::rhs);
  if (m_clock)
  {
    const int n_left{
      m_countdown_secs
      - static_cast<int>(
        m_clock.value().getElapsedTime().asSeconds()
      )
    };
    show_countdown(*this, n_left);
  }

  // Display all shapes
  m_window.display();

}

void show_color_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_color(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &v.get_resources().get_lobby_menu_textures().get_color(
      v.get_options().get_color(player_side)
    )
  );
  v.get_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  std::string s{to_str(v.get_options().get_color(player_side))};
  s[0] = std::toupper(s[0]);
  text.setString(s);
  v.set_text_style(text);
  set_text_position(text, text_rect);
  v.get_window().draw(text);

  // Smaller
  text.setCharacterSize(text.getCharacterSize() - 2);
  set_text_position(text, text_rect);
  text.setFillColor(sf::Color::White);
  v.get_window().draw(text);
}

void show_countdown(lobby_view& v, const int n_left_secs)
{
  // Text
  const screen_rect window_rect{
    screen_coordinat(0, 0),
    v.get_layout().get_window_size()
  };
  sf::Text text;
  std::string s{std::to_string(n_left_secs)};
  text.setString(s);
  v.set_text_style(text);
  text.setCharacterSize(512);
  set_text_position(text, window_rect);
  v.get_window().draw(text);

  // Smaller
  text.setCharacterSize(512 - 2);
  set_text_position(text, window_rect);
  text.setFillColor(sf::Color::White);
  v.get_window().draw(text);

}

void show_race_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_race(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &v.get_resources().get_lobby_menu_textures().get_head(
      v.get_options().get_race(player_side)
    )
  );
  v.get_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  std::string s{to_str(v.get_options().get_race(player_side))};
  s[0] = std::toupper(s[0]);
  text.setString(s);
  v.set_text_style(text);
  set_text_position(text, text_rect);
  v.get_window().draw(text);

  // Smaller
  text.setCharacterSize(text.getCharacterSize() - 2);
  set_text_position(text, text_rect);
  text.setFillColor(sf::Color::White);
  v.get_window().draw(text);
}

void show_image_panel(lobby_view& v)
{
  const auto screen_rect{v.get_layout().get_image()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &v.get_resources().get_textures().get_all_races_1()
  );
  v.get_window().draw(rectangle);
}

void show_layout_panels(lobby_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    v.get_window().draw(rectangle);
  }
}

void show_selected_panel(lobby_view& v, const side player_side)
{
  const auto select_rect{
    get_cursor_rect(
      v.get_layout(),
      v.get_selected(player_side),
      player_side
    )
  };
  sf::RectangleShape rectangle;
  set_rect(rectangle, select_rect);
  rectangle.setOrigin(
    get_width(select_rect) / 2,
    get_height(select_rect) / 2
  );
  rectangle.setFillColor(sf::Color::Transparent);
  rectangle.setOutlineColor(sf::Color::Red);
  rectangle.setOutlineThickness(5);
  v.get_window().draw(rectangle);
}

void show_start_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_start(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &v.get_resources().get_lobby_menu_textures().get_ready(
      v.get_start(player_side)
    )
  );
  v.get_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  if (v.get_start(player_side))
  {
    text.setString("Ready");
  }
  else
  {
    text.setString("Not ready");
  }
  v.set_text_style(text);
  set_text_position(text, text_rect);
  v.get_window().draw(text);

  // Smaller
  text.setCharacterSize(text.getCharacterSize() - 2);
  set_text_position(text, text_rect);
  text.setFillColor(sf::Color::White);
  v.get_window().draw(text);
}

#endif // LOGIC_ONLY
