#include "lobby_view.h"

#ifndef LOGIC_ONLY

#include "render_window.h"
#include "screen_coordinate.h"

#include "draw.h"
#include "draw_board.h"
#include "board_layout.h"
#include "lobby_options.h"
#include "game_options.h"
#include "game_resources.h"
#include "render_window.h"
#include "sfml_helper.h"

#include <cassert>
#include <cmath>
#include <iostream>

lobby_view::lobby_view()
  : m_lhs_cursor{lobby_view_item::color},
    m_lhs_start{false},
    m_rhs_cursor{lobby_view_item::color},
    m_rhs_start{false}
{
  m_controls_bar.set_draw_up_down(false);
  m_controls_bar.set_draw_select(false);
}

void lobby_view::tick_impl(const delta_t)
{
  assert(is_active());
  if (m_clock)
  {
    if (m_clock.value().getElapsedTime().asSeconds() > m_countdown_secs)
    {
      // Store to the lobby options, as these settings are accepted
      m_has_accepted = true;

      // Move on
      set_next_state(program_state::game);
    }
  }
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

bool lobby_view::process_event_impl(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
    set_next_state(program_state::main_menu);
    return false;
  }
  else if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      set_next_state(program_state::main_menu);
      return false;
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
        case lobby_view_item::start:
          m_lhs_start = !m_lhs_start;
          break;
      }
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::W)
    {
      m_lhs_cursor = get_previous(m_lhs_cursor);
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::S)
    {
      m_lhs_cursor = get_next(m_lhs_cursor);
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::T)
    {
      m_dog_mode = !m_dog_mode;
      if (m_dog_mode)
      {
        game_resources::get().get_sound_effects().play_bark(1);
      }
      else
      {
        game_resources::get().get_sound_effects().play_bark(2);
      }
    }
    else if (key_pressed == sf::Keyboard::Key::O)
    {
      m_rhs_cursor = get_previous(m_rhs_cursor);
      game_resources::get().get_sound_effects().play_hide();
    }
    else if (key_pressed == sf::Keyboard::Key::L)
    {
      m_rhs_cursor = get_next(m_rhs_cursor);
      game_resources::get().get_sound_effects().play_hide();
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
        case lobby_view_item::start:
          m_rhs_start = !m_rhs_start;
          break;
      }
      game_resources::get().get_sound_effects().play_hide();
    }
  }
  if (m_lhs_start && m_rhs_start && !m_clock.has_value())
  {
    m_clock = sf::Clock();
    game_resources::get().get_sound_effects().play_countdown();
  }
  else if (!(m_lhs_start && m_rhs_start) && m_clock.has_value())
  {
    m_clock = {};
  }

  return false; // Do not close the lobby :-)
}

void lobby_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  const screen_rect window_rect(
    screen_coordinate(0, 0),
    screen_coordinate(event.size.width, event.size.height)
  );
  m_layout = lobby_view_layout(
    window_rect,
    get_default_margin_width()
  );
  m_controls_bar.set_screen_rect(window_rect);
}

void lobby_view::draw_impl()
{
  draw_background(*this);
  draw_layout_panels(*this);
  draw_lobby_title(*this);
  draw_color_panel(*this, side::lhs);
  draw_king_portrait(*this, side::lhs);
  draw_race_panel(*this, side::lhs);
  draw_ready_panel(*this, side::lhs);
  draw_color_panel(*this, side::rhs);
  draw_king_portrait(*this, side::rhs);
  draw_race_panel(*this, side::rhs);
  draw_ready_panel(*this, side::rhs);
  draw_selected_panel(*this, side::lhs);
  draw_selected_panel(*this, side::rhs);

  m_controls_bar.draw();

  if (m_clock)
  {
    const int n_left{
      m_countdown_secs
      - static_cast<int>(
        m_clock.value().getElapsedTime().asSeconds()
      )
    };
    draw_countdown(*this, n_left);
  }

}

void draw_color_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_color(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_lobby_menu_textures().get_color(
      v.get_lobby_options().get_color(player_side)
    )
  );
  get_render_window().draw(rectangle);

  draw_fancy_text(to_human_str(v.get_lobby_options().get_color(player_side)), screen_rect, 64, 60);
}

