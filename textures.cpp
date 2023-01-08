#include "textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>

textures::textures()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Texture>, std::string>> v = {
    std::make_pair(std::ref(m_action_attack), "icon_attack.png"),
    std::make_pair(std::ref(m_action_castle_kingside), "icon_castle_kingside.png"),
    std::make_pair(std::ref(m_action_castle_queenside), "icon_castle_queenside.png"),
    std::make_pair(std::ref(m_action_en_passant), "icon_attack.png"),
    std::make_pair(std::ref(m_action_move), "icon_move.png"),
    std::make_pair(std::ref(m_action_promote_to_bishop), "icon_promote_to_bishop.png"),
    std::make_pair(std::ref(m_action_promote_to_knight), "icon_promote_to_knight.png"),
    std::make_pair(std::ref(m_action_promote_to_queen), "icon_promote_to_queen.png"),
    std::make_pair(std::ref(m_action_promote_to_rook), "icon_promote_to_rook.png"),
    std::make_pair(std::ref(m_action_select), "icon_select.png"),
    std::make_pair(std::ref(m_action_unselect), "icon_unselect.png"),
    std::make_pair(std::ref(m_all_races_1), "all_races_1.jpeg"),
    std::make_pair(std::ref(m_all_races_2), "all_races_2.jpeg"),
    std::make_pair(std::ref(m_all_races_3), "all_races_3.jpeg"),
    std::make_pair(std::ref(m_all_races_4), "all_races_4.jpeg"),
    std::make_pair(std::ref(m_black_chess_color), "black_chess_color.png"),
    std::make_pair(std::ref(m_classic_head), "classic_head.png"),
//    std::make_pair(std::ref(m_classic_map), "classic_map.png"),
    std::make_pair(std::ref(m_controls), "controls.png"),
    std::make_pair(std::ref(m_dark_black_square), "d_black.png"),
    std::make_pair(std::ref(m_dark_square), "d.png"),
    std::make_pair(std::ref(m_dark_strip), "dark_strip.png"),
    std::make_pair(std::ref(m_dark_white_square), "d_white.png"),
    std::make_pair(std::ref(m_game_speed), "game_speed.png"),
    std::make_pair(std::ref(m_keyboard), "keyboard.png"),
    std::make_pair(std::ref(m_light_black_square), "l_black.png"),
    std::make_pair(std::ref(m_light_square), "l.png"),
    std::make_pair(std::ref(m_light_strip), "light_strip.png"),
    std::make_pair(std::ref(m_light_white_square), "l_white.png"),
    std::make_pair(std::ref(m_mouse), "mouse.png"),
    std::make_pair(std::ref(m_music_volume), "music_volume.png"),
    std::make_pair(std::ref(m_protoss_head), "protoss_head.jpeg"),
//    std::make_pair(std::ref(m_protoss_map), "protoss_map.png"),
    std::make_pair(std::ref(m_ready_no), "ready_no.png"),
    std::make_pair(std::ref(m_ready_yes), "ready_yes.png"),
    std::make_pair(std::ref(m_sound_effects_volume), "sound_effects_volume.png"),
    std::make_pair(std::ref(m_starting_position), "starting_position.jpeg"),
    std::make_pair(std::ref(m_subtitle), "subtitle.png"),
    std::make_pair(std::ref(m_terran_head), "terran_head.jpeg"),
//    std::make_pair(std::ref(m_terran_map), "terran_map.png"),
    std::make_pair(std::ref(m_title), "title.png"),
    std::make_pair(std::ref(m_white_chess_color), "white_chess_color.png"),
    std::make_pair(std::ref(m_zerg_head), "zerg_head.jpeg")
//    std::make_pair(std::ref(m_zerg_map), "zerg_map.png")
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

sf::Texture& textures::get_action_icon(const piece_action_type t) noexcept
{
  switch(t)
  {
    case piece_action_type::attack:
      return m_action_attack;
    case piece_action_type::castle_kingside:
      return m_action_castle_kingside;
    case piece_action_type::castle_queenside:
      return m_action_castle_queenside;
    case piece_action_type::en_passant:
      return m_action_en_passant;
    case piece_action_type::move:
      return m_action_move;
    case piece_action_type::promote_to_bishop:
      return m_action_promote_to_bishop;
    case piece_action_type::promote_to_knight:
      return m_action_promote_to_knight;
    case piece_action_type::promote_to_queen:
      return m_action_promote_to_queen;
    case piece_action_type::promote_to_rook:
      return m_action_promote_to_rook;
    case piece_action_type::select:
      return m_action_select;
    case piece_action_type::unselect:
    default:
      assert(t == piece_action_type::unselect);
      return m_action_unselect;
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

sf::Texture& textures::get_game_option_icon(const options_view_item i) noexcept
{
  switch (i)
  {
    case options_view_item::game_speed:
      return m_game_speed;
    case options_view_item::left_controls:
      return m_controls;
    case options_view_item::right_controls:
      return m_controls;
    case options_view_item::music_volume:
      return m_music_volume;
    case options_view_item::sound_effects_volume:
      return m_sound_effects_volume;
    case options_view_item::starting_position:
    default:
      assert(i == options_view_item::starting_position);
      return m_starting_position;
  }
}

sf::Texture& textures::get_head(const race r) noexcept
{
  switch (r)
  {
    case race::classic: return m_classic_head;
    case race::protoss: return m_protoss_head;
    case race::terran: return m_terran_head;
    default:
    case race::zerg:
      assert(r == race::zerg);
      return m_zerg_head;
  }
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
sf::Texture& textures::get_map(const race r) noexcept
{
  switch (r)
  {
    case race::classic: return m_classic_map;
    case race::protoss: return m_protoss_map;
    case race::terran: return m_terran_map;
    default:
    case race::zerg:
      assert(r == race::zerg);
      return m_zerg_map;
  }

}
*/

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
