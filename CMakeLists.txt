cmake_minimum_required(VERSION 3.15)
project(NuclearWars LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(NuclearWars Nuclear.cpp)

# Treat warnings as errors in debug builds
target_compile_options(NuclearWars PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)
