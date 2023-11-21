# This is the general project file,
# to be used to simply run the game.
#
# Other .pro files are used for specific tasks,
# such as codecov or profiling

# On GHA, this DEFINE is added in the .yaml script
#
#DEFINES += LOGIC_ONLY

# All files are in here, the rest are just settings
include(game.pri)
include(game_view.pri)
include(test_game.pri)

TARGET = conquer_chess

# Use the C++ version that all team members can use
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

# High warning levels
QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

# Debug and release settings
CONFIG += debug_and_release
CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}
CONFIG(debug, debug|release) {
  # High warning levels
  QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

  # A warning is an error
  QMAKE_CXXFLAGS += -Werror

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov
}

# Qt5
QT += core gui widgets

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# magic_enum, from https://github.com/Neargye/magic_enum
#
# Must allow for:
#
#   include <magic_enum/magic_enum.hpp>
#
INCLUDEPATH += ../magic_enum/include
