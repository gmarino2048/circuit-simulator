
# Add full adder test
add_executable(full-adder-test)

target_sources(full-adder-test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/full-adder/full-adder.cpp
)

target_link_libraries(full-adder-test
    PRIVATE
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

# Add flip-flop test
add_executable(flip-flop-test)

target_sources(flip-flop-test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/flip-flop/flip-flop.cpp
)

target_link_libraries(flip-flop-test
    PRIVATE
        circsim-sim

        GTest::Main
        GTest::GTest
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/flip-flop/flip-flop.json
    flip-flop.json
    COPYONLY
)

gtest_add_tests(
    TARGET flip-flop-test
)
