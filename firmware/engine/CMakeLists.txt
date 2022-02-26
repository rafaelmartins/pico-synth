add_library(pico_synth_engine INTERFACE)

target_sources(pico_synth_engine INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/engine.c
    ${CMAKE_CURRENT_LIST_DIR}/include/pico-synth/engine.h
)

target_include_directories(pico_synth_engine INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)
