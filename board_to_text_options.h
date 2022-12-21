#ifndef BOARD_TO_TEXT_OPTIONS_H
#define BOARD_TO_TEXT_OPTIONS_H

class board_to_text_options
{
public:
  explicit board_to_text_options(
    const bool shows_coordinats = false,
    const bool shows_legend = false
  ) noexcept;

  bool get_show_coordinats() const noexcept { return m_show_coordinats; }
  bool get_show_legend() const noexcept { return m_show_legend; }

  void set_show_coordinats(const bool show_coordinats) noexcept
  {
    m_show_coordinats = show_coordinats;
  }
  void set_show_legend(const bool show_legend) noexcept
  {
    m_show_legend = show_legend;
  }

private:

  bool m_show_coordinats;
  bool m_show_legend;
};


/// Test this class and its free functions
void test_board_to_text_options();

#endif // BOARD_TO_TEXT_OPTIONS_H
