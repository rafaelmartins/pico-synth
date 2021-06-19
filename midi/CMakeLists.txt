add_library(midi INTERFACE)

target_sources(midi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include/midi.h
    ${CMAKE_CURRENT_LIST_DIR}/midi.c
)

target_include_directories(midi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)
