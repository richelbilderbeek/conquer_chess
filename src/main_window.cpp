#include "main_window.h"

#ifndef LOGIC_ONLY

#include "about_view.h"
#include "controls_view.h"
#include "diagnostics_file.h"
#include "draw.h"
#include "game_options.h"
#include "game_resources.h"
#include "game_view.h"
#include "loading_view.h"
#include "lobby_options.h"
#include "lobby_view.h"
#include "menu_view.h"
#include "options_view.h"
#include "render_window.h"
#include "replay_view.h"
#include "screen_rect.h"

#include <cassert>
#include <cmath>

main_window::main_window(const cc_cli_options& options)
  : m_cli_options{options}
{
  game_resources::get().get_loading_screen_songs().get_heroes().setVolume(
    10
  );
  game_resources::get().get_loading_screen_songs().get_heroes().play();


  m_views[program_state::about] = std::make_unique<about_view>();
  m_views[program_state::game] = std::make_unique<game_view>();
  m_views[program_state::left_controls] = std::make_unique<controls_view>(side::lhs);
  m_views[program_state::loading] = std::make_unique<loading_view>();
  m_views[program_state::lobby] = std::make_unique<lobby_view>();
  m_views[program_state::main_menu] = std::make_unique<menu_view>();
  m_views[program_state::options] = std::make_unique<options_view>();
  m_views[program_state::replay] = std::make_unique<replay_view>();
  m_views[program_state::right_controls] = std::make_unique<controls_view>(side::rhs);

  m_replay = get_played_scholars_mate();

  // All programs states have a view
  for (const auto s: get_all_program_states())
  {
    // If this fails, add the view corresponding to the game to 'm_views'
    assert(m_views.find(s) != std::end(m_views));
  }
  // All programs states are inactive and have no suggested next start
  for (const auto s: get_all_program_states())
  {
    assert(!m_views[s]->is_active());
    assert(!m_views[s]->get_next_state().has_value());
  }
  // Start and stop
  for (const auto s: get_all_program_states())
  {
    assert(!m_views[s]->is_active());
    m_views[s]->start();
    assert(m_views[s]->is_active());
    m_views[s]->tick(delta_t(0.01));
    m_views[s]->draw();
    m_views[s]->stop();
    assert(!m_views[s]->is_active());
  }

  // Setup the game options
  m_game_options.reset(); // All default
  m_game_options.set_show_debug_info(
    m_cli_options.get_do_show_debug_info()
  );
}

void main_window::exec()
{
  m_views[program_state::loading]->start();

  while (get_render_window().isOpen())
  {
    // Process user input and play game until instructed to exit
    const bool must_quit{process_events()};
    if (must_quit) return;

    // Go to the next state
    tick();

    // Show the new state
    show();
  }
  get_render_window().close();
}

bool main_window::process_events()
{
  sf::Event event;
  while (get_render_window().pollEvent(event))
  {
    const bool stop_program{process_event(event)};
    if (stop_program == true) return true;
  }
  return false; // Do not close the window :-)
}

bool main_window::process_event(sf::Event& event)
{
  // General events
  if (event.type == sf::Event::Resized)
  {
    process_resize_event(event);
    return false; // Do not close the program
  }
  if (event.type == sf::Event::KeyPressed)
  {
    sf::Keyboard::Key key_pressed = event.key.code;
    if (key_pressed == sf::Keyboard::Key::F3)
    {
      m_game_options.set_show_debug_info(
        !m_game_options.get_show_debug_info()
      );
      return false; // Done. Do not close the program
    }
  }

  // Specific
  return m_views[m_program_state]->process_event(event);
}
void main_window::process_resize_event(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);

  // Add screen size to diagnostics file
  diagnostics_file().add_screen_size(event.size.width, event.size.height);

  // From https://www.sfml-dev.org/tutorials/2.2/graphics-view.php#showing-more-when-the-window-is-resized
  const sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
  get_render_window().setView(sf::View(visible_area));

  // Resize all windows
  for (const auto s: get_all_program_states())
  {
    m_views[s]->process_resize_event(event);
  }

}

