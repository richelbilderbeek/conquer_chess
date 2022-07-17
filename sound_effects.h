#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H

#ifndef LOGIC_ONLY

#include <SFML/Audio.hpp>
#include "chess_color.h"
#include "piece_type.h"
#include "sound_effect.h"

/// Raw sound effects
class sound_effects
{
public:
  sound_effects();

  /// Play a sound effect
  void play(const sound_effect& effect);

private:
  sf::Sound m_faring_into_battle;
  sf::Sound m_its_time_to_rock;
  sf::Sound m_jumping_into_battle;
  sf::Sound m_lets_rule;
  sf::Sound m_moving_forward;
  sf::Sound m_to_rule_is_to_act;

  sf::SoundBuffer m_faring_into_battle_buffer;
  sf::SoundBuffer m_its_time_to_rock_buffer;
  sf::SoundBuffer m_jumping_into_battle_buffer;
  sf::SoundBuffer m_lets_rule_buffer;
  sf::SoundBuffer m_moving_forward_buffer;
  sf::SoundBuffer m_to_rule_is_to_act_buffer;
};

#endif // LOGIC_ONLY

#endif // SOUND_EFFECTS_H
