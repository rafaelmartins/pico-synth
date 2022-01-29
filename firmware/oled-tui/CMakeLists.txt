add_library(oled_tui INTERFACE)

target_sources(oled_tui INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include/oled-tui.h
    ${CMAKE_CURRENT_LIST_DIR}/oled-tui.c
    ${CMAKE_CURRENT_LIST_DIR}/font.h
)

target_include_directories(oled_tui INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(oled_tui INTERFACE
    hardware_i2c
)
