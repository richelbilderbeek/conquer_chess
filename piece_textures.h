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

  int get_n_textures() const noexcept
  {
    return sizeof(*this)
      / sizeof(m_textures.at(race::classic).at(chess_color::white).at(piece_type::king))
    ;
  }

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
};

#endif // LOGIC_ONLY

#endif // TEXTURES_H
