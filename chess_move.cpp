#include "chess_move.h"
#include "game.h"

#include <cassert>
#include <iostream>
#include <regex>


chess_move::chess_move(std::string s, const chess_color color)
  : m_color{color},
    m_is_capture{false},
    m_pgn_str{s}
{

  // Simple 'go to there' move, no 'from'

  if (std::regex_match(s, std::regex("^[BKNQR]?([a-h]?)x?[a-h][1-8](=[BKNQR])?(\\?\\?|\\?|\\+|#)?$")))
  {
    m_to = get_square(s);
    m_type = get_piece_type(s);
    m_is_capture = ::is_capture(s);
    m_promotion_type = ::get_promotion_type(s);
  }
  else if (std::regex_match(s, std::regex("^(0-1)|1-0|(1/2-1/2)$")))
  {
    m_winner = ::get_winner(s);
  }
  else if (std::regex_match(s, std::regex("^O-O(-O)?$")))
  {
    if (s == "O-O")
    {
      m_castling_type = castling_type::king_side;
    }
    else
    {
      assert(s == "O-O-O");
      m_castling_type = castling_type::queen_side;
    }
  }
}

bool is_capture(const std::string& s)
{
  const std::regex e("x");
  std::smatch m;
  return std::regex_search(s, m, e);
}

square get_from(const game& g, const chess_move& m)
{
  if (m.get_type().has_value())
  {
    const piece_type pt{m.get_type().value()};
    switch (pt)
    {
      case piece_type::bishop: return get_from_for_bishop(g, m);
      case piece_type::king: return get_from_for_king(g, m);
      case piece_type::knight: return get_from_for_knight(g, m);
      case piece_type::pawn: return get_from_for_pawn(g, m);
      case piece_type::queen: return get_from_for_queen(g, m);
      default:
      case piece_type::rook:
        assert(pt == piece_type::rook);
        return get_from_for_rook(g, m);
    }
  }
  assert(!"TODO");
}

square get_from_for_bishop(const game& g, const chess_move& m)
{
  assert(m.get_type().has_value());
  assert(m.get_type().value() == piece_type::bishop);
  const auto pieces{
    find_pieces(g, piece_type::bishop, m.get_color())
  };
  assert(!pieces.empty());
  assert(m.get_to().has_value());
  const square target{m.get_to().value()};
  for (const auto& piece: pieces)
  {
    if (are_on_same_diagonal(piece.get_current_square(), target))
    {
      return piece.get_current_square();
    }
  }
  assert(!"Should not get here: there had to be a bishop on the diagonal");
}

square get_from_for_king(const game& g, const chess_move& m)
{
  assert(m.get_type().has_value());
  assert(m.get_type().value() == piece_type::king);
  const auto pieces{
    find_pieces(g, piece_type::king, m.get_color())
  };
  assert(!pieces.empty());
  assert(pieces.size() == 1); // There is only 1 king
  return pieces[0].get_current_square();
}

square get_from_for_knight(const game& g, const chess_move& m)
{
  assert(m.get_type().has_value());
  assert(m.get_type().value() == piece_type::knight);
  const auto pieces{
    find_pieces(g, piece_type::knight, m.get_color())
  };
  assert(!pieces.empty());
  assert(m.get_to().has_value());
  const square target{m.get_to().value()};

  for (const auto& piece: pieces)
  {
    if (are_adjacent_for_knight(piece.get_current_square(), target))
    {
      return piece.get_current_square();
    }
  }
  assert(!"Should not get here: there had to be a knight a knight's jump away");
}

square get_from_for_pawn(const game& g, const chess_move& m)
{
  assert(m.get_type().has_value());
  assert(m.get_type().value() == piece_type::pawn);
  if (!m.is_capture())
  {
    const square one_behind{
      get_behind(m.get_to().value(), m.get_color())
    };
    if (is_piece_at(g, one_behind))
    {
      assert(get_piece_at(g, one_behind).get_type() == piece_type::pawn);
      return one_behind;
    }
    const square two_behind{get_behind(one_behind, m.get_color())};
    assert(is_piece_at(g, two_behind));
    assert(get_piece_at(g, two_behind).get_type() == piece_type::pawn);
    return two_behind;
  }
  else
  {
    const square one_behind{
      get_behind(m.get_to().value(), m.get_color())
    };
    // 1 or 2 squares
    const auto& squares{get_besides(one_behind)};
    assert(!squares.empty());
    if (squares.size() == 1) return squares[0];
    assert(squares.size() == 2);
    const bool is_piece_up{is_piece_at(g, squares[0])};
    const bool is_piece_down{is_piece_at(g, squares[1])};
    assert(is_piece_up || is_piece_down);
    if (is_piece_up && !is_piece_down) return squares[0];
    if (!is_piece_up && is_piece_down) return squares[1];
    const std::string s{m.get_pgn_str()};
    assert(s.find('x') != std::string::npos);

    const auto xpos{s.find('x')};
    assert(xpos > 0);
    const char file_char{s[xpos - 1]};
    if (get_file_char(squares[0]) == file_char)
    {
      return squares[0];
    }
    assert(get_file_char(squares[1]) == file_char);
    return squares[1];
  }
}

