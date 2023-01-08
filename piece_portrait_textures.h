#ifndef PIECE_PORTRAIT_TEXTURES_H
#define PIECE_PORTRAIT_TEXTURES_H

#ifndef LOGIC_ONLY

#include <SFML/Graphics.hpp>
#include "chess_color.h"
#include "piece_type.h"
#include "race.h"

/// The games' piece_portrait_textures
/// The raw game resources
class piece_portrait_textures
{
public:
  piece_portrait_textures();

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
  sf::Texture& get_portrait(
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
