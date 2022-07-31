#ifndef ID_H
#define ID_H

#include <iosfwd>

/// An ID, each one being unique
class id
{
public:
  int get() const noexcept { return m_value; }

private:
  id();

  static int sm_next_value;

  int m_value;

  friend id create_new_id() noexcept;
};

id create_new_id() noexcept;

/// Test this class and its free functions
void test_id();

bool operator==(const id& lhs, const id& rhs) noexcept;
bool operator!=(const id& lhs, const id& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const id& i) noexcept;

#endif // ID_H
