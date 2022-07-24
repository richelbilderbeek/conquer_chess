#include "textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>

textures::textures()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Texture>, std::string>> v = {
    std::make_pair(std::ref(m_black_bishop), "bb.png"),
    std::make_pair(std::ref(m_black_bishop_portrait), "bb_portrait.png"),
    std::make_pair(std::ref(m_black_king), "kb.png"),
    std::make_pair(std::ref(m_black_king_portrait), "kb_portrait.png"),
    std::make_pair(std::ref(m_black_knight), "nb.png"),
    std::make_pair(std::ref(m_black_knight_portrait), "nb_portrait.png"),
    std::make_pair(std::ref(m_black_pawn), "pb.png"),
    std::make_pair(std::ref(m_black_pawn_portrait), "pb_portrait.png"),
    std::make_pair(std::ref(m_black_queen), "qb.png"),
    std::make_pair(std::ref(m_black_queen_portrait), "qb_portrait.png"),
    std::make_pair(std::ref(m_black_rook), "rb.png"),
    std::make_pair(std::ref(m_black_rook_portrait), "rb_portrait_2.png"),
    std::make_pair(std::ref(m_black_square), "d.png"),
    std::make_pair(std::ref(m_dark_strip), "dark_strip.png"),
    std::make_pair(std::ref(m_light_strip), "light_strip.png"),
    std::make_pair(std::ref(m_subtitle), "subtitle.png"),
    std::make_pair(std::ref(m_title), "title.png"),
    std::make_pair(std::ref(m_white_bishop), "bw.png"),
    std::make_pair(std::ref(m_white_bishop_portrait), "bw_portrait.png"),
    std::make_pair(std::ref(m_white_king), "kw.png"),
    std::make_pair(std::ref(m_white_king_portrait), "kw_portrait.png"),
    std::make_pair(std::ref(m_white_knight), "nw.png"),
    std::make_pair(std::ref(m_white_knight_portrait), "nw_portrait.png"),
    std::make_pair(std::ref(m_white_pawn), "pw.png"),
    std::make_pair(std::ref(m_white_pawn_portrait), "pw_portrait.png"),
    std::make_pair(std::ref(m_white_queen), "qw.png"),
    std::make_pair(std::ref(m_white_queen_portrait), "qw_portrait.png"),
    std::make_pair(std::ref(m_white_rook), "rw.png"),
    std::make_pair(std::ref(m_white_square), "l.png"),
    std::make_pair(std::ref(m_white_rook_portrait), "rw_portrait.png")
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
    case chess_color::white:
      assert(color == chess_color::white);
      return m_white_square;
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

void test_textures()
{
  #ifndef NDEBUG // no tests in release
  textures t;
  assert(t.get_square(chess_color::black).getSize().x > 0.0);
  #endif
}

#endif // LOGIC_ONLY
