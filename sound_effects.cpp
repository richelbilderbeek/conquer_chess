#include "sound_effects.h"

#include <functional>
#include <QFile>

#ifndef LOGIC_ONLY

sound_effects::sound_effects()
{
  const std::vector<std::tuple<std::reference_wrapper<sf::Sound>, std::reference_wrapper<sf::SoundBuffer>, std::string>> v = {
    std::make_tuple(std::ref(m_faring_into_battle), std::ref(m_faring_into_battle_buffer), "faring_into_battle.ogg"),
    std::make_tuple(std::ref(m_hide), std::ref(m_hide_buffer), "hide.ogg"),
    std::make_tuple(std::ref(m_its_time_to_rock), std::ref(m_its_time_to_rock_buffer), "its_time_to_rock.ogg"),
    std::make_tuple(std::ref(m_jumping_into_battle), std::ref(m_jumping_into_battle_buffer), "jumping_into_battle.ogg"),
    std::make_tuple(std::ref(m_lets_rule), std::ref(m_lets_rule_buffer), "lets_rule.ogg"),
    std::make_tuple(std::ref(m_moving_forward), std::ref(m_moving_forward_buffer), "moving_forward.ogg"),
    std::make_tuple(std::ref(m_to_rule_is_to_act), std::ref(m_to_rule_is_to_act_buffer), "to_rule_is_to_act.ogg")
  };
  for (const auto& p: v)
  {
    const QString filename{std::get<2>(p).c_str()};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!std::get<1>(p).get().loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
    std::get<0>(p).get().setBuffer(std::get<1>(p).get());
  }
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

void sound_effects::play_hide() noexcept
{
  m_hide.play();
}

#endif // LOGIC_ONLY
