
# CMake project file.
# Set everything up before commencing with any builds.

cmake_minimum_required(VERSION 3.5)

project(mutex_example)

message(STATUS "Project:    ${PROJECT_NAME}")
message(STATUS "Platform:   ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM})")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# Set the location where our binaries will be stored.
# WARN/TODO: Not quite right, because .lib or .a files should
#            not go in the bin folder!
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Adds 'd' onto artifacts - does NOT apply to executables.
# For executables, this needs to be done an exec-by-exec
# basis.
set(CMAKE_DEBUG_POSTFIX "d")

# Install TPSDKs
include(TPSDKs.cmake)


add_executable(process_a main_process_a.cpp
    clock_wait.hpp
    cv_utils.cpp
    cv_utils.hpp
    mutex_utils.cpp
    mutex_utils.hpp)

target_link_libraries(process_a
    pthread
    rt
    )


add_executable(process_b main_process_b.cpp
    clock_wait.hpp
    cv_utils.cpp
    cv_utils.hpp
    mutex_utils.cpp
    mutex_utils.hpp)

target_link_libraries(process_b
    pthread
    rt
    )