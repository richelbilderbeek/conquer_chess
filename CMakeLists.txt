cmake_minimum_required(VERSION 3.18)
project(conquer-chess VERSION 0.13.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure vcpkg toolchain is used if available
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ---- Dependencies ----

include(FetchContent)

# magic_enum (installed via vcpkg)
#find_path(MAGIC_ENUM_INCLUDE_DIRS "magic_enum.hpp")
#message(STATUS "magic_enum include dir: ${MAGIC_ENUM_INCLUDE_DIRS}")


# SFML (installed via vcpkg)
find_package(SFML 2.6 COMPONENTS graphics audio network window system main REQUIRED)

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

# Collect your project source files
file(GLOB LOCAL_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(conquer_chess WIN32 MACOSX_BUNDLE
    ${LOCAL_SRC}
)

# ---- Include paths ----
# If SFGraphing and chess_library don't export proper targets,
# we add includes manually as fallback:
target_include_directories(conquer_chess PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${MAGIC_ENUM_INCLUDE_DIRS}
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
    magic_enum::magic_enum
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

# Add .qrc files
# Copy resources (instead of Qt .qrc system)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Fixed compile options with proper scope
if(CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(conquer_chess PRIVATE
        NDEBUG
    )
endif()

# Fixed compile options with proper scope
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

