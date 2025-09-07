#include "action_history.h"

#include "delta_t.h"
#include "game_controller.h"
#include "pgn_game_string.h"
#include "chess_move.h"
#include "pgn_game_string.h"
//#include "pieces.h"
#include "game.h"


#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>

action_history::action_history(
  const std::vector<std::pair<in_game_time, piece_action>>& timed_actions
) : m_timed_actions{timed_actions}
{
  assert(
    std::is_sorted(
      std::begin(m_timed_actions),
      std::end(m_timed_actions),
      [](const auto& lhs, const auto& rhs)
      {
        return lhs.first < rhs.first;
      }
    )
  );
}

void action_history::add_action(const in_game_time& in_game_time, const piece_action& action) noexcept
{
  m_timed_actions.push_back(std::make_pair(in_game_time, action));
}

action_history create_action_history_from_game(const game& g)
{
  std::vector<action_history> v;
  for (const auto& p: g.get_pieces())
  {
    v.push_back(p.get_action_history());
  }
  return merge_action_histories(v);
}

action_history create_action_history_from_pgn(const pgn_game_string& s)
{
  action_history h;

  const auto move_strs{split_pgn_str(s)};
  const int n_moves = move_strs.size();
  game_controller c;
  for (int i{0}; i!=n_moves; ++i)
  {
    const in_game_time t(i);
    const fen_string fen_str{to_fen_string(c.get_game())};
    const chess_move m(move_strs[i], fen_str);
    const chess_color piece_color{m.get_color()};
    const std::optional<piece_type> pt{m.get_piece_type()};
    const std::optional<piece_action_type> at{m.get_action_type()};
    if (pt.has_value() && at.has_value()) // A win is not a piece action
    {
      const square& from{m.get_from().value()};
      const square& to{m.get_to().value()};
      const piece_action pa(
        piece_color,
        pt.value(),
        at.value(),
        from,
        to
      );
      h.add_action(t, pa);
      get_piece_at(c.get_game(), from).add_action(pa);
    }
    c.tick();
  }
  return h;
}

#ifdef REALLY_NEED_ISSUE_153
void create_r_script_from_pgn(const pgn_game_string& s, const std::string& r_filename)
{
  std::vector<std::string> cmds; // For the R script

  const auto move_strs{split_pgn_str(s)};
  const int n_moves = move_strs.size();
  game_controller c;
  for (int i{0}; i!=n_moves; ++i)
  {
    const in_game_time t(i);
    const fen_string fen_str{to_fen_string(c.get_game())};
    const chess_move m(move_strs[i], fen_str);
    const chess_color piece_color{m.get_color()};
    const std::optional<piece_type> pt{m.get_piece_type()};
    const std::optional<piece_action_type> at{m.get_action_type()};
    if (pt.has_value() && at.has_value()) // A win is not a piece action
    {
      const square& from{m.get_from().value()};
      const square& to{m.get_to().value()};

      /*
      const piece_action pa(
        piece_color,
        pt.value(),
        at.value(),
        from,
        to
      );
      h.add_action(t, pa);
      get_piece_at(c.get_game(), from).add_action(pa);
      */
    }
    c.tick();
  }
  return h;
}
#endif // REALLY_NEED_ISSUE_153


std::vector<piece_action> collect_actions_in_timespan(
  const action_history& history,
  const in_game_time from,
  const in_game_time to
)
{
  assert(from < to);
  std::vector<piece_action> actions;
  for (const auto& p: history.get())
  {
    const in_game_time& t{p.first};
    if (t >= from && t <= to) actions.push_back(p.second);
  }
  return actions;
}

const piece_action& get_last_action(const action_history& history)
{
  assert(has_actions(history));
  return history.get().back().second;
}

int get_n_piece_actions(const action_history& r) noexcept
{
  return r.get().size();
}

bool has_actions(const action_history& history) noexcept
{
  return !history.get().empty();
}

bool has_just_double_moved(
  const action_history& history,
  const in_game_time when
)
{
  // It takes 1 time unit to do a move
  const in_game_time start_earliest{
    when - delta_t(1.0) - delta_t(1.0)};
  const in_game_time start_latest{when - delta_t(1.0)};
  const std::vector<piece_action> actions{
    collect_actions_in_timespan(
      history,
      start_earliest,
      start_latest
    )
  };
  for (const piece_action a: actions)
  {
    if (is_double_move(a)) return true;
  }
  return false;
}

bool is_enpassantable(
  const action_history& action_history,
  const in_game_time when
)
{
  return has_just_double_moved(action_history, when);
}

bool is_piece_selected(const action_history& h)
{
  bool is_selected{false};
  for (const auto& a: h.get())
  {
    if (a.second.get_action_type() == piece_action_type::select)
    {
      assert(!is_selected);
      is_selected = true;
    }
    else if (a.second.get_action_type() == piece_action_type::unselect)
    {
      assert(is_selected);
      is_selected = false;
    }
  }
  return is_selected;
}

