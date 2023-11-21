#include "sound_effects.h"

#include <functional>
#include <QFile>
#include "volume.h"

#ifndef LOGIC_ONLY

sound_effects::sound_effects()
{
  const std::vector<std::tuple<std::reference_wrapper<sf::Sound>, std::reference_wrapper<sf::SoundBuffer>, std::string>> v = get_table();

  for (const auto& p: v)
  {
    const QString filename{std::get<2>(p).c_str()};
    QFile f(":/resources/sound_effects/" + filename);
    f.copy(filename);
    if (!std::get<1>(p).get().loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find sound file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
    // Connect sound with its buffer
    std::get<0>(p).get().setBuffer(std::get<1>(p).get());

  }
}

std::vector<std::tuple<std::reference_wrapper<sf::Sound>, std::reference_wrapper<sf::SoundBuffer>, std::string>> sound_effects::get_table() noexcept
{
  const std::vector<std::tuple<std::reference_wrapper<sf::Sound>, std::reference_wrapper<sf::SoundBuffer>, std::string>> v = {
    std::make_tuple(std::ref(m_attacking_high), std::ref(m_attacking_high_buffer), "attacking_high.ogg"),
    std::make_tuple(std::ref(m_attacking_low), std::ref(m_attacking_low_buffer), "attacking_low.ogg"),
    std::make_tuple(std::ref(m_attacking_mid), std::ref(m_attacking_mid_buffer), "attacking_mid.ogg"),
    std::make_tuple(std::ref(m_castling_kingside), std::ref(m_castling_kingside_buffer), "castling_kingside.ogg"),
    std::make_tuple(std::ref(m_castling_queenside), std::ref(m_castling_queenside_buffer), "castling_queenside.ogg"),
    std::make_tuple(std::ref(m_countdown), std::ref(m_countdown_buffer), "countdown.ogg"),
    std::make_tuple(std::ref(m_done_high), std::ref(m_done_high_buffer), "done_high.ogg"),
    std::make_tuple(std::ref(m_done_low), std::ref(m_done_low_buffer), "done_low.ogg"),
    std::make_tuple(std::ref(m_done_mid), std::ref(m_done_mid_buffer), "done_mid.ogg"),
    std::make_tuple(std::ref(m_faring_into_battle), std::ref(m_faring_into_battle_buffer), "faring_into_battle.ogg"),
    std::make_tuple(std::ref(m_heu_high), std::ref(m_heu_high_buffer), "heu_high.ogg"),
    std::make_tuple(std::ref(m_heu_low), std::ref(m_heu_low_buffer), "heu_low.ogg"),
    std::make_tuple(std::ref(m_heu_mid), std::ref(m_heu_mid_buffer), "heu_mid.ogg"),
    std::make_tuple(std::ref(m_hide), std::ref(m_hide_buffer), "hide.ogg"),
    std::make_tuple(std::ref(m_hmm_high), std::ref(m_hmm_high_buffer), "hmm_high.ogg"),
    std::make_tuple(std::ref(m_hmm_low), std::ref(m_hmm_low_buffer), "hmm_low.ogg"),
    std::make_tuple(std::ref(m_hmm_mid), std::ref(m_hmm_mid_buffer), "hmm_mid.ogg"),
    std::make_tuple(std::ref(m_i_cannot_high), std::ref(m_i_cannot_high_buffer), "i_cannot_high.ogg"),
    std::make_tuple(std::ref(m_i_cannot_low), std::ref(m_i_cannot_low_buffer), "i_cannot_low.ogg"),
    std::make_tuple(std::ref(m_i_cannot_mid), std::ref(m_i_cannot_mid_buffer), "i_cannot_mid.ogg"),
    std::make_tuple(std::ref(m_i_cant_high), std::ref(m_i_cant_high_buffer), "i_cant_high.ogg"),
    std::make_tuple(std::ref(m_i_cant_low), std::ref(m_i_cant_low_buffer), "i_cant_low.ogg"),
    std::make_tuple(std::ref(m_i_cant_mid), std::ref(m_i_cant_mid_buffer), "i_cant_mid.ogg"),
    std::make_tuple(std::ref(m_its_time_to_rock), std::ref(m_its_time_to_rock_buffer), "its_time_to_rock.ogg"),
    std::make_tuple(std::ref(m_jumping_into_battle), std::ref(m_jumping_into_battle_buffer), "jumping_into_battle.ogg"),
    std::make_tuple(std::ref(m_lets_rule), std::ref(m_lets_rule_buffer), "lets_rule.ogg"),
    std::make_tuple(std::ref(m_moving_forward), std::ref(m_moving_forward_buffer), "moving_forward.ogg"),
    std::make_tuple(std::ref(m_no_high), std::ref(m_no_high_buffer), "no_high.ogg"),
    std::make_tuple(std::ref(m_no_low), std::ref(m_no_low_buffer), "no_low.ogg"),
    std::make_tuple(std::ref(m_no_mid), std::ref(m_no_mid_buffer), "no_mid.ogg"),
    std::make_tuple(std::ref(m_nope_high), std::ref(m_nope_high_buffer), "nope_high.ogg"),
    std::make_tuple(std::ref(m_nope_low), std::ref(m_nope_low_buffer), "nope_low.ogg"),
    std::make_tuple(std::ref(m_nope_mid), std::ref(m_nope_mid_buffer), "nope_mid.ogg"),
    std::make_tuple(std::ref(m_to_rule_is_to_act), std::ref(m_to_rule_is_to_act_buffer), "to_rule_is_to_act.ogg"),
    std::make_tuple(std::ref(m_yes_high), std::ref(m_yes_high_buffer), "yes_high.ogg"),
    std::make_tuple(std::ref(m_yes_low), std::ref(m_yes_low_buffer), "yes_low.ogg"),
    std::make_tuple(std::ref(m_yes_mid), std::ref(m_yes_mid_buffer), "yes_mid.ogg"),
  };
  return v;
}

void sound_effects::play(const message& effect)
{
  const auto piece_type{effect.get_piece_type()};
  switch (effect.get_message_type())
  {
    case message_type::cannot:
    {
      switch (piece_type)
      {
        case piece_type::bishop: m_i_cant_high.play(); break;
        case piece_type::king: m_i_cannot_mid.play(); break;
        case piece_type::knight: m_i_cant_mid.play(); break;
        case piece_type::pawn: m_nope_mid.play(); break;
        case piece_type::queen: m_i_cannot_high.play(); break;
        default:
        case piece_type::rook:
          assert(piece_type == piece_type::rook);
          m_nope_low.play();
          break;
      }
      break;
    }
    case message_type::done:
    {
      switch (piece_type)
      {
        case piece_type::bishop: m_done_high.play(); break;
        case piece_type::king: m_done_mid.play(); break;
        case piece_type::knight: m_done_mid.play(); break;
        case piece_type::pawn: m_done_mid.play(); break;
        case piece_type::queen: m_done_high.play(); break;
        default:
        case piece_type::rook:
          assert(piece_type == piece_type::rook);
          m_done_low.play();
          break;
      }
      break;
    }
    case message_type::unselect:
      m_hide.play();
      break;
    case message_type::select:
    {
      switch (piece_type)
      {
        case piece_type::bishop: m_hmm_high.play(); break;
        case piece_type::king: m_yes_mid.play(); break;
        case piece_type::knight: m_hmm_mid.play(); break;
        case piece_type::pawn: m_heu_mid.play(); break;
        case piece_type::queen: m_yes_high.play(); break;
        default:
        case piece_type::rook:
          assert(piece_type == piece_type::rook);
          m_heu_low.play();
          break;
      }
      break;
    }
    case message_type::start_move:
    {
      switch (piece_type)
      {
        case piece_type::bishop: m_faring_into_battle.play(); break;
        case piece_type::king: m_lets_rule.play(); break;
        case piece_type::knight: m_jumping_into_battle.play(); break;
        case piece_type::pawn: m_moving_forward.play(); break;
        case piece_type::queen: m_to_rule_is_to_act.play(); break;
        default:
        case piece_type::rook:
          assert(piece_type == piece_type::rook);
          m_its_time_to_rock.play();
          break;
      }
      break;
    }
    case message_type::start_castling_kingside:
    {
      m_castling_kingside.play();
      break;
    }
    case message_type::start_castling_queenside:
    {
      m_castling_queenside.play();
      break;
    }
    default:
    case message_type::start_attack:
    {
      assert(effect.get_message_type() == message_type::start_attack);
      switch (piece_type)
      {
        case piece_type::bishop: m_attacking_high.play(); break;
        case piece_type::king: m_attacking_mid.play(); break;
        case piece_type::knight: m_attacking_mid.play(); break;
        case piece_type::pawn: m_attacking_low.play(); break;
        case piece_type::queen: m_attacking_high.play(); break;
        default:
        case piece_type::rook:
          assert(piece_type == piece_type::rook);
          m_attacking_low.play();
          break;
      }
      break;
    }
  }
}

void sound_effects::play_countdown() noexcept
{
  m_countdown.play();
}

void sound_effects::play_hide() noexcept
{
  m_hide.play();
}

void sound_effects::set_master_volume(const volume& v)
{
  for (auto& row: get_table())
  {
    std::get<0>(row).get().setVolume(v.get_percentage());
  }
}

#endif // LOGIC_ONLY
