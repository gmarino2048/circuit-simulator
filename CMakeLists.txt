cmake_minimum_required(VERSION 3.16)
project(circuit-simulator VERSION 0.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

include_directories("inc")
add_subdirectory("src")


add_executable(circsim
    src/main.cpp
)
