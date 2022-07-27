#ifndef LOG_H
#define LOG_H

#include <vector>

#include <SFML/System.hpp>

#include "delta_t.h"
#include "message.h"

/// The text log in the game
class log
{
public:
  /// @param display_time_secs the time a message will be displayed
  explicit log(const double display_time_secs);

  /// Add a message, timestamp will be added
  void add_message(const message& m) noexcept;

  /// Get the last messages that were emitted at most 'max_elapsed_time_secs'
  /// seconds ago, as a (possibly) multi-line string,
  /// for a specific color
  std::string get_last_messages(const chess_color color) const;

  /// Update, so old messages are removed
  void tick();

private:
  using elapsed_time_secs = float;

  /// the timed messages
  std::vector<std::pair<elapsed_time_secs, message>> m_timed_messages;

  /// the clock
  sf::Clock m_clock;

  /// the time a message will be displayed, in seconds
  double m_display_time_secs;
};

/// Get the log messages for a specific color
std::string get_last_log_messages(
  const log& l,
  const chess_color color
) noexcept;

#endif // LOG_H
