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

  std::string get_filename(
    const race race,
    const chess_color color,
    const piece_type type
  ) const noexcept;

  int get_n_textures() const noexcept { return sizeof(*this) / sizeof(m_textures); };

  /// Get texture of a piece
  sf::Texture& get_piece(
    const race race,
    const chess_color color,
    const piece_type type
  );

private:

  std::map<
    race,
    std::map<
      chess_color,
      std::map<
        piece_type,
        sf::Texture
      >
    >
  > m_textures;

  /*
  sf::Texture m_classic_black_bishop;
  sf::Texture m_classic_black_bishop_with_contour;
  sf::Texture m_classic_black_king;
  sf::Texture m_classic_black_king_with_contour;
  sf::Texture m_classic_black_knight;
  sf::Texture m_classic_black_knight_with_contour;
  sf::Texture m_classic_black_pawn;
  sf::Texture m_classic_black_pawn_with_contour;
  sf::Texture m_classic_black_queen;
  sf::Texture m_classic_black_queen_with_contour;
  sf::Texture m_classic_black_rook;
  sf::Texture m_classic_black_rook_with_contour;
  sf::Texture m_classic_white_bishop;
  sf::Texture m_classic_white_bishop_with_contour;
  sf::Texture m_classic_white_king;
  sf::Texture m_classic_white_king_with_contour;
  sf::Texture m_classic_white_knight;
  sf::Texture m_classic_white_knight_with_contour;
  sf::Texture m_classic_white_pawn;
  sf::Texture m_classic_white_pawn_with_contour;
  sf::Texture m_classic_white_queen;
  sf::Texture m_classic_white_queen_with_contour;
  sf::Texture m_classic_white_rook;
  sf::Texture m_classic_white_rook_with_contour;
  sf::Texture m_protoss_black_bishop;
  sf::Texture m_protoss_black_king;
  sf::Texture m_protoss_black_knight;
  sf::Texture m_protoss_black_pawn;
  sf::Texture m_protoss_black_queen;
  sf::Texture m_protoss_black_rook;
  sf::Texture m_protoss_white_bishop;
  sf::Texture m_protoss_white_king;
  sf::Texture m_protoss_white_knight;
  sf::Texture m_protoss_white_pawn;
  sf::Texture m_protoss_white_queen;
  sf::Texture m_protoss_white_rook;
  sf::Texture m_terran_black_bishop;
  sf::Texture m_terran_black_king;
  sf::Texture m_terran_black_knight;
  sf::Texture m_terran_black_pawn;
  sf::Texture m_terran_black_queen;
  sf::Texture m_terran_black_rook;
  sf::Texture m_terran_white_bishop;
  sf::Texture m_terran_white_king;
  sf::Texture m_terran_white_knight;
  sf::Texture m_terran_white_pawn;
  sf::Texture m_terran_white_queen;
  sf::Texture m_terran_white_rook;
  sf::Texture m_zerg_black_bishop;
  sf::Texture m_zerg_black_king;
  sf::Texture m_zerg_black_knight;
  sf::Texture m_zerg_black_pawn;
  sf::Texture m_zerg_black_queen;
  sf::Texture m_zerg_black_rook;
  sf::Texture m_zerg_white_bishop;
  sf::Texture m_zerg_white_king;
  sf::Texture m_zerg_white_knight;
  sf::Texture m_zerg_white_pawn;
  sf::Texture m_zerg_white_queen;
  sf::Texture m_zerg_white_rook;
  */
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
