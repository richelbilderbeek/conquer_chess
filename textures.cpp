#include "textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>
#include <sstream>

textures::textures()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Texture>, std::string>> v = {
    std::make_pair(std::ref(m_all_races_1), "all_races_1.jpg"),
    std::make_pair(std::ref(m_all_races_2), "all_races_2.jpg"),
    std::make_pair(std::ref(m_all_races_3), "all_races_3.jpg"),
    std::make_pair(std::ref(m_all_races_4), "all_races_4.jpg"),
    std::make_pair(std::ref(m_keyboard), "keyboard.png"),
    std::make_pair(std::ref(m_mouse), "mouse.png"),
    std::make_pair(std::ref(m_subtitle), "subtitle.png"),
    std::make_pair(std::ref(m_title), "title.png")
  };
  for (const auto& p: v)
  {
    const QString filename{p.second.c_str()};
    QFile f(":/resources/textures/" + filename);
    f.copy(filename);
    if (!p.first.get().loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }

  for (const auto r: get_all_chess_colors())
  {
    const std::string filename_str{get_square_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/" + filename);
    f.copy(filename);
    if (!m_squares[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  for (const auto r: get_all_chess_colors())
  {
    const std::string filename_str{get_square_semitransparent_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/" + filename);
    f.copy(filename);
    if (!m_semitransparent_squares[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }

  for (const auto r: get_all_chess_colors())
  {
    const std::string filename_str{get_strip_filename(r)};
    const QString filename{filename_str.c_str()};
    QFile f(":/resources/textures/" + filename);
    f.copy(filename);
    if (!m_strips[r].loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }

  for (const auto square_color: get_all_chess_colors())
  {
    for (const auto occupant_color: get_all_chess_colors())
    {
      const std::string filename_str{
        get_occupied_square_filename(
          square_color,
          occupant_color
        )
      };
      const QString filename{filename_str.c_str()};
      QFile f(":/resources/textures/" + filename);
      f.copy(filename);
      if (!m_occupied_squares[square_color][occupant_color].loadFromFile(filename.toStdString()))
      {
        QString msg{"Cannot find image file '" + filename + "'"};
        throw std::runtime_error(msg.toStdString());
      }
    }
  }

  // m_semitransparent_occupied_squares
  for (const auto square_color: get_all_chess_colors())
  {
    for (const auto occupant_color: get_all_chess_colors())
    {
      const std::string filename_str{
        get_occupied_square_semitransparent_filename(
          square_color,
          occupant_color
        )
      };
      const QString filename{filename_str.c_str()};
      QFile f(":/resources/textures/" + filename);
      f.copy(filename);
      if (!m_semitransparent_occupied_squares[square_color][occupant_color].loadFromFile(filename.toStdString()))
      {
        QString msg{"Cannot find image file '" + filename + "'"};
        throw std::runtime_error(msg.toStdString());
      }
    }
  }

}

sf::Texture& textures::get_controller_type(const physical_controller_type t) noexcept
{
  if (t == physical_controller_type::keyboard)
  {
    return m_keyboard;
  }
  assert(t == physical_controller_type::mouse);
  return m_mouse;
}

sf::Texture& textures::get_occupied_square(
  const chess_color square_color,
  const chess_color occupant_color
) noexcept
{
  return get_occupied_square_semitransparent(square_color, occupant_color);
  //return m_occupied_squares[square_color][occupant_color];
}

sf::Texture& textures::get_occupied_square_semitransparent(
  const chess_color square_color,
  const chess_color occupant_color
) noexcept
{
  return m_semitransparent_occupied_squares[square_color][occupant_color];
}

std::string textures::get_occupied_square_filename(
    const chess_color square_color,
    const chess_color occupant_color
) const noexcept
{
  std::stringstream s;
  s << square_color << "_" << occupant_color << ".png";
  return s.str();
}

std::string textures::get_occupied_square_semitransparent_filename(
    const chess_color square_color,
    const chess_color occupant_color
) const noexcept
{
  std::stringstream s;
  s << square_color << "_" << occupant_color << "_semitransparent.png";
  return s.str();
}

sf::Texture& textures::get_semitransparent_square(
  const chess_color color
) noexcept
{
  return m_semitransparent_squares[color];
}

sf::Texture& textures::get_square(
  const chess_color color
) noexcept
{
  return this->get_semitransparent_square(color);
  //return m_squares[color];
}

std::string textures::get_square_filename(
  const chess_color color
) const noexcept
{
  std::stringstream s;
  s << color << "_square.png";
  return s.str();
}

std::string textures::get_square_semitransparent_filename(
  const chess_color color
) const noexcept
{
  std::stringstream s;
  s << color << "_square_semitransparent.png";
  return s.str();
}

std::string textures::get_strip_filename(
  const chess_color color
) const noexcept
{
  std::stringstream s;
  s << color << "_strip.png";
  return s.str();
}

sf::Texture& textures::get_strip(const chess_color color) noexcept
{
  return m_strips[color];
}
#endif // LOGIC_ONLY
