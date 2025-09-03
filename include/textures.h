#ifndef TEXTURES_H
#define TEXTURES_H

#ifndef LOGIC_ONLY

#include "chess_color.h"

#include <SFML/Graphics.hpp>

/// The games' textures.
class textures
{
public:
  textures();

  int get_n_textures() const noexcept { return sizeof(*this) / sizeof(m_title); };

  /// Get a strip of chess board squares
  sf::Texture& get_strip(const chess_color color) noexcept;

  /// Get a chess board square
  sf::Texture& get_square(const chess_color color) noexcept;

  /// Get the subtitle image
  sf::Texture& get_subtitle() noexcept { return m_subtitle; }

  /// Get the title image
  sf::Texture& get_title() noexcept { return m_title; }

  /// Get an occupied chess board square
  sf::Texture& get_occupied_square(
    const chess_color square_color,
    const chess_color occupant_color
  ) noexcept;

  /// Get an occupied chess board square
  sf::Texture& get_occupied_square_semitransparent(
    const chess_color square_color,
    const chess_color occupant_color
  ) noexcept;

  /// Get a semitransparent chess board square
  sf::Texture& get_semitransparent_square(const chess_color color) noexcept;

private:

  std::map<chess_color, std::map<chess_color, sf::Texture>> m_semitransparent_occupied_squares;
  std::map<chess_color, std::map<chess_color, sf::Texture>> m_occupied_squares;
  std::map<chess_color, sf::Texture> m_squares;
  std::map<chess_color, sf::Texture> m_semitransparent_squares;
  std::map<chess_color, sf::Texture> m_strips;

  sf::Texture m_subtitle;
  sf::Texture m_title;

  std::string get_occupied_square_filename(
    const chess_color square_color,
    const chess_color occupant_color
  ) const noexcept;

  std::string get_occupied_square_semitransparent_filename(
    const chess_color square_color,
    const chess_color occupant_color
  ) const noexcept;

  std::string get_square_filename(
    const chess_color color
  ) const noexcept;

  std::string get_square_semitransparent_filename(
    const chess_color color
  ) const noexcept;

  std::string get_strip_filename(
    const chess_color color
  ) const noexcept;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
