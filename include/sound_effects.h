#ifndef SOUND_EFFECTS_H
#define SOUND_EFFECTS_H

#ifndef LOGIC_ONLY


#include "ccfwd.h"
#include "message.h"

#include <SFML/Audio.hpp>

/// The sound effects
class sound_effects
{
public:
  sound_effects();

  int get_n_sound_effects() const noexcept {
    return sizeof(*this)
    / (sizeof(m_attacking_high) + sizeof(m_attacking_high_buffer));
  }

  /// Play a sound effect
  void play(const message& effect);

  /// Play a Thema bark. Number can be either 1 or 2
  void play_bark(const int number);

  /// Play the robotic countdown effect.
  void play_countdown() noexcept;

  /// Play the 'wooosh' effect.
  void play_hide() noexcept;

  /// Set the master volume for all sounds
  void set_master_volume(const volume& v);

private:
  // The buffers must be declared before the sounds,
  // as an SFML 3 sf::Sound is constructed with its buffer
  sf::SoundBuffer m_attacking_high_buffer;
  sf::SoundBuffer m_attacking_low_buffer;
  sf::SoundBuffer m_attacking_mid_buffer;
  sf::SoundBuffer m_bark_1_buffer;
  sf::SoundBuffer m_bark_2_buffer;
  sf::SoundBuffer m_countdown_buffer;
  sf::SoundBuffer m_done_high_buffer;
  sf::SoundBuffer m_done_low_buffer;
  sf::SoundBuffer m_done_mid_buffer;
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

  sf::Sound m_attacking_high{m_attacking_high_buffer};
  sf::Sound m_attacking_low{m_attacking_low_buffer};
  sf::Sound m_attacking_mid{m_attacking_mid_buffer};
  sf::Sound m_bark_1{m_bark_1_buffer};
  sf::Sound m_bark_2{m_bark_2_buffer};
  sf::Sound m_countdown{m_countdown_buffer};
  sf::Sound m_done_high{m_done_high_buffer};
  sf::Sound m_done_low{m_done_low_buffer};
  sf::Sound m_done_mid{m_done_mid_buffer};
  sf::Sound m_faring_into_battle{m_faring_into_battle_buffer};
  sf::Sound m_heu_high{m_heu_high_buffer};
  sf::Sound m_heu_low{m_heu_low_buffer};
  sf::Sound m_heu_mid{m_heu_mid_buffer};
  sf::Sound m_hide{m_hide_buffer};
  sf::Sound m_hmm_high{m_hmm_high_buffer};
  sf::Sound m_hmm_low{m_hmm_low_buffer};
  sf::Sound m_hmm_mid{m_hmm_mid_buffer};
  sf::Sound m_i_cannot_high{m_i_cannot_high_buffer};
  sf::Sound m_i_cannot_low{m_i_cannot_low_buffer};
  sf::Sound m_i_cannot_mid{m_i_cannot_mid_buffer};
  sf::Sound m_i_cant_high{m_i_cant_high_buffer};
  sf::Sound m_i_cant_low{m_i_cant_low_buffer};
  sf::Sound m_i_cant_mid{m_i_cant_mid_buffer};
  sf::Sound m_its_time_to_rock{m_its_time_to_rock_buffer};
  sf::Sound m_jumping_into_battle{m_jumping_into_battle_buffer};
  sf::Sound m_lets_rule{m_lets_rule_buffer};
  sf::Sound m_moving_forward{m_moving_forward_buffer};
  sf::Sound m_no_high{m_no_high_buffer};
  sf::Sound m_no_low{m_no_low_buffer};
  sf::Sound m_no_mid{m_no_mid_buffer};
  sf::Sound m_nope_high{m_nope_high_buffer};
  sf::Sound m_nope_low{m_nope_low_buffer};
  sf::Sound m_nope_mid{m_nope_mid_buffer};
  sf::Sound m_to_rule_is_to_act{m_to_rule_is_to_act_buffer};
  sf::Sound m_yes_high{m_yes_high_buffer};
  sf::Sound m_yes_low{m_yes_low_buffer};
  sf::Sound m_yes_mid{m_yes_mid_buffer};

  /// Get the table that connects the sound and buffer and filename
  std::vector<std::tuple<std::reference_wrapper<sf::Sound>, std::reference_wrapper<sf::SoundBuffer>, std::string>> get_table() noexcept;
};

/// Test this class and its free functions
void test_sound_effects();

#endif // LOGIC_ONLY

#endif // SOUND_EFFECTS_H
