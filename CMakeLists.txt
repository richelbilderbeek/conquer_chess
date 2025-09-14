cmake_minimum_required(VERSION 3.18)
project(conquer-chess VERSION 0.13.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set vcpkg toolchain if not set
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Clone and include external dependencies
include(FetchContent)

# magic_enum - use a specific tag that exists
FetchContent_Declare(
    magic_enum
    GIT_REPOSITORY https://github.com/Neargye/magic_enum.git
    GIT_TAG v0.9.5
)

# chess-library - use default branch
FetchContent_Declare(
    chess-library
    GIT_REPOSITORY https://github.com/richelbilderbeek/chess-library.git
)

# SFGraphing - use default branch  
FetchContent_Declare(
    SFGraphing
    GIT_REPOSITORY https://github.com/richelbilderbeek/SFGraphing.git
)

# Populate all dependencies
FetchContent_MakeAvailable(magic_enum chess-library SFGraphing)

# Get local source files
file(GLOB LOCAL_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Get SFGraphing source files (set this AFTER FetchContent_MakeAvailable)
set(SFGRAPHING_SRC
    ${SFGraphing_SOURCE_DIR}/src/PieChartDataSet.cpp
    ${SFGraphing_SOURCE_DIR}/src/PlotDataSet.cpp
    ${SFGraphing_SOURCE_DIR}/src/SFPieChart.cpp
    ${SFGraphing_SOURCE_DIR}/src/SFPlot.cpp
)

# Debug: Print paths to verify they're correct
message(STATUS "SFGraphing source directory: ${SFGraphing_SOURCE_DIR}")
message(STATUS "SFGraphing source files: ${SFGRAPHING_SRC}")
message(STATUS "Magic enum source directory: ${magic_enum_SOURCE_DIR}")
message(STATUS "Chess library source directory: ${chess-library_SOURCE_DIR}")

add_executable(conquer_chess WIN32 MACOSX_BUNDLE
    ${SFGRAPHING_SRC}
    ${LOCAL_SRC}
)

target_include_directories(conquer_chess PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${SFGraphing_SOURCE_DIR}/include
    ${magic_enum_SOURCE_DIR}/include
    ${chess-library_SOURCE_DIR}/include
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

# Fixed compile options with proper scope
if(CMAKE_BUILD_TYPE STREQUAL Release)
    target_compile_definitions(conquer_chess PRIVATE
        NDEBUG
    )
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