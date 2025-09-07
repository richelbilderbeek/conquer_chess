#include "menu_view.h"

#ifndef LOGIC_ONLY

#include "draw.h"
#include "game_resources.h"
#include "game_options.h"
#include "render_window.h"
#include "screen_coordinate.h"
#include "sfml_helper.h"

#include <SFML/Window/Window.hpp>
#include <SFML/Window/ContextSettings.hpp>

#include <cassert>
#include <cmath>

menu_view::menu_view(
) : m_background_image_index{
      create_seedless_random_background_image_index()
    },
    m_selected{menu_view_item::start}
{
  m_controls_bar.set_draw_player_controls(false);
}

int create_random_background_image_index(
  std::default_random_engine& rng_engine
) {
  std::uniform_int_distribution<int> distribution{
    0,
    4 - 1 // -1 as inclusive
  };
  const auto i{distribution(rng_engine)};
  assert(i >= 0);
  assert(i < 4);
  return i;
}

int create_seedless_random_background_image_index() {
  std::random_device device;
  std::default_random_engine rng_engine(device());
  return create_random_background_image_index(rng_engine);
}


void menu_view::draw_impl()
{
  draw_background_image(*this);

  draw_menu_outline(*this);

  draw_layout_panels(*this);

  draw_title_panel(*this);
  draw_subtitle_panel(*this);
  draw_start_panel(*this);
  draw_options_panel(*this); // Also draws the sub-menu if needed
  draw_about_panel(*this);
  draw_quit_panel(*this);
  m_controls_bar.draw();

  switch (this->get_selected())
  {
    case menu_view_item::about: break;
    case menu_view_item::laws:
    case menu_view_item::controls:
    case menu_view_item::video_and_audio:
    case menu_view_item::options:
      draw_options_sub_menu(*this);
      break;
    case menu_view_item::quit: break;
    case menu_view_item::start: break;
  }

  draw_selected_panel(*this);

}

void draw_about_panel(menu_view& v)
{
  draw_about_button(v.get_layout().get_about());
}

void draw_background_image(menu_view& v)
{
  const int index{v.get_background_image_index()};
  const int n{index + 1};
  draw_texture(
    game_resources::get().get_artwork_textures().get_all_races(n),
    v.get_layout().get_background_image()
  );
}


void draw_menu_outline(menu_view& v)
{
  draw_outline(v.get_layout().get_menu_panel());
}

void draw_options_sub_menu(menu_view& )
{

}

void draw_options_panel(menu_view& v)
{
  draw_options_button(v.get_layout().get_options());

  if (v.get_selected() == menu_view_item::options || is_in_options_submenu(v.get_selected()))
  {
    draw_outline(v.get_layout().get_sub_menu_panel());
    draw_options_controls_button(v.get_layout().get_options_controls());
    draw_options_laws_button(v.get_layout().get_options_laws());
    draw_options_video_and_audio_button(v.get_layout().get_options_video_and_audio());
  }
}

void draw_layout_panels(menu_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    get_render_window().draw(rectangle);
  }

}

void draw_quit_panel(menu_view& v)
{
  draw_quit_button(v.get_layout().get_quit());
}

void draw_selected_panel(menu_view& v)
{
  draw_outline(v.get_layout().get_selectable_rect(v.get_selected()));
}

void draw_start_panel(menu_view& v)
{
  draw_start_button(v.get_layout().get_start());
}

void draw_subtitle_panel(menu_view& v)
{
  draw_texture(
    get_subtitle_texture(),
    v.get_layout().get_subtitle()
  );
}

void draw_title_panel(menu_view& v)
{
  draw_texture(
    get_title_texture(),
    v.get_layout().get_title()
  );
}

