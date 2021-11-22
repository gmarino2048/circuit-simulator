
file(
    GLOB components-test_SRC "components/*.cpp"
)

add_executable(components-test.unit
    ${components-test_SRC}
)

target_link_libraries(components-test.unit
    circsim-components
    GTest::GTest
    GTest::Main
)

file(
    GLOB data-test_SRC "data/*.cpp"
)

add_executable(data-test.unit
    ${data-test_SRC}
)

target_link_libraries(data-test.unit
    circsim-data
    GTest::GTest
    GTest::Main
)

gtest_add_tests(
    TARGET data-test
)