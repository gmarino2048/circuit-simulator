
add_library(circsim-common)

target_include_directories(circsim-common
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

target_sources(circsim-common
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/EndianOperations.cpp
)
