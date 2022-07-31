#ifndef REPLAY_H
#define REPLAY_H

#include <string>

/// A collection of chess moves
class replay
{
public:
  replay();
};

std::string get_replay_str_1() noexcept;

replay get_replay_1() noexcept;

/// Test this class and its free functions
void test_replay();


#endif // REPLAY_H
