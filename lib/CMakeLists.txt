
add_subdirectory(common)
add_subdirectory(components)
add_subdirectory(data)

file(
    GLOB sim_SRC "sim/*.cpp"
)

add_library(
    circsim-sim SHARED
    ${sim_SRC}
)

target_link_libraries(
    circsim-sim
    circsim-components
    circsim-data
)
