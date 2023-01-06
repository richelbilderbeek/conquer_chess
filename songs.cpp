#include "songs.h"

#include <QFile>
#include <QResource>

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
    const QString filename{p.second.c_str()};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!f.exists(filename))
    {
      QString msg{
        "Cannot write sound file '" + filename + "'.\n"
        "Tip 1: check the spelling of the filename\n"
        "Tip 2: do not load resources in static initialization time"
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
