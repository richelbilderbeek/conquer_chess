#include "view.h"

#ifndef LOGIC_ONLY

#include "helper.h"

#include <cassert>
#include <iostream>

view::view()
{

}

view::~view()
{


}

void view::clear_next_state()
{
  // A window can be closed programmatically too,
  // hence do not do:
  //
  // assert(m_next_state.has_value());

  m_next_state.reset();
  assert(!m_next_state.has_value());
}

void view::draw()
{
  draw_impl();
}

bool view::process_event(sf::Event& e)
{
  return process_event_impl(e);
}

void view::process_resize_event(sf::Event& event)
{
  assert(event.type == sf::Event::Resized);
  process_resize_event_impl(event);
}

void view::set_is_active(const bool is_active)
{
  assert(m_is_active != is_active);
  m_is_active = is_active;
}

void view::set_next_state(const program_state next_state)
{
  #ifndef NDEBUG
  if (m_next_state.has_value())
  {
    std::clog
      << "Class " << typeid(*this).name() << " attempts to overwrite its next state. "
      << "Current next_state: " << to_str(m_next_state.value()) << '\n'
      << "Next next_state: " << to_str(next_state) << '\n'
    ;
  }
  #endif // NDEBUG
  assert(!m_next_state.has_value());
  m_next_state = next_state;
}

void view::start()
{
  assert(!m_is_active);
  start_impl();
}

void view::stop()
{
  assert(m_is_active);
  stop_impl();
}

void view::tick(const delta_t dt)
{
  assert(m_is_active);
  tick_impl(dt);
}

#endif // LOGIC_ONLY
