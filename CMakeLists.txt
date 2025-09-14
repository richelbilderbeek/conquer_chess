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

# Initialize source file lists
set(EXTERNAL_SRC "")
set(EXTERNAL_INCLUDES "")

# Handle magic_enum (header-only)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/magic_enum/include")
    message(STATUS "Found magic_enum - adding include path")
    list(APPEND EXTERNAL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/magic_enum/include")
else()
    message(WARNING "magic_enum not found - enum reflection features may be disabled")
endif()

# Handle chess-library (source files)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/chess-library")
    message(STATUS "Found chess-library - searching for source files")
    
    # Look for source files in multiple possible locations
    file(GLOB_RECURSE CHESS_LIBRARY_SRC 
        "${CMAKE_CURRENT_SOURCE_DIR}/chess-library/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/chess-library/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/chess-library/source/*.cpp"
    )
    
    if(CHESS_LIBRARY_SRC)
        list(APPEND EXTERNAL_SRC ${CHESS_LIBRARY_SRC})
        list(LENGTH CHESS_LIBRARY_SRC CHESS_LIB_COUNT)
        message(STATUS "Found ${CHESS_LIB_COUNT} chess-library source files")
    endif()
    
    # Add include directories
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/chess-library/include")
        list(APPEND EXTERNAL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/chess-library/include")
    endif()
    # Also add root directory in case headers are there
    list(APPEND EXTERNAL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/chess-library")
else()
    message(WARNING "chess-library not found")
endif()

# Handle SFGraphing (source files) 
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SFGraphing")
    message(STATUS "Found SFGraphing - searching for source files")
    
    # Look for source files in multiple possible locations
    file(GLOB_RECURSE SFGRAPHING_SRC 
        "${CMAKE_CURRENT_SOURCE_DIR}/SFGraphing/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/SFGraphing/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/SFGraphing/source/*.cpp"
    )
    
    if(SFGRAPHING_SRC)
        list(APPEND EXTERNAL_SRC ${SFGRAPHING_SRC})
        list(LENGTH SFGRAPHING_SRC SFGRAPHING_COUNT)
        message(STATUS "Found ${SFGRAPHING_COUNT} SFGraphing source files")
    endif()
    
    # Add include directories
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SFGraphing/include")
        list(APPEND EXTERNAL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/SFGraphing/include")
    endif()
    # Also add root directory in case headers are there
    list(APPEND EXTERNAL_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/SFGraphing")
else()
    message(WARNING "SFGraphing not found")
endif()

# Get local source files
file(GLOB_RECURSE LOCAL_SRC "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Debug output
message(STATUS "Local source files: ${LOCAL_SRC}")
message(STATUS "External source files: ${EXTERNAL_SRC}")
message(STATUS "External includes: ${EXTERNAL_INCLUDES}")

# Create executable with all source files
add_executable(conquer_chess WIN32 MACOSX_BUNDLE
    ${LOCAL_SRC}
    ${EXTERNAL_SRC}
)

# Add include directories
target_include_directories(conquer_chess PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${EXTERNAL_INCLUDES}
)


target_compile_definitions(conquer_chess PRIVATE
    USE_SFGRAPHING
)
# Tell CMake to use SFML from vcpkg
# Version can be found at https://vcpkg.link/ports/sfml/versions
find_package(SFML 2.6.1 COMPONENTS graphics audio network window system main REQUIRED)

# Link libraries
target_link_libraries(conquer_chess PRIVATE
    SFML::System
    SFML::Window  
    SFML::Graphics
    SFML::Network
    SFML::Audio
)

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
