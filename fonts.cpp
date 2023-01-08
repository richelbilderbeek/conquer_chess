#include "fonts.h"

#include <QFile>

fonts::fonts()
{
  // Load font file
  {
    const QString filename{"arial.ttf"};
    QFile f(":/resources/fonts/" + filename);
    f.copy(filename);
    if (!m_arial_font.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load font file
  {
    const QString filename{"CodeSquaredRegular-AYRg.ttf"};
    QFile f(":/resources/fonts/" + filename);
    f.copy(filename);
    if (!m_code_squared_font.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load font file
  {
    const QString filename{"16114_FuturistFixed-width.ttf"};
    QFile f(":/resources/fonts/" + filename);
    f.copy(filename);
    if (!m_futuristic_font.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}