action_history merge_action_histories(const std::vector<action_history> histories)
{
  std::vector<std::pair<in_game_time, piece_action>> timed_actions;
  for (const auto& history: histories)
  {
    const auto tas{history.get()};
    std::copy(std::begin(tas), std::end(tas), std::back_inserter(timed_actions));
  }
  std::sort(
    std::begin(timed_actions),
    std::end(timed_actions),
    [](const auto& lhs, const auto& rhs)
    {
      return lhs.first < rhs.first;
    }
  );
  return action_history(timed_actions);
}

void test_action_history()
{
#ifndef NDEBUG
  // upon construction there are timed actions
  {
    const action_history h;
    assert(h.get().empty());
  }
  // create_action_history_from_game
  {
    const action_history r(create_action_history_from_game(game()));
    assert(get_n_piece_actions(r) == 0);
  }
  // create_action_history_from_pgn from empty string is OK
  {
    const action_history r(create_action_history_from_pgn(pgn_game_string("")));
    assert(get_n_piece_actions(r) == 0);
  }
  // create_action_history_from_pgn from one action
  {
    const action_history r(create_action_history_from_pgn(pgn_game_string("1. e4")));
    assert(get_n_piece_actions(r) == 1);
  }
  // create_action_history_from_pgn from two actions
  {
    const action_history r(create_action_history_from_pgn(pgn_game_string("1. e4 e5")));
    assert(get_n_piece_actions(r) == 2);
  }
  // create_action_history_from_pgn from get_scholars_mate_as_pgn_str
  {
    const action_history r(create_action_history_from_pgn(get_scholars_mate_as_pgn_str()));
    const int n_piece_actions{get_n_piece_actions(r)};
    assert(n_piece_actions == 7); // Winning is not a piece action
  }
  //#define FIX_COMPLEX_PGN_20250702
  #ifdef FIX_COMPLEX_PGN_20250702
  // create_action_history_from_pgn from get_replay_1_as_pgn_str
  {
    // It is a complex PGN
    const action_history r(create_action_history_from_pgn(get_replay_1_as_pgn_str()));
    assert(get_n_piece_actions(r) > 8);
  }
  #endif // FIX_COMPLEX_PGN_20250702
  #ifdef REALLY_NEED_ISSUE_153
  // create_r_scrip_from_pgn
  {
    const std::string filename{"tmp_create_r_scrip_from_pgn.R"};
    if (std::filesystem::exists(filename))
    {
      std::filesystem::remove(filename);
    }
    const pgn_game_string s("1. e4 e5 2. Qh5 Nc6 3. Bc4 Nf6 Qxf7# 1-0");
    create_r_scrip_from_pgn(s, "play_scholars_mate.R");
    assert(std::filesystem::exists(filename));
  }
  #endif // REALLY_NEED_ISSUE_153

  // merge_action_histories
  {
    const action_history a{
      {
        std::make_pair(in_game_time(0.1), get_test_piece_action()),
        std::make_pair(in_game_time(0.3), get_test_piece_action())
      }
    };

    const action_history b{
      {
        std::make_pair(in_game_time(0.2), get_test_piece_action()),
        std::make_pair(in_game_time(0.4), get_test_piece_action())
      }
    };
    const auto c{merge_action_histories( { a, b } ) };
    assert(
      a.get().size() + b.get().size()
      == c.get().size()
    );
  }
  // operator==
  {
    const action_history a(create_action_history_from_pgn(pgn_game_string("1. e4")));
    const action_history b(create_action_history_from_pgn(pgn_game_string("1. e4")));
    const action_history c(create_action_history_from_pgn(pgn_game_string("1. e4 e5")));
    assert(a == b);
    assert(!(a == c));
  }
  // operator<<
  {
    std::stringstream s;
    action_history h;
    s << h; // Still empty
    assert(s.str().empty());
    h.add_action(
      in_game_time(1.0),
      get_test_piece_action()
    );
    s << h;
    assert(!s.str().empty());
  }
#endif // NDEBUG
}

std::string to_str(const action_history& history) noexcept
{
  std::stringstream s;
  for (const auto& timed_action: history.get())
  {
    s << to_human_str(timed_action.first) << ": " << timed_action.second << '\n';
  }
  std::string t{s.str()};
  if (!t.empty()) t.pop_back();
  return t;
}

bool operator==(const action_history& lhs, const action_history& rhs) noexcept
{
  return lhs.get() == rhs.get();
}

std::ostream& operator<<(std::ostream& os, const action_history& history) noexcept
{
  os << to_str(history);
  return os;
}
