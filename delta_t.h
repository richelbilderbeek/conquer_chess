#ifndef DELTA_T_H
#define DELTA_T_H

#include <string>

/// The fraction of a full move that is done per tick,
/// where
///   * 0.01 means that only 1% of the full move is done
///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
class delta_t
{
public:
  delta_t(const double dt);

  double get() const noexcept { return m_delta_t; }

private:

  double m_delta_t;
};

/// Get the default delta_t,
/// which is the fraction of a full move that is done per tick,
/// where
///   * 0.01 means that only 1% of the full move is done
///   * 1.0 denotes a full move, i.e. a piece traverses 1.0 game coordinat
double get_default_delta_t();

/// Test this class and its free functions
void test_delta_t();

/// Describe the delta_t to human
std::string to_human_str(const delta_t& dt) noexcept;

#endif // DELTA_T_H
