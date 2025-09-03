# This is the general project file,
# to be used to simply run the game.
#
# I will leave it as is, so that Sajid can do his job :-)
#
# Other .pro files are used for specific tasks,
# such as codecov or profiling

# On GHA, this DEFINE is added by the .yaml script
#
#DEFINES += LOGIC_ONLY

CONFIG += static

# All files are in here, the rest are just settings
include(game.pri)
include(game_view.pri)

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

  # gprof
  QMAKE_CXXFLAGS += -pg
  QMAKE_LFLAGS += -pg
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

# SFML
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

win32 {
  LIBS += $$PWD/../SFML/lib
  INCLUDEPATH += $$PWD/../SFML/include
}

# SFGraphing
DEFINES += USE_SFGRAPHING
SOURCES += $$PWD/../SFGraphing/src/*.cpp
INCLUDEPATH += $$PWD/../SFGraphing/include
HEADERS += $$PWD/../SFGraphing/include/SFGraphing

