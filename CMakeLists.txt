cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(pico-synth)

pico_sdk_init()

add_executable(pico-synth
    main.c
    usb.c
    note.c
    phase.c
    waveform.c
    oscillator.c
    controller.c
)

target_link_libraries(pico-synth PRIVATE
    hardware_i2c
    hardware_pio
    pico_stdlib
    pico_multicore
    tinyusb_device
    mcp4822
    ssd1306
)

target_include_directories(pico-synth PRIVATE
    include
    .
)

target_compile_definitions(tinyusb_device INTERFACE
    TUD_OPT_RP2040_USB_DEVICE_ENUMERATION_FIX=1
)

pico_enable_stdio_uart(pico-synth 1)

add_subdirectory(mcp4822)
add_subdirectory(ssd1306)
