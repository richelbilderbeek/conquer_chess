#include "game.h"

#include <cassert>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <random>

game::game()
  : m_pieces{get_starting_pieces()}
{

}

void game::add_action(const action a)
{
  // These will be processed in 'tick'
  m_actions.push_back(a);
}

void game::tick()
{

}

void test_game() //!OCLINT tests may be many
{
#ifndef NDEBUG // no tests in release
#endif // no tests in release
}


