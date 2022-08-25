
add_library(circsim-test SHARED)

target_include_directories(circsim-test
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_sources(circsim-test
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Gates.cpp
)

target_link_libraries(circsim-test
    PUBLIC
        circsim-components

    PRIVATE
        Boost::json

        circsim-data
)