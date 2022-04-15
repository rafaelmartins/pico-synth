cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(pico-synth)

pico_sdk_init()

add_executable(pico-synth
    main.c

    screens.c
    screens.h

    settings.c
    settings.h
)

target_link_libraries(pico-synth PRIVATE
    pico_stdlib
    pico_multicore


    pico_synth_engine
    pico_synth_midi
    pico_synth_tui
)

target_compile_definitions(tinyusb_device INTERFACE
    TUD_OPT_RP2040_USB_DEVICE_ENUMERATION_FIX=1
)

pico_enable_stdio_uart(pico-synth 1)

add_subdirectory(engine)
add_subdirectory(midi)
add_subdirectory(tui)
