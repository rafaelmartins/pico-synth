#pragma once

#include <hardware/i2c.h>

typedef struct {
    i2c_inst_t *i2c;
    uint8_t i2c_address;
    enum {
        OLED_TUI_SSD1306 = 1,
        OLED_TUI_SH1106,
    } oled_controller;

    // private
    struct {
        uint8_t data[129];
        bool rendered;
    } _ram_pages[8];
    uint8_t render_cmds[4];  // FIXME: avoid hardcoding size
} oled_tui_t;

typedef enum {
    OLED_TUI_HALIGN_LEFT = 0,
    OLED_TUI_HALIGN_RIGHT,
    OLED_TUI_HALIGN_CENTER,
} oled_tui_halign_t;

int oled_tui_init(oled_tui_t *s);
int oled_tui_clear_line(oled_tui_t *s, uint8_t line);
void oled_tui_clear(oled_tui_t *s);
int oled_tui_line(oled_tui_t *s, uint8_t line, const char *str, oled_tui_halign_t align);
int oled_tui_render(oled_tui_t *s);
