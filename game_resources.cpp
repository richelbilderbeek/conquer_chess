#include "game_resources.h"

#include <QFile>
#include <cassert>

game_resources::game_resources()
{
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
}

sf::Texture& game_resources::get_piece(
  const chess_color color,
  const piece_type type
)
{
  if (color == chess_color::black)
  {
    if (type == piece_type::bishop) return m_black_square;  // m_black_bishop;
    if (type == piece_type::king) return m_black_king;
    if (type == piece_type::queen) return m_black_queen;
    if (type == piece_type::rook) return m_black_rook;
  }
  else
  {
    if (type == piece_type::bishop) return m_white_bishop;
    if (type == piece_type::king) return m_white_king;
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
    if (type == piece_type::king) return m_black_square; //m_black_king_portrait;
    if (type == piece_type::queen) return m_black_square; // m_black_queen_portrait;
    if (type == piece_type::rook) return m_black_rook_portrait;
  }
  else
  {
    if (type == piece_type::bishop) return m_white_bishop_portrait;
    if (type == piece_type::king) return m_white_king_portrait;
    if (type == piece_type::queen) return m_white_queen_portrait;
    if (type == piece_type::rook) return m_white_rook_portrait;
  }

  assert(!"Should not get here");
  return m_white_square;
}

void test_game_resources()
{
  #ifndef NDEBUG // no tests in release
  game_resources r;
  assert(r.get_white_square().getSize().x > 0.0);
  #endif
}
