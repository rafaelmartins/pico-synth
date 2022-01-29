add_library(mcp4822 INTERFACE)

target_sources(mcp4822 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/mcp4822.c
)

target_include_directories(mcp4822 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

pico_generate_pio_header(mcp4822
    ${CMAKE_CURRENT_LIST_DIR}/mcp4822.pio
)
