#ifndef LOADING_SCREEN_SONGS_H
#define LOADING_SCREEN_SONGS_H

#include <SFML/Audio/Music.hpp>

/// All the songs in the loading screen
class loading_screen_songs
{
public:
  loading_screen_songs();

  int get_n_songs() const noexcept { return sizeof(*this) / sizeof(m_heroes); };

  sf::Music& get_heroes() noexcept { return m_heroes; }

private:
  sf::Music m_heroes;
};

#endif // LOADING_SCREEN_SONGS_H
