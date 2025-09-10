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

<<<<<<< HEAD
# Qt5
# QT += core

=======
>>>>>>> upstream/main
# SFML
win32 {
  # Include path
  INCLUDEPATH += $$PWD/SFML-2.6.1/include

  # Library path
  LIBS += -L$$PWD/SFML-2.6.1/lib

  # Link SFML modules
  LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
}



# SFGraphing
DEFINES += USE_SFGRAPHING

# Expand all .cpp and .h files explicitly
SFGRAPHING_SRCS = $$files($$PWD/SFGraphing/src/*.cpp)
SOURCES += $$SFGRAPHING_SRCS

SFGRAPHING_HDRS = $$files($$PWD/SFGraphing/include/SFGraphing/*.h)
HEADERS += $$SFGRAPHING_HDRS

# Add include directory
INCLUDEPATH += $$PWD/SFGraphing/include

# Add chess-library include path
INCLUDEPATH += $$PWD/chess-library/include

# Debug output (will show when running qmake)
message("SFGraphing sources: $$SFGRAPHING_SRCS")
message("SFGraphing headers: $$SFGRAPHING_HDRS")
