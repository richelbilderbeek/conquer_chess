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

# Set vcpkg toolchain if not set
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Add external dependencies
add_subdirectory(magic_enum EXCLUDE_FROM_ALL)
add_subdirectory(chess-library)
add_subdirectory(SFGraphing)

# Get SFGraphing source files
file(GLOB_RECURSE SFGRAPHING_SRC "${CMAKE_CURRENT_SOURCE_DIR}/SFGraphing/src/*.cpp")
file(GLOB_RECURSE CHESS_LIBRARY_SRC "${CMAKE_CURRENT_SOURCE_DIR}/chess-library/src/*.cpp")

# Debug: Print found external library files
message(STATUS "Found SFGraphing files: ${SFGRAPHING_SRC}")
message(STATUS "Found chess-library files: ${CHESS_LIBRARY_SRC}")

# Get local source files
file(GLOB LOCAL_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(conquer_chess WIN32 MACOSX_BUNDLE
    ${LOCAL_SRC}
    ${SFGRAPHING_SRC}
    ${CHESS_LIBRARY_SRC}
)

target_include_directories(conquer_chess PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum/include
    ${CMAKE_CURRENT_SOURCE_DIR}/chess-library/include
    ${CMAKE_CURRENT_SOURCE_DIR}/SFGraphing/include
)


target_compile_definitions(conquer_chess PRIVATE
    USE_SFGRAPHING
)
# Tell CMake to use SFML from vcpkg
# Version can be found at https://vcpkg.link/ports/sfml/versions
find_package(SFML 2.6.1 COMPONENTS graphics audio network window system main REQUIRED)

# Link against SFML targets (vcpkg uses lowercase names)
target_link_libraries(conquer_chess PRIVATE
    sfml-system
    sfml-window
    sfml-graphics
    sfml-network
    sfml-audio
    magic_enum::magic_enum
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

