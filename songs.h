#ifndef SONGS_H
#define SONGS_H

#ifndef LOGIC_ONLY

#include <SFML/Audio/Music.hpp>

/// Holds all the music
class songs
{
public:
  songs();

  int get_n_songs() const noexcept { return sizeof(*this) / sizeof(m_bliss); };

  sf::Music& get_aura() noexcept { return m_aura; }
  sf::Music& get_bliss() noexcept { return m_bliss; }
  sf::Music& get_heroes() noexcept { return m_heroes; }
  sf::Music& get_silence() noexcept { return m_silence; }
  sf::Music& get_soothing() noexcept { return m_soothing; }
  sf::Music& get_wonderful_time() noexcept { return m_wonderful_time; }

private:
  sf::Music m_aura;
  sf::Music m_bliss;
  sf::Music m_heroes;
  sf::Music m_silence;
  sf::Music m_soothing;
  sf::Music m_wonderful_time;
};

#endif // LOGIC_ONLY

#endif // SONGS_H
