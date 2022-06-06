# SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
# SPDX-License-Identifier: BSD-3-Clause

add_library(pico_synth_engine INTERFACE)

target_sources(pico_synth_engine INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/engine.c
    ${CMAKE_CURRENT_LIST_DIR}/engine-data.h
    ${CMAKE_CURRENT_LIST_DIR}/include/pico-synth/engine.h

    ${CMAKE_CURRENT_LIST_DIR}/driver-mcp4822.c
    ${CMAKE_CURRENT_LIST_DIR}/driver-mcp4822.h
    ${CMAKE_CURRENT_LIST_DIR}/driver-mcp4822-data.h

    ${CMAKE_CURRENT_LIST_DIR}/module-amplifier.c
    ${CMAKE_CURRENT_LIST_DIR}/include/pico-synth/engine/module-amplifier.h

    ${CMAKE_CURRENT_LIST_DIR}/module-mixer.c
    ${CMAKE_CURRENT_LIST_DIR}/include/pico-synth/engine/module-mixer.h

    ${CMAKE_CURRENT_LIST_DIR}/module-oscillator.c
    ${CMAKE_CURRENT_LIST_DIR}/include/pico-synth/engine/module-oscillator.h

    ${CMAKE_CURRENT_LIST_DIR}/note.c
    ${CMAKE_CURRENT_LIST_DIR}/note-data.h
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
