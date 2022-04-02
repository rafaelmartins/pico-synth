add_library(pico_synth_engine INTERFACE)

target_sources(pico_synth_engine INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/engine.c
    ${CMAKE_CURRENT_LIST_DIR}/include/pico-synth/engine.h

    ${CMAKE_CURRENT_LIST_DIR}/driver-mcp4822.c
    ${CMAKE_CURRENT_LIST_DIR}/driver-mcp4822.h
    ${CMAKE_CURRENT_LIST_DIR}/driver-mcp4822-data.h
)

pico_generate_pio_header(pico_synth_engine
    ${CMAKE_CURRENT_LIST_DIR}/driver-mcp4822.pio
)

target_include_directories(pico_synth_engine INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(pico_synth_engine INTERFACE
    hardware_pio
)
