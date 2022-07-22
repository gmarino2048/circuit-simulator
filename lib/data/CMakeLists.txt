
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
        ${CMAKE_CURRENT_LIST_DIR}/src/InternalDatabase.cpp

        ${CMAKE_CURRENT_LIST_DIR}/src/objects/TransistorDatabaseObject.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/objects/WireDatabaseObject.cpp
)

target_link_libraries(circsim-data
    PUBLIC
        circsim-common
        circsim-components
)