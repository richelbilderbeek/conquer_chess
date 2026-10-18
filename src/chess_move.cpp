#include "chess_move.h"
#include "game.h"
#include "fen_string.h"
// Ignore warnings in the chess library, as is done in pieces.cpp
#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "chess.hpp"

#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif // _MSC_VER

#include <cassert>
#include <iostream>
#include <regex>
#include <sstream>

namespace {

/// Remove what is not part of the move itself:
/// a move number ('1.e4', '1...e5') and annotations ('Rxd4!!', 'Nf6??').
/// Castling written with zeros ('0-0', '0-0-0') is written with O's.
std::string normalize_move_str(std::string s)
{
  s = std::regex_replace(s, std::regex("^[0-9]+\\.+"), "");
  s = std::regex_replace(s, std::regex("[!?]+$"), "");
  s = std::regex_replace(s, std::regex("^0-0-0"), "O-O-O");
  s = std::regex_replace(s, std::regex("^0-0(?!-[01])"), "O-O");
  return s;
}

piece_type to_piece_type(const chess::PieceType pt) noexcept
{
  if (pt == chess::PieceType::BISHOP) return piece_type::bishop;
  if (pt == chess::PieceType::KING) return piece_type::king;
  if (pt == chess::PieceType::KNIGHT) return piece_type::knight;
  if (pt == chess::PieceType::PAWN) return piece_type::pawn;
  if (pt == chess::PieceType::QUEEN) return piece_type::queen;
  assert(pt == chess::PieceType::ROOK);
  return piece_type::rook;
}

square to_square(const chess::Square& s)
{
  return square(static_cast<std::string>(s));
}

/// Find the legal move described by the SAN string in this position, if any
std::optional<chess::Move> find_legal_move(const fen_string& fen_str, const std::string& san)
{
  try
  {
    const chess::Board board(fen_str.get());
    const chess::Move move{chess::uci::parseSan(board, san)};
    if (move == chess::Move::NO_MOVE) return {};
    return move;
  }
  catch (const std::exception&)
  {
    return {};
  }
}

} // namespace

chess_move::chess_move(
  const pgn_move_string& pgn_str,
  const fen_string& fen_str
)
  :
    m_color{::get_color(fen_str)},
    m_is_capture{false},
    m_pgn_str{pgn_str}
{
  const std::string s{normalize_move_str(pgn_str.get())};
  if (std::regex_match(s, std::regex("^(0-1)|1-0|(1/2-1/2)$")))
  {
    m_winner = ::get_winner(s);
    m_action_type = {};
  }
  else if (std::regex_match(s, std::regex("^O-O(-O)?[+#]?$")))
  {
    // Recognized without checking if castling is possible
    if (std::regex_match(s, std::regex("^O-O[+#]?$")))
    {
      m_castling_type = castling_type::king_side;
      m_action_type = piece_action_type::castle_kingside;
    }
    else
    {
      m_castling_type = castling_type::queen_side;
      m_action_type = piece_action_type::castle_queenside;
    }
    m_piece_type = piece_type::king;
    m_from = get_initial_king_square(m_color);
    m_to = get_king_target_square(m_color, m_action_type.value());
  }
  else if (const auto move{find_legal_move(fen_str, s)}; move.has_value())
  {
    // The chess library found the move in this position,
    // which takes care of disambiguation ('Nbd7'), pins,
    // pawn captures, en-passant and promotion
    const chess::Board board(fen_str.get());
    m_from = to_square(move->from());
    m_to = to_square(move->to());
    m_piece_type = to_piece_type(board.at<chess::PieceType>(move->from()));
    m_is_capture = board.isCapture(move.value());
    if (move->typeOf() == chess::Move::PROMOTION)
    {
      m_promotion_type = to_piece_type(move->promotionType());
    }
    if (move->typeOf() == chess::Move::ENPASSANT)
    {
      m_action_type = piece_action_type::attack_en_passant;
    }
    else
    {
      m_action_type = m_is_capture ? piece_action_type::attack : piece_action_type::move;
    }
  }
  else if (std::regex_match(s, std::regex("^[BKNQR]?x?[a-h][1-8](=[BKNQR])?(\\+|#)?$")))
  {
    // Not a legal move in this position: find the piece
    // by its type and the target square only
    m_to = get_square(s);
    m_from = ::get_from(fen_str, pgn_move_string(s));
    m_piece_type = ::get_piece_type(s);
    m_is_capture = ::is_capture(s);
    m_promotion_type = ::get_promotion_type(s);
    m_action_type = m_is_capture ? piece_action_type::attack : piece_action_type::move;
  }
}

