#include "songs.h"

#include <filesystem>

songs::songs()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Music>, std::string>> v = {
    std::make_pair(std::ref(m_aura), "aura.ogg"),
    std::make_pair(std::ref(m_bliss), "bliss.ogg"),
    std::make_pair(std::ref(m_heroes), "heroes.ogg"),
    std::make_pair(std::ref(m_silence), "silence.ogg"),
    std::make_pair(std::ref(m_soothing), "soothing.ogg"),
    std::make_pair(std::ref(m_wonderful_time), "wonderful_time.ogg")
  };
  for (const auto& p: v)
  {
    const auto filename{
      std::string("resources/songs/")
      + p.second.c_str()
    };

    if (!std::filesystem::exists(filename))
    {
      auto msg{
        "Cannot write sound file '" + filename + "'.\n"
        "Tip 1: check the spelling of the filename\n"
        "Tip 2: do not load resources in static initialization time"
      };
      throw std::runtime_error(msg);
    }
    if (!p.first.get().openFromFile(filename))
    {
      auto msg{"Cannot find song file '" + filename + "'"};
      throw std::runtime_error(msg);
    }
    p.first.get().setLoop(true);
  }
}
