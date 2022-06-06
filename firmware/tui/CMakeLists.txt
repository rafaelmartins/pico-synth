# SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
# SPDX-License-Identifier: BSD-3-Clause

add_library(pico_synth_tui INTERFACE)

target_sources(pico_synth_tui INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/tui.c
    ${CMAKE_CURRENT_LIST_DIR}/include/pico-synth/tui.h

    ${CMAKE_CURRENT_LIST_DIR}/driver-ec11.c
    ${CMAKE_CURRENT_LIST_DIR}/driver-ec11.h

    ${CMAKE_CURRENT_LIST_DIR}/driver-eeprom.c
    ${CMAKE_CURRENT_LIST_DIR}/driver-eeprom.h

    ${CMAKE_CURRENT_LIST_DIR}/driver-oled.c
    ${CMAKE_CURRENT_LIST_DIR}/driver-oled-font.c
    ${CMAKE_CURRENT_LIST_DIR}/driver-oled.h

    ${CMAKE_CURRENT_LIST_DIR}/screen.c
)

target_include_directories(pico_synth_tui INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(pico_synth_tui INTERFACE
    hardware_i2c
)
