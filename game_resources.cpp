#include "game_resources.h"

#ifndef LOGIC_ONLY

#include <QFile>
#include <cassert>

game_resources::game_resources()
{
  // Load font file
  {
    const QString filename{"arial.ttf"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_font.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find font file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load music
  // Playing sound on Travis gives thousands of error lines, which causes the
  // build to fail
  {
    const QString filename{"ninja_gods.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_ninja_gods.openFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  {
    const QString filename{"faring_into_battle.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_faring_into_battle_buffer.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  {
    const QString filename{"its_time_to_rock.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_its_time_to_rock_buffer.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  {
    const QString filename{"jumping_into_battle.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_jumping_into_battle_buffer.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  {
    const QString filename{"lets_rule.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_lets_rule_buffer.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  {
    const QString filename{"moving_forward.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_moving_forward_buffer.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  {
    const QString filename{"to_rule_is_to_act.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_to_rule_is_to_act_buffer.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  m_faring_into_battle.setBuffer(m_faring_into_battle_buffer);
  m_its_time_to_rock.setBuffer(m_its_time_to_rock_buffer);
  m_jumping_into_battle.setBuffer(m_jumping_into_battle_buffer);
  m_lets_rule.setBuffer(m_lets_rule_buffer);
  m_moving_forward.setBuffer(m_moving_forward_buffer);
  m_to_rule_is_to_act.setBuffer(m_to_rule_is_to_act_buffer);
}

sf::Texture& get_black_square(game_resources& r) noexcept
{
  return r.get_square(chess_color::black);
}

sf::Texture& game_resources::get_piece(
  const chess_color color,
  const piece_type type
)
{
  return m_textures.get_piece(color, type);
}

sf::Texture& game_resources::get_piece_portrait(
  const chess_color color,
  const piece_type type
)
{
  return m_textures.get_piece_portrait(color, type);
}

sf::Texture& get_white_square(game_resources& r) noexcept
{
  return get_white_square(r.get_textures());
}

sf::Texture& game_resources::get_square(const chess_color color) noexcept
{
  return m_textures.get_square(color);
}

void game_resources::play_sound(
  const chess_color color,
  const piece_type type
)
{
  if (color == chess_color::black)
  {
    if (type == piece_type::bishop) m_faring_into_battle.play();
    if (type == piece_type::king) m_lets_rule.play();
    if (type == piece_type::knight) m_jumping_into_battle.play();
    if (type == piece_type::pawn) m_moving_forward.play();
    if (type == piece_type::queen) m_to_rule_is_to_act.play();
    if (type == piece_type::rook) m_its_time_to_rock.play();
  }
  else
  {
    if (type == piece_type::bishop) m_faring_into_battle.play();
    if (type == piece_type::king) m_lets_rule.play();
    if (type == piece_type::knight) m_jumping_into_battle.play();
    if (type == piece_type::pawn) m_moving_forward.play();
    if (type == piece_type::queen) m_to_rule_is_to_act.play();
    if (type == piece_type::rook) m_its_time_to_rock.play();
  }
}

void test_game_resources()
{
  #ifndef NDEBUG // no tests in release
  game_resources r;
  assert(r.get_square(chess_color::black).getSize().x > 0.0);
  #endif
}

#endif // LOGIC_ONLY
