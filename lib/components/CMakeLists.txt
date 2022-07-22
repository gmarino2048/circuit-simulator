
add_library(circsim-components SHARED)

target_include_directories(circsim-components
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_sources(circsim-components
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/Transistor.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Wire.cpp
)

target_link_libraries(circsim-components
    PRIVATE
        circsim-common
)