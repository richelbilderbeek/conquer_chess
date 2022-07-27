#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H

#ifndef LOGIC_ONLY

#include <SFML/Audio.hpp>
#include "chess_color.h"
#include "piece_type.h"
#include "message.h"

/// Raw sound effects
class sound_effects
{
public:
  sound_effects();

  /// Play a sound effect
  void play(const message& effect);

  /// Play the 'wooosh' effect
  void play_hide() noexcept;

private:
  sf::Sound m_faring_into_battle;
  sf::Sound m_hide;
  sf::Sound m_its_time_to_rock;
  sf::Sound m_jumping_into_battle;
  sf::Sound m_lets_rule;
  sf::Sound m_moving_forward;
  sf::Sound m_to_rule_is_to_act;

  sf::Sound m_heu_high;
  sf::Sound m_heu_low;
  sf::Sound m_heu_mid;
  sf::Sound m_hmm_high;
  sf::Sound m_hmm_low;
  sf::Sound m_hmm_mid;
  sf::Sound m_i_cannot_high;
  sf::Sound m_i_cannot_low;
  sf::Sound m_i_cannot_mid;
  sf::Sound m_i_cant_high;
  sf::Sound m_i_cant_low;
  sf::Sound m_i_cant_mid;
  sf::Sound m_no_high;
  sf::Sound m_no_low;
  sf::Sound m_no_mid;
  sf::Sound m_nope_high;
  sf::Sound m_nope_low;
  sf::Sound m_nope_mid;
  sf::Sound m_yes_high;
  sf::Sound m_yes_low;
  sf::Sound m_yes_mid;

  sf::SoundBuffer m_faring_into_battle_buffer;
  sf::SoundBuffer m_heu_high_buffer;
  sf::SoundBuffer m_heu_low_buffer;
  sf::SoundBuffer m_heu_mid_buffer;
  sf::SoundBuffer m_hide_buffer;
  sf::SoundBuffer m_hmm_high_buffer;
  sf::SoundBuffer m_hmm_low_buffer;
  sf::SoundBuffer m_hmm_mid_buffer;
  sf::SoundBuffer m_i_cannot_high_buffer;
  sf::SoundBuffer m_i_cannot_low_buffer;
  sf::SoundBuffer m_i_cannot_mid_buffer;
  sf::SoundBuffer m_i_cant_high_buffer;
  sf::SoundBuffer m_i_cant_low_buffer;
  sf::SoundBuffer m_i_cant_mid_buffer;
  sf::SoundBuffer m_its_time_to_rock_buffer;
  sf::SoundBuffer m_jumping_into_battle_buffer;
  sf::SoundBuffer m_lets_rule_buffer;
  sf::SoundBuffer m_moving_forward_buffer;
  sf::SoundBuffer m_no_high_buffer;
  sf::SoundBuffer m_no_low_buffer;
  sf::SoundBuffer m_no_mid_buffer;
  sf::SoundBuffer m_nope_high_buffer;
  sf::SoundBuffer m_nope_low_buffer;
  sf::SoundBuffer m_nope_mid_buffer;
  sf::SoundBuffer m_to_rule_is_to_act_buffer;
  sf::SoundBuffer m_yes_high_buffer;
  sf::SoundBuffer m_yes_low_buffer;
  sf::SoundBuffer m_yes_mid_buffer;
};

#endif // LOGIC_ONLY

#endif // SOUND_EFFECTS_H
