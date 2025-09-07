#ifndef SFML_HELPER_H
#define SFML_HELPER_H

/// @file SFML helper functions

#include "ccfwd.h"
#include "chess_color.h"
#include "screen_rect.h"
#include "piece_action_type.h"

#include <SFML/Graphics.hpp>

#include <string>

/// Convert a fraction of health (i.e. a value e [0.0, 1.0] to a color
sf::Color f_health_to_color(const double f);

/// Convert a fraction of shield (i.e. a value e [0.0, 1.0] to a color
sf::Color f_shield_to_color(const double f);

/// Get all the SFML mouse buttons
std::vector<sf::Mouse::Button> get_all_sfml_buttons() noexcept;

/// Get all the SFML keys
std::vector<sf::Keyboard::Key> get_all_sfml_keys() noexcept;

/// Get the SFML version this code has been compiled with
std::string get_sfml_version() noexcept;

/// Determine if the left or right shift has been pressed
bool is_shift_pressed() noexcept;

/// Convert an SFML key that has already been converted
/// to a filename to a filename, as used by \link{input_prompt_textures}
std::string key_str_to_resource_name(std::string key_str);

/// Make 'rectangle' have the same size as the \link{screen_coordinat}
void set_rect(sf::RectangleShape& rectangle, const screen_coordinate& screen_size);

/// Make 'rectangle' have the same size and position as the 'screen_rect'
void set_rect(sf::RectangleShape& rectangle, const screen_rect& screen_rect);

/// Make 'text' have the same size and position as the 'screen_rect'
/// Assumes the text already has a font and has text
void set_text_position(sf::Text& text, const screen_rect& screen_rect);

/// Tes these function
void test_sfml_helper();

/// Convert an SFML key to a filename, as used by \link{input_prompt_textures}
std::string to_inverted_resource_name(const sf::Keyboard::Key k);

/// Convert an SFML key to a filename, as used by \link{input_prompt_textures}
std::string to_resource_name(const sf::Keyboard::Key k);

/// Convert an SFML mouse button to a filename, as used by \link{input_prompt_textures}
std::string to_resource_name(const sf::Mouse::Button k);

/// Convert an SFML key to a string
std::string to_str(const sf::Keyboard::Key k);

/// Convert an SFML mouse button to a string
std::string to_str(const sf::Mouse::Button k);

/// Convert a key to its one-character description
std::string to_one_char_str(const sf::Keyboard::Key);

/// Convert chess_color to sf::Color
sf::Color to_sfml_color(const chess_color color) noexcept;

/// Convert chess_color to sf::Color
sf::Color to_sfml_color(
  const chess_color color,
  const piece_action_type t
) noexcept;

#endif // SFML_HELPER_H
