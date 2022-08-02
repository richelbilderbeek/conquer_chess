#ifndef SFML_HELPER_H
#define SFML_HELPER_H

#ifndef LOGIC_ONLY

/// SFML helper functions

#include "chess_color.h"
#include "screen_rect.h"

#include <SFML/Graphics.hpp>

/// Convert a fraction of health (i.e. a value e [0.0, 1.0] to a color
sf::Color f_health_to_color(const double f);

/// Make 'rectangle' have the same size and position as the 'screen_rect'
void set_rect(sf::RectangleShape& rectangle, const screen_rect& screen_rect);

/// Make 'text' have the same size and position as the 'screen_rect'
/// Assumes the text already has a font
void set_text_position(sf::Text& text, const screen_rect& screen_rect);

/// Convert an SFML key to a string
std::string to_str(const sf::Keyboard::Key k);

sf::Color to_sfml_color(const chess_color color) noexcept;

#endif // LOGIC_ONLY

#endif // SFML_HELPER_H
