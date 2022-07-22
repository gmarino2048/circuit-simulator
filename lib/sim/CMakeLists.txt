
add_library(circsim-sim SHARED)

target_include_directories(circsim-sim
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_sources(circsim-sim
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/Simulator.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/WireGroup.cpp
)

target_link_libraries(circsim-sim
    PRIVATE
        circsim-common
        circsim-components
        circsim-data
)
