
# Add full adder test
add_executable(full-adder-test)

target_sources(full-adder-test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/full-adder/full-adder.cpp
)

target_link_libraries(full-adder-test
    PRIVATE
        circsim-common
        circsim-components
        circsim-data
        circsim-sim

        GTest::Main
        GTest::GTest
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/full-adder/full-adder.json
    full-adder.json
    COPYONLY
)

gtest_add_tests(
    TARGET full-adder-test
)