bool is_capture(const pgn_move_string& s)
{
  const std::regex e("x");
  std::smatch m;
  return std::regex_search(s.get(), m, e);
}

std::optional<square> get_from(const fen_string& g, const pgn_move_string& m)
{
  if (get_piece_type(m).has_value())
  {
    const piece_type pt{get_piece_type(m).value()};
    switch (pt)
    {
      case piece_type::bishop: return get_from_for_bishop(g, m);
      case piece_type::king: return get_from_for_king(g, m);
      case piece_type::knight: return get_from_for_knight(g, m);
      case piece_type::pawn: return get_from_for_pawn(g, m);
      case piece_type::queen: return get_from_for_queen(g, m);
      case piece_type::rook:
        assert(pt == piece_type::rook);
        return get_from_for_rook(g, m);
    }
  }
  return {};
}

square get_from_for_bishop(const fen_string& s, const pgn_move_string& m)
{
  assert(get_piece_type(m).has_value());
  assert(get_piece_type(m).value() == piece_type::bishop);
  const auto g{create_game_from_fen_string(s)};
  const auto pieces{
    find_pieces(g, piece_type::bishop, get_color(s))
  };
  assert(!pieces.empty());
  assert(get_square(m).has_value());
  const square target{get_to(m).value()};

  std::optional<square> from_square;
  for (const auto& piece: pieces)
  {
    if (are_on_same_diagonal(piece.get_current_square(), target))
    {
      from_square = piece.get_current_square();
      break;
    }
  }
  assert(from_square.has_value() && "There had to be a bishop on the diagonal for this function to be called");
  return from_square.value();
}

square get_from_for_king(const fen_string& s, const pgn_move_string& m)
{
  assert(get_piece_type(m).has_value());
  assert(get_piece_type(m).value() == piece_type::king);
  const auto g{create_game_from_fen_string(s)};
  const auto pieces{
    find_pieces(g, piece_type::king, get_color(s))
  };
  assert(!pieces.empty());
  assert(pieces.size() == 1); // There is only 1 king
  return pieces[0].get_current_square();
}

square get_from_for_knight(const fen_string& s, const pgn_move_string& m)
{
  assert(get_piece_type(m).has_value());
  assert(get_piece_type(m).value() == piece_type::knight);
  const auto g{create_game_from_fen_string(s)};
  const auto pieces{
    find_pieces(g, piece_type::knight, get_color(s))
  };
  assert(!pieces.empty());
  assert(get_to(m).has_value());
  const square target{get_to(m).value()};

  std::optional<square> from_square;
  for (const auto& piece: pieces)
  {
    if (are_adjacent_for_knight(piece.get_current_square(), target))
    {
      from_square = piece.get_current_square();
      break;
    }
  }
  assert(from_square.has_value() && "there had to be a knight a knight's jump away for this function to be called");
  return from_square.value();
}

