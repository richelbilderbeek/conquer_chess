#include "replay.h"

#include "game.h"
#include "game_controller.h"
#include "pieces.h"
#include "pgn_game_string.h"
#include <cassert>

#include <sstream>

replay::replay(
  const action_history& r,
  const game_controller& c
) :
    m_action_history{r},
    m_game_controller{game_controller(game(), c.get_lobby_options())},
    m_index{0},
    m_initial_game_controller{c}
{

}

void replay::do_move(const delta_t& dt)
{

  const auto& g{m_game_controller.get_game()};

  // All moves are done
  if (m_index == static_cast<int>(m_action_history.get_value().get().size()))
  {
    m_game_controller.tick(dt);
    return;
  }

  delta_t dt_to_do(dt);
  while (1)
  {
    assert(m_index < static_cast<int>(m_action_history.get_value().get().size()));

    const in_game_time next_action_time{m_action_history.get_value().get()[m_index].first};
    if (next_action_time > get_in_game_time(m_game_controller) + dt_to_do)
    {
      // Just pass the time in the timestep, without any action
      // but passing the time
      m_game_controller.tick(dt_to_do);
      return;
    }

    // Forward to the next move
    double forward_time_raw{next_action_time.get() - g.get_in_game_time().get()};
    assert(forward_time_raw >= -0.00000001);
    const delta_t forward_time{std::max(forward_time_raw, 0.0)};
    m_game_controller.tick(forward_time);
    dt_to_do = dt_to_do - forward_time;

    // Make the piece do the action retrieved from history
    const piece_action& next_action{m_action_history.get_value().get().at(m_index).second};
    get_piece_at(m_game_controller, next_action.get_from()).add_action(next_action);

    ++m_index;

    if (m_index == static_cast<int>(m_action_history.get_value().get().size()))
    {
      // Tick away the last time.
      m_game_controller.tick(dt_to_do);

      // Need another second to finish the moves
      // assert(this->is_done());
      return;
    }
  }
}

game_statistics_in_time extract_game_statistics_in_time(
  const replay& r_original,
  const delta_t& dt
)
{
  game_statistics_in_time s;
  replay r(
    r_original.get_action_history().get_value(),
    game_controller()
  );
  assert(get_in_game_time(r) == in_game_time(0.0));
  while (!r.is_done())
  {
    s.add(r.get_game_controller());
    r.do_move(dt);
  }

  // Add the tail
  assert(dt.get() != 0.0);
  const int n{2 * (1 + static_cast<int>((1.0 / dt.get())))};
  for (int i{0}; i!=n; ++i)
  {
    s.add(r.get_game_controller());
    r.do_move(dt);
  }
  return s;
}

const in_game_time& get_in_game_time(
  const replay& r
) noexcept
{
  return r.get_game().get_in_game_time();
}

int get_n_moves(const replay& r) noexcept
{
  return get_n_piece_actions(r.get_action_history().get_value());
}

replay get_played_scholars_mate()
{
  // Scholar's mate
  // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
  replay r(
    create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0")),
    game_controller()
  );
  assert(get_n_moves(r) == 7);

  r.do_move(); // e2-e4
  r.do_move(); // e7-e5
  r.do_move(); // Qd1-h5
  r.do_move(); // Nb8-c6
  r.do_move(); // Bf1-c4
  r.do_move(); // Ng8-f6
  r.do_move(); // Qh5xf7#
  r.do_move(); // 1-0, which is ignored
  assert(is_checkmate(r.get_game().get_pieces(), chess_color::black));
  return r;
}

bool replay::is_done() const noexcept
{
  assert(m_index >= 0);
  const int n_moves{get_n_moves(*this)};
  assert(m_index <= n_moves);
  const bool has_started_last_move{
    m_index == n_moves
  };
  if (!has_started_last_move)
  {
    return false;
  }
  if (m_action_history.get_value().get().empty())
  {
    return true;
  }
  const in_game_time last_move_start{
    m_action_history.get_value().get().back().first
  };
  const in_game_time last_move_end{
    last_move_start + delta_t(1.0)
  };
  const in_game_time current_time{
    this->get_game().get_in_game_time()
  };
  return current_time >= last_move_end;
}

bool is_piece_at(const replay& r, const square& coordinate)
{
  return is_piece_at(r.get_game(), coordinate);
}

void replay::reset() noexcept
{
  m_game_controller = m_initial_game_controller.get_value();
  m_index = 0;
}

