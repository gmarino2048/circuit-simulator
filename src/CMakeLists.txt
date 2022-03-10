
file(
    GLOB components_SRC "components/*.cpp"
)

add_library(
    circsim-components SHARED
    ${components_SRC}
)

file(
    GLOB data_SRC
    "data/*.cpp"
    "data/objects/*.cpp"
)

add_library(
    circsim-data SHARED
    ${data_SRC}
)

target_link_libraries(
    circsim-data
    circsim-components
    ${SQLITE3_LIBRARIES}
)

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
