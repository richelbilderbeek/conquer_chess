#include "loading_screen_fonts.h"

#ifndef LOGIC_ONLY

#include <QFile>

loading_screen_fonts::loading_screen_fonts()
{
  // Load font file
  {
    const QString filename{"arial.ttf"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_arial_font.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}

#endif // LOGIC_ONLY