void test_replay()
{
#ifndef NDEBUG
  // replayer can replay the game it replayed exactly
  {
    const game g{play_random_game(2, 42)};
    const auto action_history_1{collect_action_history(g)};

    replay r(
      action_history_1,
      game_controller()
    );

    while (!r.is_done()) { r.do_move(); }

    const auto action_history_2{collect_action_history(r.get_game())};
    assert(action_history_1 == action_history_2);
  }
  // replayer contructor
  {
    const replay r;
    assert(get_n_moves(r) == 0);
    assert(r.get_index() == 0);
    assert(r.is_done());
  }
  // replayer::do_move on empty replay does nothing
  {
    replay r;
    assert(r.get_index() == 0);
    assert(get_in_game_time(r) == in_game_time(0.0));
    r.do_move();
    assert(r.get_index() == 0);
    assert(get_in_game_time(r) == in_game_time(1.0));
    assert(r.is_done());
  }
  // replayer::do_move does not increase last_time in 0.1 interval
  {
    replay r;
    r.do_move();
    assert(r.get_index() == 0);
    r.do_move(delta_t(0.1));
    assert(r.get_index() == 0);
    assert(r.is_done());
  }
  // replayer can forward a pawn two squares forward
  {
    replay r(
      create_action_history_from_pgn(pgn_game_string("1. e4")),
      game_controller()
    );
    assert(get_n_moves(r) == 1);
    assert(is_piece_at(r, square("e2")));
    assert(!is_piece_at(r, square("e4")));
    r.do_move();
    assert(get_in_game_time(r) == in_game_time(1.0));
    assert(!is_piece_at(r, square("e2")));
    assert(is_piece_at(r, square("e4")));
    assert(r.is_done());
  }
  // replayer can forward a pawn one square
  {
    replay r(create_action_history_from_pgn(pgn_game_string("1. e3")));
    assert(get_n_moves(r) == 1);
    assert(is_piece_at(r, square("e2")));
    assert(!is_piece_at(r, square("e3")));
    r.do_move();
    assert(!is_piece_at(r, square("e2")));
    assert(is_piece_at(r, square("e3")));
    assert(r.is_done());
  }
  // replayer can do Na3
  {
    replay r(create_action_history_from_pgn(pgn_game_string("1. Na3")));
    assert(get_n_moves(r) == 1);
    assert(is_piece_at(r, square("b1")));
    assert(!is_piece_at(r, square("a3")));
    r.do_move();
    assert(!is_piece_at(r, square("b1")));
    assert(is_piece_at(r, square("a3")));
    assert(r.is_done());
  }
  // replayer can do Nc3
  {
    replay r(create_action_history_from_pgn(pgn_game_string("1. Nc3")));
    assert(get_n_moves(r) == 1);
    assert(is_piece_at(r, square("b1")));
    assert(!is_piece_at(r, square("c3")));
    r.do_move();
    assert(!is_piece_at(r, square("b1")));
    assert(is_piece_at(r, square("c3")));
    assert(r.is_done());
  }
  // replayer can do two moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 e5")));
    assert(get_n_moves(r) == 2);

    r.do_move(); // e2-e4

    assert(is_piece_at(r, square("e7")));
    assert(!is_piece_at(r, square("e5")));
    assert(get_in_game_time(r) == in_game_time(1.0));

    assert(!r.is_done());

    r.do_move(); // e7-e5
    assert(!is_piece_at(r, square("e7")));
    assert(is_piece_at(r, square("e5")));
    assert(get_in_game_time(r) == in_game_time(2.0));
    assert(r.is_done());
  }
  // replayer can do three moves
  {
    // Scholar's mate
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5")));
    assert(get_n_moves(r) == 3);

    r.do_move(); // e2-e4

    assert(!r.is_done());

    r.do_move(); // e7-e5

    assert(is_piece_at(r, square("d1")));
    assert(!is_piece_at(r, square("h5")));
    assert(!r.is_done());

    r.do_move(); // Qd1-h5

    assert(!is_piece_at(r, square("d1")));
    assert(is_piece_at(r, square("h5")));
    assert(r.is_done());
  }
  // replayer can do four moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6")));
    assert(get_n_moves(r) == 4);

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5

    assert(is_piece_at(r, square("b8")));
    assert(!is_piece_at(r, square("c6")));
    r.do_move(); // Nb8-c6
    assert(!is_piece_at(r, square("b8")));
    assert(is_piece_at(r, square("c6")));
  }
  // replayer can do five moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4")));
    assert(get_n_moves(r) == 5);

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6

    assert(is_piece_at(r, square("f1")));
    assert(!is_piece_at(r, square("c4")));
    r.do_move(); // Bf1-c4
    assert(!is_piece_at(r, square("f1")));
    assert(is_piece_at(r, square("c4")));
  }
  // replayer can do six moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6")));
    assert(get_n_moves(r) == 6);
    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    r.do_move(); // Bf1-c4

    assert(is_piece_at(r, square("g8")));
    assert(!is_piece_at(r, square("f6")));
    r.do_move(); // Ng8-f6
    assert(!is_piece_at(r, square("g8")));
    assert(is_piece_at(r, square("f6")));
  }
  // replayer can do seven moves
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7#")));
    assert(get_n_moves(r) == 7);

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    r.do_move(); // Bf1-c4
    r.do_move(); // Ng8-f6

    assert(is_piece_at(r, square("h5")));
    assert(is_piece_at(r, square("f7"))); // The piece to be captures
    r.do_move(); // Qh5xf7#
    assert(!is_piece_at(r, square("h5")));
    assert(is_piece_at(r, square("f7")));
  }
  // replayer can do seven moves with score
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0")));
    assert(get_n_moves(r) == 7);

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    r.do_move(); // Bf1-c4
    r.do_move(); // Ng8-f6
    r.do_move(); // Qh5xf7#
    r.do_move(); // 1-0, which is ignored
  }
  // replayer can do seven moves with score and conclude checkmate
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0")));
    assert(get_n_moves(r) == 7);

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    r.do_move(); // Bf1-c4
    r.do_move(); // Ng8-f6
    r.do_move(); // Qh5xf7#
    r.do_move(); // 1-0, which is ignored
    assert(is_checkmate(r.get_game().get_pieces(), chess_color::black));
  }
  // replayer can do fools mate and conclude checkmate
  {
    // Fools's mate
    // 1. f3 e6 2. g4 Qh4#
    replay r(create_action_history_from_pgn(pgn_game_string("1. f3 e6 2. g4 Qh4#")));
    assert(get_n_moves(r) == 4);

    r.do_move(); // e2-e4
    r.do_move(); // e7-e5
    r.do_move(); // Qd1-h5
    r.do_move(); // Nb8-c6
    assert(is_checkmate(r.get_game().get_pieces(), chess_color::white));
    assert(!is_checkmate(r.get_game().get_pieces(), chess_color::black));
  }
  // extract_game_statistics_in_time
  {
    const auto r = get_played_scholars_mate();
    const auto statistics = extract_game_statistics_in_time(r, delta_t(0.2));
    assert(!statistics.get().empty());
  }
  // get_played_scholars_mate
  {
    const auto r{get_played_scholars_mate()};
    assert(is_checkmate(r.get_game().get_pieces(), chess_color::black));
    assert(r.get_game().get_winner().value() == chess_color::white);
  }
  // replayer::reset
  {
    // Scholar's mate
    // 1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6?? Qxf7# 1-0
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 e5 2. Qh5")));
    assert(r.get_index() == 0);
    r.do_move(); // e2-e4
    assert(r.get_index() != 0);
    r.reset();
    assert(r.get_index() == 0);
  }
  // to_fen_string
  {
    // Scholar's mate
    // 1. e4 c5 Nf3
    replay r(create_action_history_from_pgn(pgn_game_string("1. e4 c5 2. Nf3")));
    assert(get_n_moves(r) == 3);

    const auto s1{to_fen_str(r.get_game().get_pieces())};
    assert(s1 == "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    r.do_move(); // e2-e4

    const auto s2{
      to_fen_str(
        r.get_game().get_pieces(),
        chess_color::black,
        "KQkq",
        "e3",
        0,
        1
      )
    };
    assert(s2 == "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1");

    r.do_move(); // c5

    const auto s3{
      to_fen_str(
        r.get_game().get_pieces(),
        chess_color::white,
        "KQkq",
        "c6",
        0,
        2
      )
    };
    assert(s3 == "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2");

    r.do_move(); // Nf3

    const auto s4{
      to_fen_str(
        r.get_game().get_pieces(),
        chess_color::black,
        "KQkq",
        "-",
        1,
        2
      )
    };
    assert(s4 == "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
  }
  // operator<<
  {
    const replay r;
    std::stringstream s;
    s << r;
    assert(!s.str().empty());
  }
  // operator==
  {
    const replay a;
    const replay b;
    assert(a == b);
  }
#endif // NDEBUG
}

bool operator==(const replay& lhs, const replay& rhs) noexcept
{
  return lhs.get_action_history().get_value() == rhs.get_action_history().get_value()
    && lhs.get_index() == rhs.get_index()
  ;
}

std::ostream& operator<<(std::ostream& os, const replay& r) noexcept
{
  os
    << "Last time: " << r.get_index() << '\n'
    << "Action history: " << r.get_action_history().get_value()
  ;
  return os;

}
