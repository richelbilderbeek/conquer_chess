#include "game_resources.h"

#include <QFile>
#include <cassert>

game_resources::game_resources()
{
  // Load the light square
  {
    const QString filename{"l.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_square.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load font file
  {
    const QString filename{"arial.ttf"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_font.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Playing sound on Travis gives thousands of error lines, which causes the
  // build to fail
  {
    const QString filename{"ninja_gods.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_ninja_gods.openFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}

void test_game_resources()
{
  #ifndef NDEBUG // no tests in release
  game_resources r;
  assert(r.get_white_square().getSize().x > 0.0);
  #endif
}
