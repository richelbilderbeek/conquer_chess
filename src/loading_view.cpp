#include "loading_view.h"
#include "game_resources.h"

#ifndef LOGIC_ONLY

#include "screen_coordinate.h"
#include "sfml_helper.h"
#include "render_window.h"

#include <cassert>
#include <cmath>

loading_view::loading_view()
{

}

void loading_view::tick_impl(const delta_t)
{
  assert(is_active());
  if (!m_resource_loader.is_done())
  {
    m_resource_loader.process_next();
    if (m_resource_loader.is_done()) {
      set_next_state(program_state::main_menu);

    }
  }
}

bool loading_view::process_event_impl(sf::Event& event)
{
  if (event.type == sf::Event::Closed)
  {
    return true; // Close the program
  }
  if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Escape)
    {
      return true; // Close the program
    }
  }
  return false;
}

void loading_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  // The draw method checks the screen size
}

void loading_view::set_text_style(sf::Text& text)
{
  text.setFont(get_arial_font());
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(44);
  text.setFillColor(sf::Color::White);
}

sf::Text get_loading_screen_text() noexcept
{
  sf::Text text;
  text.setFont(get_arial_font());
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(44);
  text.setFillColor(sf::Color::White);
  return text;
}

void loading_view::draw_impl()
{
  const auto window_size{get_render_window().getSize()};

  const screen_rect window_rect{
    screen_coordinate(0, 0),
    screen_coordinate(
      window_size.x, window_size.y
    )
  };
  // background
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, window_rect);
    rectangle.setTexture(
      &game_resources::get().get_loading_screen_textures().get_all_races(1)
    );
    get_render_window().draw(rectangle);
  }
  // loading text
  {
    sf::Text text;
    const auto text_rect{
      get_upper_half(get_lower_eighth(window_rect))
    };
    set_text_style(text);
    text.setString(m_resource_loader.get_current());
    set_text_position(text, text_rect);
    get_render_window().draw(text);
  }
  // progress bar rectangle background
  {
    sf::RectangleShape rectangle;
    const auto progress_bar_rect{
      get_lower_half(get_lower_eighth(window_rect))
    };
    set_rect(rectangle, progress_bar_rect);
    rectangle.setFillColor(sf::Color::White);
    get_render_window().draw(rectangle);
  }
  // progress bar rectangle
  {
    sf::RectangleShape rectangle;
    const auto full_progress_bar_rect{
      get_lower_half(get_lower_eighth(window_rect))
    };
    const double f_progress{get_progress(m_resource_loader)};
    const screen_rect progress_bar_rect{
      screen_rect(
        full_progress_bar_rect.get_tl(),
        screen_coordinate(
          full_progress_bar_rect.get_tl().get_x()
          + (f_progress * get_width(full_progress_bar_rect)),
          full_progress_bar_rect.get_br().get_y()
        )
      )
    };
    set_rect(rectangle, progress_bar_rect);
    rectangle.setFillColor(sf::Color::Black);
    get_render_window().draw(rectangle);
  }
}

void loading_view::start_impl()
{
  assert(!is_active());
  set_is_active(true);
}

void loading_view::stop_impl()
{
  assert(is_active());
  game_resources::get().get_loading_screen_songs().get_heroes().stop();
  clear_next_state();
  set_is_active(false);
}

void test_loading_view()
{
  loading_view v;
  assert(!v.get_next_state().has_value());
}

#endif // LOGIC_ONLY
