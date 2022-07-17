#include "sound_effects.h"

#include <QFile>

sound_effects::sound_effects()
{
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
    const QString filename{"hide.ogg"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_hide_buffer.loadFromFile(filename.toStdString()))
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
  m_hide.setBuffer(m_hide_buffer);
  m_its_time_to_rock.setBuffer(m_its_time_to_rock_buffer);
  m_jumping_into_battle.setBuffer(m_jumping_into_battle_buffer);
  m_lets_rule.setBuffer(m_lets_rule_buffer);
  m_moving_forward.setBuffer(m_moving_forward_buffer);
  m_to_rule_is_to_act.setBuffer(m_to_rule_is_to_act_buffer);
}

void sound_effects::play(const sound_effect& effect)
{
  if (effect.get_sound_effect_type() == sound_effect_type::select)
  {
     m_hide.play();
  }
  else
  {
    const auto color{effect.get_color()};
    const auto piece_type{effect.get_piece_type()};
    if (color == chess_color::black)
    {
      if (piece_type == piece_type::bishop) m_faring_into_battle.play();
      if (piece_type == piece_type::king) m_lets_rule.play();
      if (piece_type == piece_type::knight) m_jumping_into_battle.play();
      if (piece_type == piece_type::pawn) m_moving_forward.play();
      if (piece_type == piece_type::queen) m_to_rule_is_to_act.play();
      if (piece_type == piece_type::rook) m_its_time_to_rock.play();
    }
    else
    {
      if (piece_type == piece_type::bishop) m_faring_into_battle.play();
      if (piece_type == piece_type::king) m_lets_rule.play();
      if (piece_type == piece_type::knight) m_jumping_into_battle.play();
      if (piece_type == piece_type::pawn) m_moving_forward.play();
      if (piece_type == piece_type::queen) m_to_rule_is_to_act.play();
      if (piece_type == piece_type::rook) m_its_time_to_rock.play();
    }
  }
}
