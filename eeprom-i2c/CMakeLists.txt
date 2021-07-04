add_library(eeprom_i2c INTERFACE)

target_sources(eeprom_i2c INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include/eeprom-i2c.h
    ${CMAKE_CURRENT_LIST_DIR}/eeprom-i2c.c
)

target_include_directories(eeprom_i2c INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(eeprom_i2c INTERFACE
    hardware_i2c
)
