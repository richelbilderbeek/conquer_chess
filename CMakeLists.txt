cmake_minimum_required(VERSION 3.18)
project(conquer-chess VERSION 0.13.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/share/sfml")

# Static build settings (only for non-MSVC)
if(NOT MSVC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

# Set vcpkg toolchain if not set
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ---- Dependencies ----
include(FetchContent)

# SFML (via vcpkg)
find_package(SFML 2.6.2 COMPONENTS graphics audio network window system REQUIRED)

# SFGraphing (from GitHub)
FetchContent_Declare(
  SFGraphing
  GIT_REPOSITORY https://github.com/richelbilderbeek/SFGraphing.git
  GIT_TAG main
)
FetchContent_MakeAvailable(SFGraphing)

# chess-library (from GitHub)
FetchContent_Declare(
  chess_library
  GIT_REPOSITORY https://github.com/richelbilderbeek/chess-library.git
  GIT_TAG main
)
FetchContent_MakeAvailable(chess_library)

# ---- Sources ----
file(GLOB LOCAL_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(conquer_chess WIN32 MACOSX_BUNDLE
    ${LOCAL_SRC}
)

# ---- Include paths ----
# Fallback include dirs in case targets don’t export properly
target_include_directories(conquer_chess PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SFGraphing_SOURCE_DIR}/include
    ${chess_library_SOURCE_DIR}/include
)


target_compile_definitions(conquer_chess PRIVATE
    USE_SFGRAPHING
)
# Tell CMake to use SFML from vcpkg
# Version can be found at https://vcpkg.link/ports/sfml/versions
find_package(SFML 2.6.1 COMPONENTS graphics audio network window system main REQUIRED)

# Link against SFML targets (vcpkg uses lowercase names)
target_link_libraries(conquer_chess PRIVATE
    SFGraphing
    chess_library
    sfml-system
    sfml-window
    sfml-graphics
    sfml-network
    sfml-audio
    sfml-main
)

if(MSVC)
    add_compile_options(/bigobj)
else()
    add_compile_options(-Wa,-mbig-obj)
endif()

# Copy resources instead of using Qt .qrc
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Compile definitions for release/debug
if(CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(conquer_chess PRIVATE NDEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    target_link_libraries(conquer_chess PRIVATE gcov)
    target_compile_options(conquer_chess PRIVATE
        -Wall
        -Wextra
        -Wshadow
        -fprofile-arcs
        -ftest-coverage
    )
endif()

install(TARGETS conquer_chess
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

