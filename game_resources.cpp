#include "game_resources.h"

#ifndef LOGIC_ONLY

#include <QFile>
#include <cassert>

game_resources::game_resources()
{
  // Load the black/dark bishop
  {
    const QString filename{"bb.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_bishop.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark bishop portrait
  {
    const QString filename{"bb_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_bishop_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark king
  {
    const QString filename{"kb.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_king.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark king's portrait
  {
    const QString filename{"kb_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_king_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark knight
  {
    const QString filename{"nb.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_knight.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark knight portrait
  {
    const QString filename{"nb_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_knight_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark pawn
  {
    const QString filename{"pb.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_pawn.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark pawn portrait
  {
    const QString filename{"pb_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_pawn_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark queen
  {
    const QString filename{"qb.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_queen.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark queen's portrait
  {
    const QString filename{"qb_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_queen_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark rook
  {
    const QString filename{"rb.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_rook.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark rook portrait
  {
    const QString filename{"rb_portrait_2.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_rook_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the black/dark square
  {
    const QString filename{"d.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_black_square.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light bishop
  {
    const QString filename{"bw.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_bishop.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light bishop portrait
  {
    const QString filename{"bw_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_bishop_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light king
  {
    const QString filename{"kw.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_king.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light king portrait
  {
    const QString filename{"kw_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_king_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light knight
  {
    const QString filename{"nw.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_knight.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light knight portrait
  {
    const QString filename{"nw_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_knight_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light pawn
  {
    const QString filename{"pw.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_pawn.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light pawn portrait
  {
    const QString filename{"pw_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_pawn_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light queen
  {
    const QString filename{"qw.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_queen.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light queen portrait
  {
    const QString filename{"qw_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_queen_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light rook
  {
    const QString filename{"rw.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_rook.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light rook portrait
  {
    const QString filename{"rw_portrait.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_rook_portrait.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Load the white/light square
  {
    const QString filename{"l.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_white_square.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
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

sf::Texture& game_resources::get_piece(
  const chess_color color,
  const piece_type type
)
{
  if (color == chess_color::black)
  {
    if (type == piece_type::bishop) return m_black_bishop;
    if (type == piece_type::king) return m_black_king;
    if (type == piece_type::knight) return m_black_knight;
    if (type == piece_type::pawn) return m_black_pawn;
    if (type == piece_type::queen) return m_black_queen;
    if (type == piece_type::rook) return m_black_rook;
  }
  else
  {
    if (type == piece_type::bishop) return m_white_bishop;
    if (type == piece_type::king) return m_white_king;
    if (type == piece_type::knight) return m_white_knight;
    if (type == piece_type::pawn) return m_white_pawn;
    if (type == piece_type::queen) return m_white_queen;
    if (type == piece_type::rook) return m_white_rook;
  }

  assert(!"Should not get here");
  return m_white_square;
}

sf::Texture& game_resources::get_piece_portrait(
  const chess_color color,
  const piece_type type
)
{
  if (color == chess_color::black)
  {
    if (type == piece_type::bishop) return m_black_bishop_portrait;
    if (type == piece_type::king) return m_black_king_portrait;
    if (type == piece_type::knight) return m_black_knight_portrait;
    if (type == piece_type::pawn) return m_black_pawn_portrait;
    if (type == piece_type::queen) return m_black_queen_portrait;
    if (type == piece_type::rook) return m_black_rook_portrait;
  }
  else
  {
    if (type == piece_type::bishop) return m_white_bishop_portrait;
    if (type == piece_type::king) return m_white_king_portrait;
    if (type == piece_type::knight) return m_white_knight_portrait;
    if (type == piece_type::pawn) return m_white_pawn_portrait;
    if (type == piece_type::queen) return m_white_queen_portrait;
    if (type == piece_type::rook) return m_white_rook_portrait;
  }

  assert(!"Should not get here");
  return m_white_square;
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
  assert(r.get_white_square().getSize().x > 0.0);
  #endif
}

#endif // LOGIC_ONLY
