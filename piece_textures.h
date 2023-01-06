#ifndef PIECE_TEXTURES_H
#define PIECE_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "chess_color.h"
#include "piece_type.h"
#include "race.h"

/// The games' piece_textures
/// The raw game resources
class piece_textures
{
public:
  piece_textures();

  int get_n_textures() const noexcept { return sizeof(*this) / sizeof(m_black_bishop); };

  /// Get texture of a piece
  sf::Texture& get_piece(
    const chess_color color,
    const piece_type type
  );

private:

  sf::Texture m_black_bishop;
  sf::Texture m_black_bishop_with_contour;
  sf::Texture m_black_king;
  sf::Texture m_black_king_with_contour;
  sf::Texture m_black_knight;
  sf::Texture m_black_knight_with_contour;
  sf::Texture m_black_pawn;
  sf::Texture m_black_pawn_with_contour;
  sf::Texture m_black_queen;
  sf::Texture m_black_queen_with_contour;
  sf::Texture m_black_rook;
  sf::Texture m_black_rook_with_contour;
  sf::Texture m_white_bishop;
  sf::Texture m_white_bishop_with_contour;
  sf::Texture m_white_king;
  sf::Texture m_white_king_with_contour;
  sf::Texture m_white_knight;
  sf::Texture m_white_knight_with_contour;
  sf::Texture m_white_pawn;
  sf::Texture m_white_pawn_with_contour;
  sf::Texture m_white_queen;
  sf::Texture m_white_queen_with_contour;
  sf::Texture m_white_rook;
  sf::Texture m_white_rook_with_contour;
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
