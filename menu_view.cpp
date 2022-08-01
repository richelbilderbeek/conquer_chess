#include "menu_view.h"

#ifndef LOGIC_ONLY

#include "screen_coordinat.h"
#include "game_view.h"
#include "options_view.h"

#include "sfml_helper.h"
#include <cassert>
#include <cmath>
#include <iostream>

menu_view::menu_view()
  : m_options{get_default_game_options()},
    m_selected{menu_view_item::start}
{
#ifdef DEMO_REPLAYER_ISSUE_22
  m_options.set_replayer(replayer(replay(get_scholars_mate_as_pgn_str())));
#endif // DEMO_REPLAYER_ISSUE_22
}

void menu_view::exec()
{
  // Open window
  m_window.create(
    sf::VideoMode(
      get_default_main_menu_screen_size().get_x(),
      get_default_main_menu_screen_size().get_y()
    ),
    "Conquer Chess: main menu"
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

  }
}

void menu_view::exec_game()
{
  game_view view{game(m_options)};
  view.exec();
}

void menu_view::exec_options()
{
  options_view view(m_options);
  assert(!to_str(get_starting_position(view)).empty());
  view.exec();
  assert(!to_str(get_starting_position(view)).empty());
  m_options = view.get_options();
}

bool menu_view::process_events()
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

      // From https://stackoverflow.com/a/38486530
      if (1 == 2 && "centering would really work")
      {
        m_window.setView(
          sf::View(
            m_window.getView().getCenter(),
            sf::Vector2f(event.size.width, event.size.height)
          )
        );
      }

      m_layout = menu_view_layout(
        screen_coordinat(event.size.width, event.size.height),
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
        m_selected = get_previous(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Right)
      {
        m_selected = get_next(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Down)
      {
        m_selected = get_next(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Left)
      {
        m_selected = get_previous(m_selected);
        m_resources.get_sound_effects().play_hide();
      }
      else if (key_pressed == sf::Keyboard::Key::Space)
      {
        if (m_selected == menu_view_item::start) exec_game();
        else if (m_selected == menu_view_item::options) exec_options();
        else if (m_selected == menu_view_item::quit)
        {
          m_window.close();
          return true;
        }
      }
      else if (key_pressed == sf::Keyboard::Key::O)
      {
        exec_options();
      }
      else if (key_pressed == sf::Keyboard::Key::S)
      {
        exec_game();
      }
      else if (key_pressed == sf::Keyboard::Key::Q)
      {
        m_window.close();
        return true;
      }
      else if (key_pressed == sf::Keyboard::Key::F3)
      {
        // debug
        std::clog << "Debug";
      }
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        const auto mouse_screen_pos{
          screen_coordinat(event.mouseButton.x, event.mouseButton.y)
        };
        if (is_in(mouse_screen_pos, m_layout.get_start())) exec_game();
        else if (is_in(mouse_screen_pos, m_layout.get_options())) exec_options();
        else if (is_in(mouse_screen_pos, m_layout.get_quit()))
        {
          m_window.close();
          return true;
        }
      }
    }
  }
  return false; // Do not close the window :-)
}

void menu_view::show()
{
  // Start drawing the new frame, by clearing the screen
  m_window.clear();

  show_panels(*this);

  show_title_panel(*this);
  show_subtitle_panel(*this);
  show_start_panel(*this);
  show_options_panel(*this);
  show_about_panel(*this);
  show_quit_panel(*this);
  show_selected_panel(*this);

  // Display all shapes
  m_window.display();

}

void show_about_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_about()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_strip(v.get_resources(), chess_color::white)
  );
  v.get_window().draw(rectangle);

  sf::Text text;
  text.setFont(get_font(v.get_resources()));
  text.setString("About");
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(64);
  text.setFillColor(sf::Color::Black);
  set_text_position(text, screen_rect);
  v.get_window().draw(text);
}

void show_options_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_options()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_strip(v.get_resources(), chess_color::black)
  );
  v.get_window().draw(rectangle);

  sf::Text text;
  text.setFont(get_font(v.get_resources()));
  text.setString("Options");
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(64);
  text.setFillColor(sf::Color::Black);
  set_text_position(text, screen_rect);
  v.get_window().draw(text);
}

void show_panels(menu_view& v)
{
  for (const auto& screen_rect: get_panels(v.get_layout()))
  {
    sf::RectangleShape rectangle;
    set_rect(rectangle, screen_rect);
    rectangle.setFillColor(sf::Color(32, 32, 32));
    v.get_window().draw(rectangle);
  }
}

void show_quit_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_quit()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_strip(v.get_resources(), chess_color::black)
  );
  v.get_window().draw(rectangle);

  sf::Text text;
  text.setFont(get_font(v.get_resources()));
  text.setString("Quit");
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(64);
  text.setFillColor(sf::Color::Black);
  set_text_position(text, screen_rect);
  v.get_window().draw(text);
}

void show_selected_panel(menu_view& v)
{

  const auto select_rect{v.get_layout().get_selectable_rect(v.get_selected())};
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

void show_start_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_start()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_strip(v.get_resources(), chess_color::white)
  );
  v.get_window().draw(rectangle);

  sf::Text text;
  text.setFont(get_font(v.get_resources()));
  text.setString("Start");
  text.setStyle(sf::Text::Bold);
  text.setCharacterSize(64);
  text.setFillColor(sf::Color::Black);
  set_text_position(text, screen_rect);
  v.get_window().draw(text);
}

void show_subtitle_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_subtitle()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_subtitle(v.get_resources())
  );
  v.get_window().draw(rectangle);
}

void show_title_panel(menu_view& v)
{
  const auto screen_rect{v.get_layout().get_title()};
  sf::RectangleShape rectangle;
  set_rect(rectangle, screen_rect);
  rectangle.setTexture(
    &get_title(v.get_resources())
  );
  v.get_window().draw(rectangle);
}

#endif // LOGIC_ONLY
