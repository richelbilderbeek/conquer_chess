#include "fonts.h"

#include <stdexcept>

fonts::fonts()
{
  // Load font file
  {
    const std::string filename{"resources/fonts/arial.ttf"};
    if (!m_arial_font.loadFromFile(filename))
    {
      auto msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
  // Load font file
  {
    const std::string filename{"resources/fonts/CodeSquaredRegular-AYRg.ttf"};
    if (!m_code_squared_font.loadFromFile(filename))
    {
      auto msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
  // Load font file
  {
    const std::string filename{"resources/fonts/16114_FuturistFixed-width.ttf"};
    if (!m_futuristic_font.loadFromFile(filename))
    {
      auto msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
  }
}
