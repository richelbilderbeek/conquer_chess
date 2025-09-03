#include "loading_screen_fonts.h"

#include <stdexcept>

loading_screen_fonts::loading_screen_fonts()
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
}
