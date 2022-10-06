cmake_minimum_required(VERSION 3.16)
project(circuit-simulator VERSION 0.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

enable_testing()

include(GoogleTest)
find_package(GTest REQUIRED)

find_package(Boost 1.75 REQUIRED COMPONENTS json)
find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(${SQLite3_INCLUDE_DIRS})

# Add component directories
add_subdirectory(lib/common)
add_subdirectory(lib/components)
add_subdirectory(lib/data)
add_subdirectory(lib/sim)
add_subdirectory(lib/test)

# Add integration tests
add_subdirectory(test)
