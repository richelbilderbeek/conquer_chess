#include "chess_move.h"

#include <cassert>
#include <iostream>
#include <regex>

chess_move::chess_move(std::string s, const chess_color color)
  : m_color{color},
    m_is_capture{false},
    m_pgn_str{s}
{

  // Simple 'go to there' move, no 'from'
  if (std::regex_match(s, std::regex("^[BKNQR]?x?[a-h][1-8](=[BKNQR])?(\\?\\?|\\?|\\+|#)?$")))
  {
    m_to.push_back(get_square(s));
    m_type.push_back(get_piece_type(s));
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
      m_castling_type.push_back(castling_type::king_side);
    }
    else
    {
      assert(s == "O-O-O");
      m_castling_type.push_back(castling_type::queen_side);
    }
  }
}

bool is_capture(const std::string& s)
{
  const std::regex e("x");
  std::smatch m;
  return std::regex_search(s, m, e);
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

std::vector<piece_type> get_promotion_type(const std::string& s)
{
  const std::regex e("=[BKNQR]");
  std::smatch m;
  const bool is_there{std::regex_search(s, m, e)};
  if (!is_there) return {};
  assert(!m.str().empty());
  assert(m.str().size() == 2);
  assert(m.str()[0] == '=');
  return { to_piece_type(m.str()[1]) };
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
  return !move.get_castling_type().empty();
}

bool is_draw(const chess_move& move) noexcept
{
  return move.get_winner().size() == 2;
}

bool is_promotion(const chess_move& move) noexcept
{
  return !move.get_promotion_type().empty();
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
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("e4"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::pawn);
    assert(!m.is_capture());
  }
  {
    const chess_move m("e5", chess_color::black);
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("e5"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::pawn);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Qh5", chess_color::white);
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("h5"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::queen);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Nc6", chess_color::black);
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("c6"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::knight);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Bc4", chess_color::white);
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("c4"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::bishop);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Nf6??", chess_color::black);
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("f6"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::knight);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Qxf7#", chess_color::white);
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("f7"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::queen);
    assert(m.is_capture());
  }
  {
    const chess_move m("1-0", chess_color::white);
    assert(m.get_to().empty());
    assert(m.get_type().empty());
    assert(!m.is_capture());
    assert(!m.get_winner().empty());
    assert(m.get_winner().at(0) == chess_color::white);
    assert(!m.is_capture());
  }
#endif // NDEBUG
}

std::ostream& operator<<(std::ostream& os, const chess_move& m) noexcept
{
  os << m.get_pgn_str();
  return os;
}
