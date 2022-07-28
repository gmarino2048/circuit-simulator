
# Add main components library

add_library(circsim-components SHARED)

target_include_directories(circsim-components
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_sources(circsim-components
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/Transistor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Wire.cpp
)

target_link_libraries(circsim-components
    PUBLIC
        circsim-common
)

# Add test executable for components

add_executable(circsim-components-test)

target_sources(circsim-components-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/test/Transistor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test/Wire.cpp
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
