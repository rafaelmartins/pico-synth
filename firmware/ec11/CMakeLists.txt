add_library(ec11 INTERFACE)

target_sources(ec11 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include/ec11.h
    ${CMAKE_CURRENT_LIST_DIR}/ec11.c
)

target_include_directories(ec11 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ec11 INTERFACE
    hardware_gpio
    hardware_timer
)
