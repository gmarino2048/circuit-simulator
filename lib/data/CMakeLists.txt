
# Add main library

add_library(circsim-data SHARED)

target_include_directories(circsim-data
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_sources(circsim-data
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/DatabaseObject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/ExternalDatabase.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/InternalStorage.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/objects/TransistorDatabaseObject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/objects/WireDatabaseObject.cpp
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
        ${CMAKE_CURRENT_LIST_DIR}/test/DatabaseObject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test/InternalStorage.cpp

        ${CMAKE_CURRENT_LIST_DIR}/test/objects/TransistorDatabaseObject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/test/objects/WireDatabaseObject.cpp
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