void draw_countdown(lobby_view& v, const int n_left_secs)
{
  const screen_rect window_rect{
    v.get_layout().get_background()
  };
  draw_huge_fancy_text(std::to_string(n_left_secs), window_rect);
}

void draw_race_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_race(player_side)};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &game_resources::get().get_lobby_menu_textures().get_head(
      v.get_lobby_options().get_race(player_side)
    )
  );
  get_render_window().draw(rectangle);

  // Text
  sf::Text text;
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  const std::string s{to_human_str(v.get_lobby_options().get_race(player_side))};
  draw_fancy_text(s, text_rect, 64, 60);
}

void draw_background(lobby_view& v)
{
  const auto screen_rect{v.get_layout().get_background()};
  draw_texture(
    game_resources::get().get_map_textures().get_map(race::classic),
    //game_resources::get().get_textures().get_all_races_1(),
    screen_rect
  );
}

void draw_king_portrait(lobby_view& v, const side player_side)
{
  const auto player_color{v.get_lobby_options().get_color(player_side)};
  const auto player_race{v.get_lobby_options().get_race(player_side)};
  if (!v.get_dog_mode())
  {
    draw_texture(
      game_resources::get().get_piece_portrait_textures().get_portrait(
        player_race,
        player_color,
        piece_type::king
      ),
      v.get_layout().get_king_portrait(player_side)
    );
  }
  else
  {
    int index{
      (1 + static_cast<int>(player_color) + (1 + static_cast<int>(player_race)))
    };
    if (index > 5) index-=5;
    draw_texture(
      game_resources::get().get_themba_textures().get_texture(index),
      v.get_layout().get_king_portrait(player_side)
    );
  }
  sf::Color outline_color;
  switch (player_side)
  {
    case side::lhs: outline_color = sf::Color::Red; break;
    case side::rhs: outline_color = sf::Color::Blue; break;
  }
  draw_outline(
    v.get_layout().get_king_portrait(player_side),
    outline_color
  );
}

void draw_layout_panels(lobby_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    get_render_window().draw(rectangle);
  }
}

void draw_selected_panel(lobby_view& v, const side player_side)
{
  const auto select_rect{
    get_cursor_rect(
      v.get_layout(),
      v.get_selected(player_side),
      player_side
    )
  };
  sf::Color c;
  switch (player_side)
  {
    case side::lhs: c = sf::Color::Red; break;
    case side::rhs: c = sf::Color::Blue; break;
  }
  draw_outline(select_rect, c);
}

void draw_ready_panel(lobby_view& v, const side player_side)
{
  const auto screen_rect{v.get_layout().get_ready(player_side)};
  draw_texture(
    game_resources::get().get_lobby_menu_textures().get_ready(v.get_start(player_side)),
    screen_rect
  );

  // Text
  const auto text_rect{
    get_lower_half(screen_rect)
  };
  const bool is_ready{v.get_start(player_side)};
  const std::string s{is_ready ? "Ready" : "Not ready"};
  draw_fancy_text(s, text_rect, 64, 60);
}

void draw_lobby_title(lobby_view& v)
{
  const auto screen_rect{v.get_layout().get_title()};
  draw_squares(board_layout(screen_rect), true);
  draw_fancy_text("Lobby", screen_rect, 120, 116);

}

void lobby_view::start_impl()
{
  game_resources::get().get_songs().get_soothing().setVolume(
    get_music_volume_as_percentage(m_game_options)
  );
  game_resources::get().get_sound_effects().set_master_volume(
    m_game_options.get_sound_effects_volume()
  );
  game_resources::get().get_songs().get_soothing().play();
  if (m_clock.has_value())
  {
    m_clock.value().restart();
  }
  m_clock = {};
  m_lhs_cursor = lobby_view_item::color;
  m_rhs_cursor = lobby_view_item::color;
  m_lhs_start = false;
  m_rhs_start = false;
  m_has_accepted = false;
  assert(!is_active());
  set_is_active(true);
}

void lobby_view::stop_impl()
{
  assert(is_active());
  game_resources::get().get_songs().get_soothing().stop();
  if (m_clock.has_value())
  {
    m_clock.value().restart();
  }
  m_clock = {};
  m_lhs_start = false;
  m_rhs_start = false;
  clear_next_state();
  set_is_active(false);
}

#endif // LOGIC_ONLY
