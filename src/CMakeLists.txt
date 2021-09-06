
file(
    GLOB components_SRC "components/*.cpp"
)

add_library(
    circsim-components
    ${components_SRC}
)
