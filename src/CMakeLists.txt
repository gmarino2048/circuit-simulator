
file(
    GLOB components_SRC "components/*.cpp"
)

add_library(
    circsim-components
    ${components_SRC}
)

file(
    GLOB data_SRC "data/*.cpp"
)

add_library(
    circsim-data
    ${data_SRC}
)

target_link_libraries(
    circsim-data
    circsim-components
)
