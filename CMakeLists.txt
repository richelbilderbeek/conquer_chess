cmake_minimum_required(VERSION 3.18)
project(conquer-chess VERSION 0.13.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -------------------------------------------------------------------
# Dependencies via FetchContent
# -------------------------------------------------------------------
include(FetchContent)

# magic_enum (header-only)
FetchContent_Declare(
  magic_enum
  GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
  GIT_TAG        v0.9.5 # adjust version if needed
)
FetchContent_MakeAvailable(magic_enum)

# chess-library
FetchContent_Declare(
  chess-library
  GIT_REPOSITORY https://github.com/richelbilderbeek/chess-library.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(chess-library)

# SFGraphing
FetchContent_Declare(
  SFGraphing
  GIT_REPOSITORY https://github.com/sairaatta/SFGraphing.git
  GIT_TAG master
)
FetchContent_MakeAvailable(SFGraphing)


# -------------------------------------------------------------------
# vcpkg toolchain (for SFML)
# -------------------------------------------------------------------
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
endif()

# -------------------------------------------------------------------
# Source files
# -------------------------------------------------------------------
file(GLOB LOCAL_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(conquer_chess WIN32 MACOSX_BUNDLE
    ${LOCAL_SRC}
)

# -------------------------------------------------------------------
# Includes
# -------------------------------------------------------------------
target_include_directories(conquer_chess PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${magic_enum_SOURCE_DIR}/include
    ${chess-library_SOURCE_DIR}/include
    ${SFGraphing_SOURCE_DIR}/include
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

# Resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Install target
install(TARGETS conquer_chess
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
