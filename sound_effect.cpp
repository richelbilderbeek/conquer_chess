#include "sound_effect.h"

sound_effect::sound_effect(
  const sound_effect_type set,
  const chess_color c,
  const piece_type pc
) : m_sound_effect_type{set},
    m_chess_color{c},
    m_piece_type{pc}
{

}
