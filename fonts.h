#ifndef FONTS_H
#define FONTS_H

#include <SFML/Graphics/Font.hpp>

/// The fonts used by this game
class fonts
{
public:
  fonts();

  int get_n_fonts() const noexcept { return sizeof(*this) / sizeof(m_arial_font); };

  /// Get an Arial font
  sf::Font& get_arial_font() noexcept { return m_arial_font; }

  /// Get the Code Squared font
  sf::Font& get_code_squared_font() noexcept { return m_code_squared_font; }

  /// Get the Futuristic Fixed-Width font
  sf::Font& get_futuristic_font() noexcept { return m_futuristic_font; }

private:
  sf::Font m_arial_font;
  sf::Font m_code_squared_font;
  sf::Font m_futuristic_font;
};

#endif // FONTS_H
