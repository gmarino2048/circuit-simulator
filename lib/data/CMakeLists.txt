
# Add main library

add_library(circsim-data SHARED)

target_include_directories(circsim-data
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_sources(circsim-data
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/ExternalStorage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/InternalStorage.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/ExternalStorage/Transistor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ExternalStorage/Wire.cpp
)

target_link_libraries(circsim-data
    PUBLIC
        sqlite3

        circsim-common
        circsim-components
)

# Add test file

add_executable(circsim-data-test)

target_sources(circsim-data-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/test/ExternalStorage.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test/InternalStorage.cpp
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
