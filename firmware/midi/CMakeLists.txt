# SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
# SPDX-License-Identifier: BSD-3-Clause

add_library(pico_synth_midi INTERFACE)

target_sources(pico_synth_midi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include/pico-synth/midi.h
    ${CMAKE_CURRENT_LIST_DIR}/include/tusb_config.h
    ${CMAKE_CURRENT_LIST_DIR}/driver-uart.c
    ${CMAKE_CURRENT_LIST_DIR}/driver-uart.h
    ${CMAKE_CURRENT_LIST_DIR}/driver-usb.c
    ${CMAKE_CURRENT_LIST_DIR}/driver-usb.h
    ${CMAKE_CURRENT_LIST_DIR}/midi.c
)

target_include_directories(pico_synth_midi INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(pico_synth_midi INTERFACE
    hardware_uart
    pico_unique_id
    tinyusb_device
)
