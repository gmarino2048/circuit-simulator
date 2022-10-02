
# Add main library

add_library(circsim-data SHARED)

target_include_directories(circsim-data
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_sources(circsim-data
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ExternalStorage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/JsonParser.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/ExternalStorage/CircuitState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ExternalStorage/Register.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ExternalStorage/Transistor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ExternalStorage/Wire.cpp
)

target_link_libraries(circsim-data
    PUBLIC
        Boost::json
        sqlite3

        circsim-common
        circsim-components
)

# Add test file

add_executable(circsim-data-test)

target_sources(circsim-data-test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ExternalStorage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/JsonParser.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/test/ExternalStorage/CircuitState.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ExternalStorage/Register.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ExternalStorage/Transistor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/test/ExternalStorage/Wire.cpp
)

target_link_libraries(circsim-data-test
    PRIVATE
        GTest::Main
        GTest::GTest

        circsim-data
)

gtest_add_tests(
    TARGET circsim-data-test
)