void main_window::set_new_state(const program_state s)
{
  // Respond to the old state closing
  {
    // Get the replay of a played game
    if (m_program_state == program_state::game)
    {
      const auto p{dynamic_cast<game_view*>(m_views[program_state::game].get())};
      assert(p);
      m_replay = p->get_replay();
    }
    // Get the lobby options
    else if (m_program_state == program_state::lobby)
    {
      const auto p{dynamic_cast<lobby_view*>(m_views[program_state::lobby].get())};
      assert(p);
      if (p->has_accepted())
      {
        m_lobby_options = p->get_lobby_options();
      }
      else
      {

      }
    }
    // Get the game options
    else if (m_program_state == program_state::options)
    {
      const auto p{dynamic_cast<options_view*>(m_views[program_state::options].get())};
      assert(p);
      if (p->has_accepted())
      {
          m_game_options = p->get_game_options();
      }
    }
  }

  assert(m_program_state != s);

  assert(m_views[m_program_state]->is_active());
  assert(m_views[m_program_state]->get_next_state().has_value());
  m_views[m_program_state]->stop();
  assert(!m_views[m_program_state]->is_active());
  assert(!m_views[m_program_state]->get_next_state().has_value());

  // Respond to the new state starting
  {
    // Re-/set the game options
    if (s == program_state::game)
    {
      const auto p{dynamic_cast<game_view*>(m_views[program_state::game].get())};
      assert(p);
      p->set_game_options(m_game_options);
      p->set_lobby_options(m_lobby_options);
    }
    // Re-/set the game options
    else if (s == program_state::options)
    {
      const auto p{dynamic_cast<options_view*>(m_views[program_state::options].get())};
      assert(p);
      p->set_game_options(m_game_options);
    }
    // Set the replay of the last game
    else if (s == program_state::replay)
    {
      const auto p{dynamic_cast<replay_view*>(m_views[program_state::replay].get())};
      assert(p);
      p->set_replay(m_replay);
    }
  }

  // Start the new state
  m_program_state = s;

  assert(!m_views[m_program_state]->is_active());
  assert(!m_views[m_program_state]->get_next_state().has_value());
  m_views[m_program_state]->start();
  assert(m_views[m_program_state]->is_active());
  assert(!m_views[m_program_state]->get_next_state().has_value());
}

void main_window::show()
{
  // Start drawing the new frame, by clearing the screen
  get_render_window().clear();

  m_views[m_program_state]->draw();

  if (m_game_options.get_show_debug_info()) {
    show_debug_info();
  }

  // Display all shapes
  get_render_window().display();

  m_sleep_scheduler.tick();
}

void main_window::show_debug_info()
{
  const auto debug_rect = screen_rect(
    screen_coordinate(4, 4),
    screen_coordinate(400, 30)
  );
  const int fps{
    static_cast<int>(std::round(m_sleep_scheduler.get_fps()))
  };
  draw_rectangle(debug_rect, sf::Color(128, 128, 128, 128));
  draw_text(
    std::to_string(fps) + std::string(" FPS, ")
    + to_str(m_program_state) + ", "
    + to_str(m_lobby_options.get_race(side::lhs))
    + std::string(" vs ")
    + to_str(m_lobby_options.get_race(side::rhs)),
    debug_rect,
    16
  );
}

void test_main_window()
{
  #ifndef NDEBUG
  {
    cc_cli_options options;
    main_window w(options);
    assert(w.get_program_state() == program_state::loading);
  }

  #endif // NDEBUG
}

void main_window::tick()
{
  const double dt_raw{
    get_speed_multiplier(m_game_options.get_game_speed())
      / m_sleep_scheduler.get_fps()
  };
  // For the first frames, dt_raw may be too big
  const delta_t dt{std::min(dt_raw, 1.0)};

  m_views[m_program_state]->tick(dt);

  if (m_views[m_program_state]->get_next_state().has_value()) {
    set_new_state(m_views[m_program_state]->get_next_state().value());
  }
}

#endif // LOGIC_ONLY

