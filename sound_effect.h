#ifndef SOUND_EFFECT_H
#define SOUND_EFFECT_H

#include "sound_effect_type.h"
#include "chess_color.h"
#include "piece_type.h"

/// Representation of a sound effect
/// Use 'sound_effects::play' to play it
class sound_effect
{
public:
  sound_effect(
    const sound_effect_type set,
    const chess_color c,
    const piece_type pc
  );
  auto get_sound_effect_type() const noexcept { return m_sound_effect_type; }
  auto get_color() const noexcept { return m_chess_color; }
  auto get_piece_type() const noexcept { return m_piece_type; }
  private:

  sound_effect_type m_sound_effect_type;
  chess_color m_chess_color;
  piece_type m_piece_type;
};

#endif // SOUND_EFFECT_H