square get_from_for_queen(const game& g, const chess_move& m)
{
  assert(m.get_type().has_value());
  assert(m.get_type().value() == piece_type::queen);
  const auto pieces{
    find_pieces(g, piece_type::queen, m.get_color())
  };
  assert(!pieces.empty());
  assert(m.get_to().has_value());
  const square target{m.get_to().value()};
  for (const auto& piece: pieces)
  {
    if (are_on_same_diagonal(piece.get_current_square(), target)
      || are_on_same_rank(piece.get_current_square(), target)
      || are_on_same_file(piece.get_current_square(), target)
    )
    {
      return piece.get_current_square();
    }
  }
  assert(!"Should not get here: there had to be a queen on the same rank, file, or diagonal");
}

square get_from_for_rook(const game& g, const chess_move& m)
{
  assert(m.get_type().has_value());
  assert(m.get_type().value() == piece_type::rook);
  const auto pieces{
    find_pieces(g, piece_type::rook, m.get_color())
  };
  assert(!pieces.empty());
  assert(m.get_to().has_value());
  const square target{m.get_to().value()};
  for (const auto& piece: pieces)
  {
    if (are_on_same_rank(piece.get_current_square(), target)
      || are_on_same_file(piece.get_current_square(), target)
    )
    {
      return piece.get_current_square();
    }
  }
  assert(!"Should not get here: there had to be a rook on the same rank or file");
}


piece_type get_piece_type(const std::string& s)
{
  const std::regex e("[BKNQR]");
  std::smatch m;
  const bool is_there{std::regex_search(s, m, e)};
  if (!is_there) return piece_type::pawn;
  assert(!m.str().empty());
  assert(m.str().size() == 1);
  return to_piece_type(m.str()[0]);
}

std::optional<piece_type> get_promotion_type(const std::string& s)
{
  const std::regex e("=[BKNQR]");
  std::smatch m;
  const bool is_there{std::regex_search(s, m, e)};
  if (!is_there) return {};
  assert(!m.str().empty());
  assert(m.str().size() == 2);
  assert(m.str()[0] == '=');
  return to_piece_type(m.str()[1]);
}

square get_square(const std::string& s)
{
  const std::regex e("[a-h][1-8]");
  std::smatch m;
  const bool is_there{std::regex_search(s, m, e)};
  assert(is_there);
  return square(m.str());
}

std::vector<chess_color> get_winner(const std::string& s)
{
  assert(std::regex_match(s, std::regex("^(0-1)|1-0|(1/2-1/2)$")));
  if (s == "1-0") return {  chess_color::white };
  if (s == "0-1") return {  chess_color::black };
  assert(s == "1/2-1/2");
  return { chess_color::white, chess_color::black };
}

bool is_capture(const chess_move& move) noexcept
{
  return move.is_capture();
}

bool is_castling(const chess_move& move) noexcept
{
  return move.get_castling_type().has_value();
}

bool is_draw(const chess_move& move) noexcept
{
  return move.get_winner().size() == 2;
}

bool is_promotion(const chess_move& move) noexcept
{
  return move.get_promotion_type().has_value();
}

bool is_simple_move(const chess_move& move) noexcept
{
  return !move.is_capture()
    && !is_win(move)
    && !is_draw(move)
    && !is_promotion(move)
    && !is_castling(move)
  ;
}

bool is_win(const chess_move& move) noexcept
{
  return move.get_winner().size() == 1;
}