bool menu_view::process_event_impl(sf::Event& event)
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
    else if (key_pressed == sf::Keyboard::Key::F4)
    {
      return true; // Close the program
    }
  }

  if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::Up)
    {
      set_selected(get_up(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Right)
    {
      set_selected(get_right(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Down)
    {
      set_selected(get_down(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Left)
    {
      set_selected(get_left(m_selected));
    }
    else if (key_pressed == sf::Keyboard::Key::Space
      || key_pressed == sf::Keyboard::Key::Return
    )
    {
      if (m_selected == menu_view_item::start)
      {
        set_next_state(program_state::lobby);
      }
      else if (m_selected == menu_view_item::options)
      {
        set_next_state(program_state::options);
      }
      else if (m_selected == menu_view_item::about)
      {
        set_next_state(program_state::about);
      }
      else if (m_selected == menu_view_item::quit)
      {
        return true;
      }
    }
    else if (key_pressed == sf::Keyboard::Key::A)
    {
      set_next_state(program_state::about);
    }
    else if (key_pressed == sf::Keyboard::Key::O)
    {
      set_next_state(program_state::options);
    }
    else if (key_pressed == sf::Keyboard::Key::S)
    {
      set_next_state(program_state::lobby);
    }
    else if (key_pressed == sf::Keyboard::Key::Q)
    {
      return true;
    }
    else if (key_pressed == sf::Keyboard::Key::F1)
    {
      // Developer shortcut
      set_next_state(program_state::game);
    }
    else if (key_pressed == sf::Keyboard::Key::F2)
    {
      // Developer shortcut
      set_next_state(program_state::replay);
    }
    else if (key_pressed == sf::Keyboard::Key::F7)
    {
      assert(1 == 2);
    }
  }
  if (event.type == sf::Event::MouseMoved)
  {
    const auto mouse_screen_pos{
      screen_coordinate(event.mouseMove.x, event.mouseMove.y)
    };
    if (is_in(mouse_screen_pos, m_layout.get_start())) set_selected(menu_view_item::start);
    else if (is_in(mouse_screen_pos, m_layout.get_options())) set_selected(menu_view_item::options);
    else if (is_in(mouse_screen_pos, m_layout.get_about())) set_selected(menu_view_item::about);
    else if (is_in(mouse_screen_pos, m_layout.get_quit())) set_selected(menu_view_item::quit);
  }
  else if (event.type == sf::Event::MouseButtonPressed)
  {
    if (event.mouseButton.button == sf::Mouse::Left)
    {
      const auto mouse_screen_pos{
        screen_coordinate(event.mouseButton.x, event.mouseButton.y)
      };
      if (is_in(mouse_screen_pos, m_layout.get_start()))
      {
        set_next_state(program_state::lobby);
      }
      else if (is_in(mouse_screen_pos, m_layout.get_options()))
      {
        set_next_state(program_state::options);
      }
      else if (is_in(mouse_screen_pos, m_layout.get_about()))
      {
        set_next_state(program_state::about);

      }
      else if (is_in(mouse_screen_pos, m_layout.get_quit()))
      {
        return true;
      }
    }
  }
  return false;
}

void menu_view::process_resize_event_impl(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  const screen_rect w(
    screen_coordinate(0, 0),
    screen_coordinate(event.size.width, event.size.height)
  );
  m_layout = menu_view_layout(
    w,
    get_default_margin_width()
  );
  m_controls_bar.set_screen_rect(w);
}

void menu_view::set_selected(const menu_view_item i)
{
  if (m_selected != i)
  {
    game_resources::get().get_sound_effects().play_hide();
  }
  m_selected = i;
}

void menu_view::start_impl()
{
  get_render_window().setTitle("Conquer Chess: Main Menu");
  game_resources::get().get_songs().get_bliss().setVolume(
    get_music_volume_as_percentage(m_game_options)
  );
  game_resources::get().get_sound_effects().set_master_volume(
    m_game_options.get_sound_effects_volume()
  );
  game_resources::get().get_songs().get_bliss().play();
  assert(!is_active());
  set_is_active(true);
}

void menu_view::stop_impl()
{
  assert(is_active());
  clear_next_state();
  set_is_active(false);
}

void menu_view::tick_impl(const delta_t)
{
  assert(is_active());
  // Nothing to do yet
}

#endif // LOGIC_ONLY