square get_from_for_pawn(const fen_string& s, const pgn_move_string& m)
{
  assert(get_piece_type(m).has_value());
  assert(get_piece_type(m).value() == piece_type::pawn);
  const auto g{create_game_from_fen_string(s)};
  assert(!is_capture(m));
  const square one_behind{
    get_behind(get_to(m).value(), get_color(s))
  };
  if (is_piece_at(g, one_behind))
  {
    // Pawns cannot jump, so if there is a pawn one behind the target square,
    // it must be from there
    assert(get_piece_at(g, one_behind).get_type() == piece_type::pawn);
    return one_behind;
  }
  const square two_behind{get_behind(one_behind, get_color(s))};
  assert(is_piece_at(g, two_behind));
  assert(get_piece_at(g, two_behind).get_type() == piece_type::pawn);
  return two_behind;
}

square get_from_for_queen(const fen_string& s, const pgn_move_string& m)
{
  assert(get_piece_type(m).has_value());
  assert(get_piece_type(m).value() == piece_type::queen);
  const auto g{create_game_from_fen_string(s)};
  assert(!g.get_pieces().empty());


  const auto pieces{
    find_pieces(g, piece_type::queen, get_color(s))
  };

  assert(!pieces.empty());
  assert(get_to(m).has_value());
  const square target{get_to(m).value()};

  std::optional<square> from_square;
  for (const auto& piece: pieces)
  {
    if (are_on_same_diagonal(piece.get_current_square(), target)
      || are_on_same_rank(piece.get_current_square(), target)
      || are_on_same_file(piece.get_current_square(), target)
    )
    {
      from_square = piece.get_current_square();
      break;
    }
  }


  assert(from_square.has_value() && "There had to be a queen on the same rank, file, or diagonal for this function to be called");
  return from_square.value();
}