void test_chess_move()
{
#ifndef NDEBUG
  // Constructor
  {
    const std::string pgn_str{"e4"};
    const chess_move m{pgn_str, chess_color::white};
    assert(pgn_str == m.get_pgn_str());
  }
  // chess_move::get_pgn_str
  {
    const std::string pgn_str{"Qd4"};
    const chess_move m{pgn_str, chess_color::white};
    assert(pgn_str == m.get_pgn_str());
  }
  // Individual functions
  // get_from: e2-e3
  {
    const game g;
    const chess_move m("e3", chess_color::white);
    assert(get_from(g, m) == square("e2"));
  }
  // get_winner
  {
    assert(get_winner("0-1").at(0) == chess_color::black);
    assert(get_winner("1-0").at(0) == chess_color::white);
    assert(get_winner("1/2-1/2").size() == 2);
  }
  // is_capture
  // is_castling
  {
    assert(is_castling(chess_move("O-O", chess_color::white)));
    assert(is_castling(chess_move("O-O-O", chess_color::white)));
    assert(is_castling(chess_move("O-O", chess_color::black)));
    assert(is_castling(chess_move("O-O-O", chess_color::black)));
    assert(!is_castling(chess_move("e4", chess_color::white)));
    assert(!is_castling(chess_move("1-0", chess_color::white)));
    assert(!is_castling(chess_move("1/2-1/2", chess_color::white)));
  }
  // is_draw
  {
    assert(is_draw(chess_move("1/2-1/2", chess_color::white)));
    assert(is_draw(chess_move("1/2-1/2", chess_color::black)));
    assert(!is_draw(chess_move("1-0", chess_color::white)));
    assert(!is_draw(chess_move("0-1", chess_color::white)));
    assert(!is_draw(chess_move("e4", chess_color::white)));
  }
  // is_move
  {
    // e4
    {
      const chess_move m{"e4", chess_color::white};
      assert(!is_capture(m));
      assert(!is_win(m));
      assert(!is_draw(m));
      assert(!is_promotion(m));
      assert(!is_castling(m));
      assert(is_simple_move(m));
    }
    {
      const chess_move m("Qxf7+", chess_color::white);
      assert(is_capture(m));
      assert(!is_win(m));
      assert(!is_draw(m));
      assert(!is_promotion(m));
      assert(!is_castling(m));
      assert(!is_simple_move(m));
    }
    // Some quick ones
    {
      assert(is_simple_move(chess_move("e4", chess_color::white)));
      assert(!is_simple_move(chess_move("Qxf7", chess_color::white)));
      assert(!is_simple_move(chess_move("Qxf7+", chess_color::white)));
      assert(!is_simple_move(chess_move("Qxf7#", chess_color::white)));
      assert(!is_simple_move(chess_move("0-1", chess_color::white)));
      assert(!is_simple_move(chess_move("O-O", chess_color::white)));
      assert(!is_simple_move(chess_move("e8=Q", chess_color::white)));
    }
  }
  // is_promotion
  {
    assert(is_promotion(chess_move("e8=Q", chess_color::white)));
    assert(!is_promotion(chess_move("e7", chess_color::white)));
  }
  // '1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0'
  {
    const chess_move m("e4", chess_color::white);
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("e4"));
    assert(m.get_type().has_value());
    assert(m.get_type().value() == piece_type::pawn);
    assert(!m.is_capture());
  }
  {
    const chess_move m("e5", chess_color::black);
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("e5"));
    assert(m.get_type().has_value());
    assert(m.get_type().value() == piece_type::pawn);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Qh5", chess_color::white);
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("h5"));
    assert(m.get_type().has_value());
    assert(m.get_type().value() == piece_type::queen);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Nc6", chess_color::black);
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("c6"));
    assert(m.get_type().has_value());
    assert(m.get_type().value() == piece_type::knight);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Bc4", chess_color::white);
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("c4"));
    assert(m.get_type().has_value());
    assert(m.get_type().value() == piece_type::bishop);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Nf6??", chess_color::black);
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("f6"));
    assert(m.get_type().has_value());
    assert(m.get_type().value() == piece_type::knight);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Qxf7#", chess_color::white);
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("f7"));
    assert(m.get_type().has_value());
    assert(m.get_type().value() == piece_type::queen);
    assert(m.is_capture());
  }
  {
    const chess_move m("1-0", chess_color::white);
    assert(!m.get_to().has_value());
    assert(!m.get_type().has_value());
    assert(!m.is_capture());
    assert(!m.get_winner().empty());
    assert(m.get_winner().at(0) == chess_color::white);
    assert(!m.is_capture());
  }
  // gxh6
  {
    const chess_move m("gxh6", chess_color::white);
    assert(is_capture(m));
    assert(!is_win(m));
    assert(!is_draw(m));
    assert(!is_promotion(m));
    assert(!is_castling(m));
    assert(!is_simple_move(m));
  }
  // operator==
  {
    // On string
    {
      const chess_move a("e2", chess_color::white);
      const chess_move b("e2", chess_color::white);
      const chess_move c("e3", chess_color::white);
      assert(a == b);
      assert(!(a == c));
    }
    // On color
    {
      const chess_move a("Qxf7#", chess_color::white);
      const chess_move b("Qxf7#", chess_color::white);
      const chess_move c("Qxf7#", chess_color::black);
      assert(a == b);
      assert(!(a == c));
    }
  }
  // operator<<
  {
    const chess_move m("Qxf7#", chess_color::white);
    std::stringstream s;
    s << m;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const chess_move& lhs, const chess_move& rhs) noexcept
{
  return lhs.get_pgn_str() == rhs.get_pgn_str()
    && lhs.get_color() == rhs.get_color()
  ;
}

std::ostream& operator<<(std::ostream& os, const chess_move& m) noexcept
{
  os << m.get_pgn_str();
  return os;
}
