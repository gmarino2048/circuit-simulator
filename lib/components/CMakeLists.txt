
# Add main components library

add_library(circsim-components SHARED)

target_include_directories(circsim-components
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_sources(circsim-components
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Circuit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/CircuitState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Register.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Transistor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Wire.cpp
)

target_link_libraries(circsim-components
    PUBLIC
        circsim-common
)

# Add test executable for components

add_executable(circsim-components-test)

target_sources(circsim-components-test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/Circuit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/Register.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/Transistor.cpp
)

target_link_libraries(circsim-components-test
    PRIVATE
        GTest::Main
        GTest::GTest
        circsim-components
)

gtest_add_tests(
    TARGET circsim-components-test
)
