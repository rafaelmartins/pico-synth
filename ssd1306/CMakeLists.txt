add_library(ssd1306 INTERFACE)

target_sources(ssd1306 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/ssd1306.c
)

target_include_directories(ssd1306 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ssd1306 INTERFACE
    hardware_i2c
)
