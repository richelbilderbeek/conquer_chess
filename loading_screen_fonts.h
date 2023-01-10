#ifndef LOADING_SCREEN_FONTS_H
#define LOADING_SCREEN_FONTS_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics/Font.hpp>

/// The loading_screen_fonts used in the loading screen
class loading_screen_fonts
{
public:
  loading_screen_fonts();

  int get_n_fonts() const noexcept { return sizeof(*this) / sizeof(m_arial_font); };

  /// Get an Arial font
  sf::Font& get_arial_font() noexcept { return m_arial_font; }

private:
  sf::Font m_arial_font;
};

#endif // LOGIC_ONLY

#endif // LOADING_SCREEN_FONTS_H
