#include "loading_screen_songs.h"

#include <QFile>
#include <QResource>

loading_screen_songs::loading_screen_songs()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Music>, std::string>> v = {
    std::make_pair(std::ref(m_heroes), "heroes.ogg")
  };
  for (const auto& p: v)
  {
    const QString filename{p.second.c_str()};
    QFile f(":/resources/songs/" + filename);
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
