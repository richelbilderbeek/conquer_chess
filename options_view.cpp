#include "options_view.h"

#ifndef LOGIC_ONLY

#include "options_view.h"

#include "screen_coordinat.h"
#include "game_view.h"
#include "sfml_helper.h"

#include <cassert>
#include <iostream>
#include <sstream>

options_view::options_view(const game_options& options)
  : m_options{options},
    m_selected{options_view_item::game_speed}
{

}

void options_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_screen_size().get_x(),
      get_default_screen_size().get_y()
    ),
    "Conquer Chess: options menu"
  );
  while (m_window.isOpen())
  {
    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Show the current options on-screen
    assert(!to_str(get_starting_position(*this)).empty());
    show();
    assert(!to_str(get_starting_position(*this)).empty());
  }
}

starting_position_type get_starting_position(const options_view& v) noexcept
{
  return get_starting_position(v.get_options());
}



bool options_view::process_events()
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
      m_layout = options_view_layout(
        screen_coordinat(visibleArea.width, visibleArea.height),
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
      if (key_pressed == sf::Keyboard::Key::Q)
      {
        m_window.close();
        return true;
      }
      else if (key_pressed == sf::Keyboard::Key::Up)
      {
        m_selected = get_above(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Right)
      {
        m_selected = get_right_of(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Down)
      {
        m_selected = get_below(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Left)
      {
        m_selected = get_left_of(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Space)
      {
        switch (m_selected)
        {
          case options_view_item::game_speed:
            m_options.set_game_speed(get_next(m_options.get_game_speed()));
          break;
          case options_view_item::music_volume:
            m_options.set_volume(get_next(m_options.get_music_volume()));
          break;
          case options_view_item::starting_position:
            assert(!to_str(get_starting_position(*this)).empty());
            m_options.set_starting_position(get_next(get_starting_position(m_options)));
            assert(!to_str(get_starting_position(*this)).empty());
          break;
          case options_view_item::left_color:
          case options_view_item::right_color:
            m_options.set_left_player_color(get_other_color(m_options.get_left_player_color()));
          break;
          case options_view_item::left_controls:
            m_options.set_left_controller_type(get_next(m_options.get_left_controller_type()));
          break;
          case options_view_item::right_controls:
            m_options.set_right_controller_type(get_next(m_options.get_right_controller_type()));
          break;
        }

      }
      else if (key_pressed == sf::Keyboard::Key::F3)
      {
        // debug
        std::clog << "Debug";
      }
    }
  }
  return false; // if no events proceed with tick
}

void options_view::set_text_style(sf::Text& text)
{
  text.setFont(get_font(get_resources()));
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(64);
  text.setFillColor(sf::Color::Black);
}

void options_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  show_panels(*this);

  assert(!to_str(get_starting_position(*this)).empty());
  show_top(*this);
  assert(!to_str(get_starting_position(*this)).empty());

  show_bottom(*this);
  show_selected_panel(*this);

  // Display all shapes
  m_window.display();

  assert(!to_str(get_starting_position(*this)).empty());
}

void show_bottom(options_view& v)
{
  show_bottom_header(v);
  show_bottom_left(v);
  show_bottom_right(v);
}

void show_bottom_header(options_view& v)
{
  const auto& layout = v.get_layout();
  {
    const auto& screen_rect = layout.get_player_label();
    sf::RectangleShape rectangle;
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::white)
    );
    set_rect(rectangle, screen_rect);
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString("Player");
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  {
    const auto& screen_rect = layout.get_color_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::black)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString("Color");
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  {
    const auto& screen_rect = layout.get_controls_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::white)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString("Controls");
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
}

void show_bottom_left(options_view& v)
{
  const auto& layout = v.get_layout();
  {
    const auto& screen_rect = layout.get_left_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::black)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString("Left");
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  {
    const auto& screen_rect = layout.get_left_color_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::white)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString(to_str(get_left_player_color(v.get_options())));
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  {
    const auto& screen_rect = layout.get_left_controls_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::black)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString(to_str(v.get_options().get_left_controller_type()));
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
}

void show_bottom_right(options_view& v)
{
  const auto& layout = v.get_layout();
  {
    const auto& screen_rect = layout.get_right_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::white)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString("Right");
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  {
    const auto& screen_rect = layout.get_right_color_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::black)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString(to_str(get_right_player_color(v.get_options())));
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  {
    const auto& screen_rect = layout.get_right_controls_value();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::white)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString(to_str(v.get_options().get_right_controller_type()));
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
}

void show_game_speed(options_view& v)
{
  const auto& layout = v.get_layout();
  // game speed label
  {
    const auto& screen_rect = layout.get_game_speed_label();
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::white)
    );
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString("Game speed");
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  // game speed value
  {
    const auto& screen_rect = layout.get_game_speed_value();
    sf::RectangleShape rectangle;
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::black)
    );
    set_rect(rectangle, screen_rect);
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);


    text.setString(to_str(v.get_options().get_game_speed()));
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
}

void show_starting_position(options_view& v)
{
  const auto& layout = v.get_layout();
  // starting pos label
  {
    const auto& screen_rect = layout.get_starting_pos_label();
    sf::RectangleShape rectangle;
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::white)
    );
    set_rect(rectangle, screen_rect);
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString("Starting position");
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  // starting pos value
  {
    const auto& screen_rect = layout.get_starting_pos_value();
    sf::RectangleShape rectangle;
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::black)
    );
    set_rect(rectangle, screen_rect);
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString(to_str(get_starting_position(v)));
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }


}

void show_top(options_view& v)
{
  assert(!to_str(get_starting_position(v)).empty());

  show_game_speed(v);
  show_music_volume(v);
  show_starting_position(v);

  assert(!to_str(get_starting_position(v)).empty());
}


void show_panels(options_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    v.get_window().draw(rectangle);
  }
}

void show_music_volume(options_view& v)
{
  const auto& layout = v.get_layout();
  // music volume label
  {
    const auto& screen_rect = layout.get_music_volume_label();
    sf::RectangleShape rectangle;
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::black)
    );
    set_rect(rectangle, screen_rect);
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    text.setString("Music volume");
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
  // music volume value
  {
    const auto& screen_rect = layout.get_music_volume_value();
    sf::RectangleShape rectangle;
    rectangle.setTexture(
      &get_strip(v.get_resources(), chess_color::white)
    );
    set_rect(rectangle, screen_rect);
    v.get_window().draw(rectangle);

    sf::Text text;
    v.set_text_style(text);
    std::stringstream s;
    s << get_music_volume(v.get_options()) << " %";
    text.setString(s.str());
    set_text_position(text, screen_rect);
    v.get_window().draw(text);
  }
}

void show_selected_panel(options_view& v)
{
  const auto select_rect{
    v.get_layout().get_selectable_rect(
      v.get_selected()
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

#endif // LOGIC_ONLY
