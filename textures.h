#ifndef TEXTURES_H
#define TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "chess_color.h"
#include "piece_type.h"
#include "piece_action_type.h"
#include "physical_controller_type.h"
#include "race.h"
#include "options_view_item.h"

/// The games' textures
/// The raw game resources
class textures
{
public:
  textures();

  int get_n_textures() const noexcept { return sizeof(*this) / sizeof(m_all_races_1); };

  /// Get an occupied chess board square
  sf::Texture& get_occupied_square(
    const chess_color square_color,
    const chess_color occupant_color
  ) noexcept;

  sf::Texture& get_all_races_1() noexcept { return m_all_races_1; }
  sf::Texture& get_all_races_2() noexcept { return m_all_races_2; }
  sf::Texture& get_all_races_3() noexcept { return m_all_races_3; }
  sf::Texture& get_all_races_4() noexcept { return m_all_races_4; }

  /// Get an image for a controller type, i.e. mouse or keyboard
  sf::Texture& get_controller_type(const physical_controller_type t) noexcept;

  /// Get a chess board square
  sf::Texture& get_square(const chess_color color) noexcept;

  /// Get a strip of chess board squares
  sf::Texture& get_strip(const chess_color color) noexcept;

  /// Get the subtitle image
  sf::Texture& get_subtitle() noexcept { return m_subtitle; }

  /// Get the title image
  sf::Texture& get_title() noexcept { return m_title; }

private:

  sf::Texture m_all_races_1;
  sf::Texture m_all_races_2;
  sf::Texture m_all_races_3;
  sf::Texture m_all_races_4;


  sf::Texture m_mouse;
  sf::Texture m_keyboard;

  sf::Texture m_dark_black_square;
  sf::Texture m_dark_square;
  sf::Texture m_dark_white_square;
  sf::Texture m_dark_strip;

  sf::Texture m_light_black_square;
  sf::Texture m_light_square;
  sf::Texture m_light_white_square;
  sf::Texture m_light_strip;

  sf::Texture m_subtitle;
  sf::Texture m_title;
};

/// Get a picture of a black/dark square
sf::Texture& get_black_square(textures& r) noexcept;

/// Get a picture of a white/light square
sf::Texture& get_white_square(textures& r) noexcept;

#endif // LOGIC_ONLY

#endif // TEXTURES_H
