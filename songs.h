#ifndef SONGS_H
#define SONGS_H

#include <SFML/Audio/Music.hpp>

/// Holds all the music
class songs
{
public:
  songs();

  int get_n_songs() const noexcept { return sizeof(*this) / sizeof(m_ninja_gods); };

  sf::Music& get_wonderful_time() noexcept { return m_wonderful_time; }


private:
  sf::Music m_ninja_gods;
  /*
  sf::Music m_aura;
  sf::Music m_bliss;
  sf::Music m_heroes;
  sf::Music m_silence;
  sf::Music m_soothing;
  */
  sf::Music m_wonderful_time;

};

#endif // SONGS_H
