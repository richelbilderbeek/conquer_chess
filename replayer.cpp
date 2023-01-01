#include "replayer.h"
#include "game.h"

#include <cassert>
#include <iostream>
#include <sstream>

replayer::replayer(const replay& r)
  : m_last_time{-1.0},
    m_replay{r}
{

}


void replayer::do_move(game& g)
{
  // Do one move per chess move
  if (g.get_time() - m_last_time < delta_t(1.0)) return;

  m_last_time = g.get_time();

  const int move_index{static_cast<int>(m_last_time.get())};

  if (move_index >= get_n_moves(m_replay)) return;

  // Do the move
  const auto& move{m_replay.get_moves().at(move_index)};
  std::clog << g.get_time() << ": replayer doing " << move << '\n';
  const auto inputs{convert_move_to_user_inputs(g, move)};
  std::clog << g.get_time() << ": replayer doing inputs:\n" << user_inputs(inputs) << '\n';
  add_user_inputs(g, inputs);
}

int get_n_moves(const replayer& r) noexcept
{
  return get_n_moves(r.get_replay());
}

void test_replayer()
{
#ifndef NDEBUG
  // replayer contructor
  {
    const replayer r;
    assert(get_n_moves(r) == 0);
    assert(r.get_last_time() == delta_t(-1.0));
  }
  // replayer::do_move on empty replay does nothing
  {
    replayer r;
    game g;
    assert(r.get_last_time() == delta_t(-1.0));
    r.do_move(g);
    assert(r.get_last_time() == delta_t(0.0));
  }
  // replayer::do_move does not increase last_time in 0.1 interval
  {
    replayer r;
    game g;
    r.do_move(g);
    assert(r.get_last_time() == delta_t(0.0));
    g.tick(delta_t(0.1));
    r.do_move(g);
    assert(r.get_last_time() == delta_t(0.0));
  }
  // replayer::do_move on one move does it
  {
    replayer r(replay("1. e4"));
    assert(get_n_moves(r) == 1);
    game g;
    assert(is_piece_at(g, square("e2")));
    assert(!is_piece_at(g, square("e4")));
    r.do_move(g);
    #ifdef FIX_ISSUE_64_ANOTHER
    assert(!is_piece_at(g, square("e2")));
    assert(is_piece_at(g, square("e4")));
    #endif // FIX_ISSUE_64_ANOTHER
  }
  // 38: operator<<
  {
    const replayer r;
    std::stringstream s;
    s << r;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

bool operator==(const replayer& lhs, const replayer& rhs) noexcept
{
  return lhs.get_replay() == rhs.get_replay()
    && lhs.get_last_time() == rhs.get_last_time()
  ;
}

std::ostream& operator<<(std::ostream& os, const replayer& r) noexcept
{
  os
    << "Last time: " << r.get_last_time() << '\n'
    << "Replay: " << r.get_replay()
  ;
  return os;

}
