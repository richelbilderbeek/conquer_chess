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
    if (type == piece_type::king) return m_black_king;
    if (type == piece_type::queen) return m_black_queen;
  }
  else
  {
    if (type == piece_type::king) return m_white_king;
    if (type == piece_type::queen) return m_white_queen;
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
    if (type == piece_type::king) return m_white_king_portrait;
    if (type == piece_type::queen) return m_white_king_portrait;
  }
  else
  {
    if (type == piece_type::king) return m_white_king_portrait;
    if (type == piece_type::queen) return m_white_queen_portrait;
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
