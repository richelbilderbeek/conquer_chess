#include "songs.h"

#include <QFile>
#include <QResource>

songs::songs()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Music>, std::string>> v = {
    std::make_pair(std::ref(m_ninja_gods), "ninja_gods.ogg"),
    std::make_pair(std::ref(m_wonderful_time), "wonderful_time.ogg")
    /*,
    std::make_pair(std::ref(m_aura), "aura.mp3"),
    std::make_pair(std::ref(m_bliss), "bliss.mp3"),
    std::make_pair(std::ref(m_heroes), "heroes.mp3"),
    std::make_pair(std::ref(m_silence), "silence.mp3"),
    std::make_pair(std::ref(m_soothing), "soothing.mp3"),
    std::make_pair(std::ref(m_wonderful_time), "wonderful_time.mp3")
    */
  };
  for (const auto& p: v)
  {
    const QString filename{p.second.c_str()};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!f.exists(filename))
    {
      QString msg{
        "Cannot write sound file '" + filename + "'.\n"
        "Tip: you cannot use resources in static initialization time"
      };
      throw std::runtime_error(msg.toStdString());
    }
    if (!p.first.get().openFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find song file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
}
