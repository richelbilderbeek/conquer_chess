#ifndef ACTION_NUMBER_H
#define ACTION_NUMBER_H

/// The index/number of the action,
/// using human counting.
/// As there are four actions, an action_number
/// can be 1, 2, 3 or 4.
class action_number
{
public:
  action_number(const int number);

  int get_number() const noexcept { return m_number; }

private:

  int m_number;
};

/// Get the next action_number,
/// e.g. when having 2, this will be 3.
/// For 4, this wil return 1
action_number get_next(const action_number& n) noexcept;

/// Get the previous action_number,
/// e.g. when having 2, this will be 1.
/// For 1, this wil return 4
action_number get_previous(const action_number& n) noexcept;

/// Test this class and its members
void test_action_number();

#endif // ACTION_NUMBER_H
