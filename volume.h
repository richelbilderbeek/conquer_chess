#ifndef VOLUME_H
#define VOLUME_H

#include <iosfwd>

/// A relative sound intensity, from 0 to 100%
class volume
{
public:
  /// Volume in percents,
  /// i.e. 0 is 0%, 10 is 10%, 100 = 100%
  explicit volume(const int volume_in_percent);

  /// Get the volume in percents,
  /// i.e. 0 is 0%, 10 is 10%, 100 = 100%
  int get_percentage() const noexcept { return m_volume_in_percent; }

private:

  /// Volume in percents,
  /// i.e. 0 is 0%, 10 is 10%, 100 = 100%
  int m_volume_in_percent;
};

/// Get the next volume, i.e. the volume when the user presses right
volume get_next(const volume& v) noexcept;

/// Test this class and its free functions
void test_volume();

bool operator==(const volume& lhs, const volume& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const volume& v) noexcept;

#endif // VOLUME_H
