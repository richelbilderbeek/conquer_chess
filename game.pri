# Game core headers
GAME_HDRS = $$files($$PWD/include/*.h)
HEADERS += $$GAME_HDRS
INCLUDEPATH += $$PWD/include

# Game core sources
GAME_SRCS = $$files($$PWD/src/*.cpp)
SOURCES += $$GAME_SRCS

# Debug output
message("Game sources: $$GAME_SRCS")
message("Game headers: $$GAME_HDRS")
