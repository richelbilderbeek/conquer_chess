# This is the general project file,
# to be used to simply run the game.
#
# Other .pro files are used for specific tasks,
# such as codecov or profiling

# On GHA, this DEFINE is added by the .yaml script
#
#DEFINES += LOGIC_ONLY

CONFIG += static

# All files are in here, the rest are just settings
include(game.pri)

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

###############################################################################
# SFML
###############################################################################
win32 {
  # Include path
  INCLUDEPATH += $$PWD/SFML-2.6.1/include

  # Library path
  LIBS += -L$$PWD/SFML-2.6.1/lib

  # Link SFML modules
  LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
}

unix {
  # Link SFML modules
  LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
}

###############################################################################
# magic_enum
###############################################################################
unix {
  INCLUDEPATH += $$PWD/../magic_enum/include
}

###############################################################################
# SFGraphing
###############################################################################
DEFINES += USE_SFGRAPHING

win32 {
  SFGRAPHING_SRCS = $$files($$PWD/SFGraphing/src/*.cpp)
  SOURCES += $$SFGRAPHING_SRCS

  SFGRAPHING_HDRS = $$files($$PWD/SFGraphing/include/SFGraphing/*.h)
  HEADERS += $$SFGRAPHING_HDRS

  INCLUDEPATH += $$PWD/SFGraphing/include

  message("SFGraphing sources: $$SFGRAPHING_SRCS")
  message("SFGraphing headers: $$SFGRAPHING_HDRS")
}
unix {
  SFGRAPHING_SRCS = $$files($$PWD/../SFGraphing/src/*.cpp)
  SOURCES += $$SFGRAPHING_SRCS

  SFGRAPHING_HDRS = $$files($$PWD/../SFGraphing/include/SFGraphing/*.h)
  HEADERS += $$SFGRAPHING_HDRS

  INCLUDEPATH += $$PWD/../SFGraphing/include
}


###############################################################################
# chess-library
###############################################################################
win32 {
  INCLUDEPATH += $$PWD/chess-library/include
}
unix {
  INCLUDEPATH += $$PWD/../chess-library/include
}

