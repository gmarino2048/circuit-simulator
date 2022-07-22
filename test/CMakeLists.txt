

file(
    GLOB components-test_SRC "components/*.cpp"
)


file(
    GLOB data-test_SRC 
    "data/*.cpp"
    "data/objects/*.cpp"
)

add_executable(data-test
    ${data-test_SRC}
)

target_link_libraries(data-test
    circsim-components
    circsim-data
    testing-common
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
    testing-common
    GTest::GTest
    GTest::Main
)

gtest_add_tests(
    TARGET sim-test
)