
include_directories(common)

file(
    GLOB components-test_SRC "components/*.cpp"
)

add_executable(components-test
    ${components-test_SRC}
)

target_link_libraries(components-test
    circsim-components
    GTest::GTest
    GTest::Main
)

gtest_add_tests(
    TARGET components-test
)


file(
    GLOB data-test_SRC "data/*.cpp"
)

add_executable(data-test
    ${data-test_SRC}
)

target_link_libraries(data-test
    circsim-components
    circsim-data
    GTest::GTest
    GTest::Main
)

gtest_add_tests(
    TARGET data-test
)


file(
    GLOB sim-test_SRC "sim/*.cpp"
)

add_executable(sim-test
    ${sim-test_SRC}
)

target_link_libraries(sim-test
    circsim-components
    circsim-data
    circsim-sim
    GTest::GTest
    GTest::Main
)

gtest_add_tests(
    TARGET sim-test
)