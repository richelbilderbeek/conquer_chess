#include "piece_textures.h"

#ifndef LOGIC_ONLY

#include "game_resources.h"

#include <QFile>
#include <functional>
#include <cassert>

piece_textures::piece_textures()
{
  const std::vector<std::pair<std::reference_wrapper<sf::Texture>, std::string>> v = {
    std::make_pair(std::ref(m_black_bishop), "classic_black_bishop.png"),
    std::make_pair(std::ref(m_black_bishop_with_contour), "classic_black_bishop_with_contour.png"),
    std::make_pair(std::ref(m_black_king), "classic_black_king.png"),
    std::make_pair(std::ref(m_black_king_with_contour), "classic_black_king_with_contour.png"),
    std::make_pair(std::ref(m_black_knight), "classic_black_knight.png"),
    std::make_pair(std::ref(m_black_knight_with_contour), "classic_black_knight_with_contour.png"),
    std::make_pair(std::ref(m_black_pawn), "classic_black_pawn.png"),
    std::make_pair(std::ref(m_black_pawn_with_contour), "classic_black_pawn_with_contour.png"),
    std::make_pair(std::ref(m_black_queen), "classic_black_queen.png"),
    std::make_pair(std::ref(m_black_queen_with_contour), "classic_black_queen_with_contour.png"),
    std::make_pair(std::ref(m_black_rook), "classic_black_rook.png"),
    std::make_pair(std::ref(m_black_rook_with_contour), "classic_black_rook_with_contour.png"),
    std::make_pair(std::ref(m_white_bishop), "classic_white_bishop.png"),
    std::make_pair(std::ref(m_white_bishop_with_contour), "classic_white_bishop_with_contour.png"),
    std::make_pair(std::ref(m_white_king), "classic_white_king.png"),
    std::make_pair(std::ref(m_white_king_with_contour), "classic_white_king_with_contour.png"),
    std::make_pair(std::ref(m_white_knight), "classic_white_knight.png"),
    std::make_pair(std::ref(m_white_knight_with_contour), "classic_white_knight_with_contour.png"),
    std::make_pair(std::ref(m_white_pawn), "classic_white_pawn.png"),
    std::make_pair(std::ref(m_white_pawn_with_contour), "classic_white_pawn_with_contour.png"),
    std::make_pair(std::ref(m_white_queen), "classic_white_queen.png"),
    std::make_pair(std::ref(m_white_queen_with_contour), "classic_white_queen_with_contour.png"),
    std::make_pair(std::ref(m_white_rook), "classic_white_rook.png"),
    std::make_pair(std::ref(m_white_rook_with_contour), "classic_white_rook_with_contour.png")
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

sf::Texture& piece_textures::get_piece(
  const race race,
  const chess_color color,
  const piece_type type
)
{
  switch (race)
  {
    case race::classic:
      if (color == chess_color::black)
      {
        if (type == piece_type::bishop) return m_black_bishop_with_contour;
        if (type == piece_type::king) return m_black_king_with_contour;
        if (type == piece_type::knight) return m_black_knight_with_contour;
        if (type == piece_type::pawn) return m_black_pawn_with_contour;
        if (type == piece_type::queen) return m_black_queen_with_contour;
        assert(type == piece_type::rook);
        return m_black_rook_with_contour;
      }
      else
      {
        assert(color == chess_color::white);
        if (type == piece_type::bishop) return m_white_bishop_with_contour;
        if (type == piece_type::king) return m_white_king_with_contour;
        if (type == piece_type::knight) return m_white_knight_with_contour;
        if (type == piece_type::pawn) return m_white_pawn_with_contour;
        if (type == piece_type::queen) return m_white_queen_with_contour;
        assert(type == piece_type::rook);
        return m_white_rook_with_contour;
      }
    case race::protoss:
    case race::terran:
    case race::zerg:
    default:
      return get_piece(race::classic, color, type);
  }
}

#endif // LOGIC_ONLY
