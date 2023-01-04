#ifndef SONGS_H
#define SONGS_H

#include <SFML/Audio/Music.hpp>

/// Holds all the music
class songs
{
public:
  songs();

  sf::Music& get_ninja_gods() noexcept { return m_ninja_gods; }

private:
  sf::Music m_ninja_gods;

};

#endif // SONGS_H
