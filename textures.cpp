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
    std::make_pair(std::ref(m_dark_black_square), "d_black.png"),
    //std::make_pair(std::ref(m_dark_square), "dark_square.png"),
    //std::make_pair(std::ref(m_dark_strip), "dark_strip.png"),
    std::make_pair(std::ref(m_dark_white_square), "d_white.png"),
    std::make_pair(std::ref(m_keyboard), "keyboard.png"),
    std::make_pair(std::ref(m_light_black_square), "l_black.png"),
    //std::make_pair(std::ref(m_light_square), "light_square.png"),
    //std::make_pair(std::ref(m_light_strip), "light_strip.png"),
    std::make_pair(std::ref(m_light_white_square), "l_white.png"),
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

}

sf::Texture& get_black_square(textures& t) noexcept
{
  return t.get_square(chess_color::black);
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
  if (square_color == chess_color::white)
  {
    if (occupant_color == chess_color::white)
    {
      return m_light_white_square;
    }
    assert(occupant_color == chess_color::black);
    return m_light_black_square;
  }
  assert(square_color == chess_color::black);
  if (occupant_color == chess_color::white)
  {
    return m_dark_white_square;
  }
  assert(occupant_color == chess_color::black);
  return m_dark_black_square;
}

sf::Texture& get_white_square(textures& t) noexcept
{
  return t.get_square(chess_color::white);
}

/*
sf::Texture& textures::get_ready(const bool is_ready) noexcept
{
  if (is_ready) return m_ready_yes;
  return m_ready_no;
}
*/

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
  return m_squares[color];
  /*
  switch (color)
  {
    case chess_color::black: return m_dark_square;
    default:
    case chess_color::white:
      assert(color == chess_color::white);
      return m_light_square;
  }
  */
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
  /*
  switch (color)
  {
    case chess_color::black: return m_dark_strip;
    default:
    case chess_color::white:
      assert(color == chess_color::white);
      return m_light_strip;
  }
  */
}
#endif // LOGIC_ONLY
