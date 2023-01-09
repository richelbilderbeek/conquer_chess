#include "piece_textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>
#include <sstream>

piece_textures::piece_textures()
{
  for (const auto r: get_all_races())
  {
    for (const auto c: get_all_chess_colors())
    {
      for (const auto p: get_all_piece_types())
      {
        const std::string filename_str{get_filename(r, c, p)};
        const QString filename{filename_str.c_str()};
        QFile f(":/resources/textures/pieces/" + filename);
        f.copy(filename);
        if (!m_textures[r][c][p].loadFromFile(filename.toStdString()))
        {
          QString msg{"Cannot find image file '" + filename + "'"};
          throw std::runtime_error(msg.toStdString());
        }
      }
    }
  }
}

std::string piece_textures::get_filename(
  const race r,
  const chess_color color,
  const piece_type type
) const noexcept
{
  std::stringstream s;
  s << r << "_" << color << "_" << type;
  if (r == race::classic)
  {
    s << "_with_contour.png";
  }
  else
  {
    s << ".jpg";
  }
  return s.str();
}

sf::Texture& piece_textures::get_piece(
  const race race,
  const chess_color color,
  const piece_type type
)
{
  return m_textures[race][color][type];
}

#endif // LOGIC_ONLY
