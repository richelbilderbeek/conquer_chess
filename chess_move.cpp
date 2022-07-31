#include "chess_move.h"

#include <cassert>
#include <iostream>
#include <regex>

chess_move::chess_move(std::string s)
  : m_is_capture{false}
{
  // Simple 'go to there' move, no 'from'
  if (std::regex_match(s, std::regex("^[BKNQR]?x?[a-h][1-8](\\?\\?|\\?|#)?$")))
  {
    m_to.push_back(get_square(s));
    m_type.push_back(get_piece_type(s));
    m_is_capture = get_is_capture(s);
  }
  else if (std::regex_match(s, std::regex("^(0-1)|1-0|(1/2-1/2)$")))
  {
    m_winner = ::get_winner(s);
  }
}

bool get_is_capture(const std::string& s)
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

void test_chess_move()
{
#ifndef NDEBUG
  // Individual functions
  // get_winner
  {
    assert(get_winner("0-1").at(0) == chess_color::black);
    assert(get_winner("1-0").at(0) == chess_color::white);
    assert(get_winner("1/2-1/2").size() == 2);
  }
  // '1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0'
  {
    const chess_move m("e4");
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("e4"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::pawn);
    assert(!m.is_capture());
  }
  {
    const chess_move m("e5");
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("e5"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::pawn);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Qh5");
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("h5"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::queen);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Nc6");
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("c6"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::knight);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Bc4");
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("c4"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::bishop);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Nf6??");
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("f6"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::knight);
    assert(!m.is_capture());
  }
  {
    const chess_move m("Qxf7#");
    assert(!m.get_to().empty());
    assert(m.get_to().at(0) == square("f7"));
    assert(!m.get_type().empty());
    assert(m.get_type().at(0) == piece_type::queen);
    assert(m.is_capture());
  }
  {
    const chess_move m("1-0");
    assert(m.get_to().empty());
    assert(m.get_type().empty());
    assert(!m.is_capture());
    assert(!m.get_winner().empty());
    assert(m.get_winner().at(0) == chess_color::white);
    assert(!m.is_capture());
  }

  assert(2==3);
#endif // NDEBUG
}
