#ifndef REPLAYER_H
#define REPLAYER_H

#include "ccfwd.h"
#include "delta_t.h"
#include "replay.h"

#include <iosfwd>

/// Player that replays a match
class replayer
{
public:
  explicit replayer(const replay& r = replay(""));

  /// Do a move or do nothing
  void do_move(
    game_controller& c,
    const game& g
  );

  /// Get the last time a move was done
  const auto& get_last_time() const noexcept { return m_last_time; }

  /// Get the play that is replayed
  const auto& get_replay() const noexcept { return m_replay; }

private:

  /// The last time a move was done
  delta_t m_last_time;

  /// A replay
  replay m_replay;
};

/// Get the number of moves in the replay
int get_n_moves(const replayer& r) noexcept;

/// Test this class and its free functions
void test_replayer();

bool operator==(const replayer& lhs, const replayer& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const replayer& r) noexcept;

#endif // REPLAYER_H
