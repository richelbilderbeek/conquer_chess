#include "fps_clock.h"

#include <cassert>

fps_clock::fps_clock()
  : m_fps{0}
{

}

void test_fps_clock()
{
#ifndef NDEBUG
  {
    fps_clock c;
    c.tick();
    assert(c.get_fps() >= 0.0);
    sf::sleep(sf::milliseconds(1));
    c.tick();
    assert(c.get_fps() > 0.0);
  }
#endif // NDEBUG
}

void fps_clock::tick() noexcept
{
  const double dt{m_clock.getElapsedTime().asSeconds()};
  if (dt == 0.0) m_fps = 0;
  else m_fps = 1.0 / dt;
  m_clock.restart();
}

