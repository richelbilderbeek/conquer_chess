cmake_minimum_required(VERSION 3.18)
project(conquer-chess VERSION 0.13.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Static build settings (only for non-MSVC)
if(NOT MSVC)
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_EXE_LINKER_FLAGS "-static")
endif()

include(FetchContent)

# magic_enum (header-only)
FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.8.0
)
FetchContent_MakeAvailable(magic_enum)

# chess-library
FetchContent_Declare(
    chess_library
    GIT_REPOSITORY https://github.com/richelbilderbeek/chess-library.git
    GIT_TAG master
)
FetchContent_MakeAvailable(chess_library)

# Set vcpkg toolchain if not set
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find and link SFML (vcpkg)
find_package(SFML 2.6 COMPONENTS system window graphics audio network REQUIRED)

# Get local and external source files
file(GLOB_RECURSE LOCAL_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE CHESS_SRC "${chess_library_SOURCE_DIR}/src/*.cpp")

# Debug output
message(STATUS "Local source files: ${LOCAL_SRC}")
message(STATUS "External source files: ${CHESS_SRC}")

# Create executable
add_executable(conquer_chess WIN32 MACOSX_BUNDLE
    ${LOCAL_SRC}
    ${CHESS_SRC}
)

# Include directories
target_include_directories(conquer_chess PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${magic_enum_SOURCE_DIR}/include
    ${chess_library_SOURCE_DIR}/include
)


target_compile_definitions(conquer_chess PRIVATE
    USE_SFGRAPHING
)
# Tell CMake to use SFML from vcpkg
# Version can be found at https://vcpkg.link/ports/sfml/versions
find_package(SFML 2.6.1 COMPONENTS graphics audio network window system main REQUIRED)

target_link_libraries(conquer_chess PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)


if(MSVC)
    add_compile_options(/bigobj)
else()
    add_compile_options(-Wa,-mbig-obj)
endif()

# Copy resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Build type specific settings
if(CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(conquer_chess PRIVATE NDEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)
    if(NOT MSVC)
        target_link_libraries(conquer_chess PRIVATE gcov)
        target_compile_options(conquer_chess PRIVATE
            -Wall -Wextra -Wshadow
            -fprofile-arcs -ftest-coverage
        )
    endif()
endif()

# Install
install(TARGETS conquer_chess
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
