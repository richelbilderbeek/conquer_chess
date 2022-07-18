#include "textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <cassert>

textures::textures()
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

  // Title
  {
    const QString filename{"title.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_title.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Subtitle
  {
    const QString filename{"subtitle.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_subtitle.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Start
  {
    const QString filename{"start.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_start.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Options
  {
    const QString filename{"options.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_options.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // About
  {
    const QString filename{"about.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_about.loadFromFile(filename.toStdString()))
    {
      QString msg{"Cannot find image file '" + filename + "'"};
      throw std::runtime_error(msg.toStdString());
    }
  }
  // Quit
  {
    const QString filename{"quit.png"};
    QFile f(":/resources/" + filename);
    f.copy(filename);
    if (!m_quit.loadFromFile(filename.toStdString()))
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

sf::Texture& textures::get_piece(
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

sf::Texture& textures::get_piece_portrait(
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


sf::Texture& get_white_square(textures& t) noexcept
{
  return t.get_square(chess_color::white);
}

sf::Texture& textures::get_square(const chess_color color) noexcept
{
  switch (color)
  {
    case chess_color::black: return m_black_square;
    default:
    case chess_color::white: return m_white_square;
  }
}

void test_textures()
{
  #ifndef NDEBUG // no tests in release
  textures t;
  assert(t.get_square(chess_color::black).getSize().x > 0.0);
  #endif
}

#endif // LOGIC_ONLY