square get_from_for_rook(const fen_string& s, const pgn_move_string& m)
{
  assert(get_piece_type(m).has_value());
  assert(get_piece_type(m).value() == piece_type::rook);
  const auto g{create_game_from_fen_string(s)};
  const auto pieces{
    find_pieces(g, piece_type::rook, get_color(s))
  };
  assert(!pieces.empty());
  assert(get_to(m).has_value());
  const square target{get_to(m).value()};

  std::optional<square> from_square;
  for (const auto& piece: pieces)
  {
    if (are_on_same_rank(piece.get_current_square(), target)
      || are_on_same_file(piece.get_current_square(), target)
    )
    {
      from_square = piece.get_current_square();
      break;
    }
  }
  assert(from_square.has_value() && "There had to be a rook on the same rank or file for this function to be called");
  return from_square.value();
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
    const pgn_move_string s{"e4"};
    const chess_move m{s, create_fen_string_of_standard_starting_position()};
    assert(s == m.get_pgn_str());
    assert(m.get_color() == chess_color::white);
  }
  // chess_move::get_pgn_str
  {
    const pgn_move_string pgn_str{"Qd2"};
    const chess_move m{pgn_str, create_fen_string_with_starting_position(starting_position_type::queen_end_game)};
    assert(pgn_str == m.get_pgn_str());
  }
  // Individual functions
  // get_from, pawn, e2-e3
  {
    const fen_string g{create_fen_string_of_standard_starting_position()};
    const pgn_move_string m("e3");
    assert(get_from(g, m).value() == square("e2"));
  }
  // get_from, knight, Nc3
  {
    const fen_string g{create_fen_string_of_standard_starting_position()};
    const pgn_move_string m("Nc3");
    assert(get_from(g, m).value() == square("b1"));
  }
  // get_from, bishop, Bb1
  {
    const fen_string g{create_fen_string_with_starting_position(starting_position_type::bishop_and_knight_end_game)};
    const pgn_move_string m("Bh5");
    assert(get_from(g, m).value() == square("g4"));
  }
  // get_from, rook, Rb1
  {
    const fen_string g{create_fen_string_with_starting_position(starting_position_type::ready_to_castle)};
    const pgn_move_string m("Rb1");
    assert(get_from(g, m).value() == square("a1"));
  }
  // get_from, queen, Qd1
  {
    const fen_string g{create_fen_string_with_starting_position(starting_position_type::queen_end_game)};
    const pgn_move_string m("Qc1");
    assert(get_from(g, m).value() == square("d1"));
  }
  // get_from, king, Kf1
  {
    const fen_string g{create_fen_string_with_starting_position(starting_position_type::ready_to_castle)};
    const pgn_move_string m("Kf1");
    assert(get_from(g, m).value() == square("e1"));
  }
  // get_from, castling
  {
    const fen_string g{create_fen_string_with_starting_position(starting_position_type::ready_to_castle)};
    const pgn_move_string m("O-O");
    assert(!get_from(g, m).has_value());
  }
  // is_capture
  // is_castling
  {
    assert(is_castling(chess_move(pgn_move_string("O-O"), create_fen_string_with_starting_position(starting_position_type::ready_to_castle, chess_color::white))));
    assert(is_castling(chess_move(pgn_move_string("O-O-O"), create_fen_string_with_starting_position(starting_position_type::ready_to_castle, chess_color::white))));
    assert(is_castling(chess_move(pgn_move_string("O-O"), create_fen_string_with_starting_position(starting_position_type::ready_to_castle, chess_color::black))));
    assert(is_castling(chess_move(pgn_move_string("O-O-O"), create_fen_string_with_starting_position(starting_position_type::ready_to_castle, chess_color::black))));
    assert(!is_castling(chess_move(pgn_move_string("e4"), create_fen_string_of_standard_starting_position())));
    assert(!is_castling(chess_move(pgn_move_string("1-0"), create_fen_string_of_standard_starting_position())));
    assert(!is_castling(chess_move(pgn_move_string("1/2-1/2"), create_fen_string_of_standard_starting_position())));
  }
  // is_draw
  {
    assert(is_draw(chess_move(pgn_move_string("1/2-1/2"), create_fen_string_of_standard_starting_position())));
    assert(is_draw(chess_move(pgn_move_string("1/2-1/2"), create_fen_string_with_starting_position(starting_position_type::ready_to_castle, chess_color::black))));
    assert(!is_draw(chess_move(pgn_move_string("1-0"), create_fen_string_of_standard_starting_position())));
    assert(!is_draw(chess_move(pgn_move_string("0-1"), create_fen_string_of_standard_starting_position())));
    assert(!is_draw(chess_move(pgn_move_string("e4"), create_fen_string_of_standard_starting_position())));
  }
  // is_move
  {
    // e4
    {
      const chess_move m{pgn_move_string("e4"), create_fen_string_of_standard_starting_position()};
      assert(!is_capture(m));
      assert(!is_win(m));
      assert(!is_draw(m));
      assert(!is_promotion(m));
      assert(!is_castling(m));
      assert(is_simple_move(m));
    }
    {
      const chess_move m(
        pgn_move_string("Qxf7+"),
        create_fen_string_with_starting_position(starting_position_type::before_scholars_mate)
      );
      assert(is_capture(m));
      assert(!is_win(m));
      assert(!is_draw(m));
      assert(!is_promotion(m));
      assert(!is_castling(m));
      assert(!is_simple_move(m));
    }
    // Some quick ones
    {
      assert(is_simple_move(chess_move(pgn_move_string("e4"), create_fen_string_of_standard_starting_position())));
      assert(!is_simple_move(chess_move(pgn_move_string("Qxf7"), create_fen_string_with_starting_position(starting_position_type::before_scholars_mate))));
      assert(!is_simple_move(chess_move(pgn_move_string("Qxf7+"), create_fen_string_with_starting_position(starting_position_type::before_scholars_mate))));
      assert(!is_simple_move(chess_move(pgn_move_string("Qxf7#"), create_fen_string_with_starting_position(starting_position_type::before_scholars_mate))));
      assert(!is_simple_move(chess_move(pgn_move_string("0-1"), create_fen_string_of_standard_starting_position())));
      assert(!is_simple_move(chess_move(pgn_move_string("O-O"), create_fen_string_of_standard_starting_position())));
      assert(!is_simple_move(chess_move(pgn_move_string("a8=Q"), create_fen_string_with_starting_position(starting_position_type::pawns_near_promotion))));
    }
  }
  // is_promotion
  {
    assert(is_promotion(chess_move(pgn_move_string("a8=Q"), create_fen_string_with_starting_position(starting_position_type::pawns_near_promotion))));
    assert(!is_promotion(chess_move(pgn_move_string("a7"), create_fen_string_with_starting_position(starting_position_type::pawns_nearly_near_promotion))));
  }
  // '1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0'
  {
    const chess_move m(pgn_move_string("e4"), create_fen_string_of_standard_starting_position());
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("e4"));
    assert(m.get_piece_type().has_value());
    assert(m.get_piece_type().value() == piece_type::pawn);
    assert(!m.is_capture());
  }
  {
    const chess_move m(pgn_move_string("e5"), get_fen_string_wikipedia_1());
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("e5"));
    assert(m.get_piece_type().has_value());
    assert(m.get_piece_type().value() == piece_type::pawn);
    assert(!m.is_capture());
  }
  {
    const chess_move m(pgn_move_string("Qh5"), create_fen_string_of_standard_starting_position());
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("h5"));
    assert(m.get_piece_type().has_value());
    assert(m.get_piece_type().value() == piece_type::queen);
    assert(!m.is_capture());
  }
  {
    const chess_move m(pgn_move_string("Nc6"), create_fen_string_of_standard_starting_position(chess_color::black));
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("c6"));
    assert(m.get_piece_type().has_value());
    assert(m.get_piece_type().value() == piece_type::knight);
    assert(!m.is_capture());
  }
  {
    const chess_move m(pgn_move_string("Bc4"), create_fen_string_of_standard_starting_position());
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("c4"));
    assert(m.get_piece_type().has_value());
    assert(m.get_piece_type().value() == piece_type::bishop);
    assert(!m.is_capture());
  }
  {
    const chess_move m(pgn_move_string("Nf6??"), create_fen_string_of_standard_starting_position(chess_color::black));
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("f6"));
    assert(m.get_piece_type().has_value());
    assert(m.get_piece_type().value() == piece_type::knight);
    assert(!m.is_capture());
  }
  {
    const chess_move m(pgn_move_string("Qxf7#"), create_fen_string_with_starting_position(starting_position_type::before_scholars_mate));
    assert(m.get_to().has_value());
    assert(m.get_to().value() == square("f7"));
    assert(m.get_piece_type().has_value());
    assert(m.get_piece_type().value() == piece_type::queen);
    assert(m.is_capture());
  }
  {
    const chess_move m(pgn_move_string("1-0"), create_fen_string_with_starting_position(starting_position_type::before_scholars_mate));
    assert(!m.get_to().has_value());
    assert(!m.get_piece_type().has_value());
    assert(!m.is_capture());
    assert(!m.get_winner().empty());
    assert(m.get_winner().at(0) == chess_color::white);
    assert(!m.is_capture());
  }
  // operator==
  {
    // On string
    {
      const chess_move a(pgn_move_string("e3"), create_fen_string_of_standard_starting_position());
      const chess_move b(pgn_move_string("e3"), create_fen_string_of_standard_starting_position());
      const chess_move c(pgn_move_string("e4"), create_fen_string_of_standard_starting_position());
      assert(a == b);
      assert(!(a == c));
    }
    // On color
    {
      const chess_move a(pgn_move_string("e3"), create_fen_string_of_standard_starting_position());
      const chess_move b(pgn_move_string("e3"), create_fen_string_of_standard_starting_position());
      const chess_move c(pgn_move_string("e5"), create_fen_string_of_standard_starting_position(chess_color::black));
      assert(a == b);
      assert(!(a == c));
    }
  }
  // operator<<
  {
    const chess_move m(pgn_move_string("Qxf7#"), create_fen_string_with_starting_position(starting_position_type::before_scholars_mate));
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
