#include "textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>

textures::textures()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Texture>, std::string>> v = {
    std::make_pair(std::ref(m_all_races_1), "all_races_1.jpg"),
    std::make_pair(std::ref(m_all_races_2), "all_races_2.jpg"),
    std::make_pair(std::ref(m_all_races_3), "all_races_3.jpg"),
    std::make_pair(std::ref(m_all_races_4), "all_races_4.jpg"),
    std::make_pair(std::ref(m_black_chess_color), "black_chess_color.png"),
    std::make_pair(std::ref(m_dark_black_square), "d_black.png"),
    std::make_pair(std::ref(m_dark_square), "d.png"),
    std::make_pair(std::ref(m_dark_strip), "dark_strip.png"),
    std::make_pair(std::ref(m_dark_white_square), "d_white.png"),
    std::make_pair(std::ref(m_keyboard), "keyboard.png"),
    std::make_pair(std::ref(m_light_black_square), "l_black.png"),
    std::make_pair(std::ref(m_light_square), "l.png"),
    std::make_pair(std::ref(m_light_strip), "light_strip.png"),
    std::make_pair(std::ref(m_light_white_square), "l_white.png"),
    std::make_pair(std::ref(m_mouse), "mouse.png"),
    std::make_pair(std::ref(m_ready_no), "ready_no.png"),
    std::make_pair(std::ref(m_ready_yes), "ready_yes.png"),
    std::make_pair(std::ref(m_subtitle), "subtitle.png"),
    std::make_pair(std::ref(m_title), "title.png"),
    std::make_pair(std::ref(m_white_chess_color), "white_chess_color.png")
  };
  for (const auto& p: v)
  {
    const QString filename{p.second.c_str()};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!p.first.get().loadFromFile(filename.toStdString()))
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

sf::Texture& textures::get_color(const chess_color color) noexcept
{
  switch (color)
  {
    case chess_color::black: return m_black_chess_color;
    default:
    case chess_color::white:
      assert(color == chess_color::white);
      return m_white_chess_color;
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

sf::Texture& textures::get_ready(const bool is_ready) noexcept
{
  if (is_ready) return m_ready_yes;
  return m_ready_no;
}

sf::Texture& textures::get_square(const chess_color color) noexcept
{
  switch (color)
  {
    case chess_color::black: return m_dark_square;
    default:
    case chess_color::white:
      assert(color == chess_color::white);
      return m_light_square;
  }
}

sf::Texture& textures::get_strip(const chess_color color) noexcept
{
  switch (color)
  {
    case chess_color::black: return m_dark_strip;
    default:
    case chess_color::white:
      assert(color == chess_color::white);
      return m_light_strip;
  }
}
#endif // LOGIC_ONLY
