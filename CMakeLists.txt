cmake_minimum_required(VERSION 3.16)
project(circuit-simulator VERSION 0.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

include(GoogleTest)
find_package(GTest REQUIRED)

find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(${SQLite3_INCLUDE_DIRS})

include_directories(inc)
add_subdirectory(lib)

add_executable(circsim
    lib/main.cpp
)
