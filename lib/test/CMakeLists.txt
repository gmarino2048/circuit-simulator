
add_library(circsim-test SHARED)

target_include_directories(circsim-test
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_sources(circsim-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/Gates.cpp
)

target_link_libraries(circsim-test
    PUBLIC
        circsim-data
    PRIVATE
        circsim-components
)