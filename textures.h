#ifndef TEXTURES_H
#define TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "chess_color.h"
#include "piece_type.h"

/// The games' textures
/// The raw game resources
class textures
{
public:
  textures();

  /// Get texture of a piece
  sf::Texture& get_piece(
    const chess_color color,
    const piece_type type
  );

  /// Get texture of a portrait of a piece
  sf::Texture& get_piece_portrait(
    const chess_color color,
    const piece_type type
  );

  /// Get a chess board square
  sf::Texture& get_square(const chess_color color) noexcept;

  /// Get a strip of chess board squares
  sf::Texture& get_strip(const chess_color color) noexcept;

  /// Get the subtitle image
  sf::Texture& get_subtitle() noexcept { return m_subtitle; }

  /// Get the title image
  sf::Texture& get_title() noexcept { return m_title; }

private:

  sf::Texture m_black_bishop;
  sf::Texture m_black_bishop_portrait;
  sf::Texture m_black_king;
  sf::Texture m_black_king_portrait;
  sf::Texture m_black_knight;
  sf::Texture m_black_knight_portrait;
  sf::Texture m_black_pawn;
  sf::Texture m_black_pawn_portrait;
  sf::Texture m_black_queen;
  sf::Texture m_black_queen_portrait;
  sf::Texture m_black_rook;
  sf::Texture m_black_rook_portrait;
  sf::Texture m_black_square;
  sf::Texture m_dark_strip;
  sf::Texture m_light_strip;
  sf::Texture m_subtitle;
  sf::Texture m_title;
  sf::Texture m_white_bishop;
  sf::Texture m_white_bishop_portrait;
  sf::Texture m_white_king;
  sf::Texture m_white_king_portrait;
  sf::Texture m_white_knight;
  sf::Texture m_white_knight_portrait;
  sf::Texture m_white_pawn;
  sf::Texture m_white_pawn_portrait;
  sf::Texture m_white_queen;
  sf::Texture m_white_queen_portrait;
  sf::Texture m_white_rook;
  sf::Texture m_white_rook_portrait;
  sf::Texture m_white_square;
};

/// Get a picture of a black/dark square
sf::Texture& get_black_square(textures& r) noexcept;

/// Get a picture of a white/light square
sf::Texture& get_white_square(textures& r) noexcept;

#endif // LOGIC_ONLY

#endif // TEXTURES_H
