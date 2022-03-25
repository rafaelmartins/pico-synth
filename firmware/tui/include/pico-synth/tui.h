#pragma once

#include <hardware/i2c.h>

typedef enum {
    PS_TUI_ENCODER_ACTION_BUTTON = 1,
    PS_TUI_ENCODER_ACTION_ROTATE_CW,
    PS_TUI_ENCODER_ACTION_ROTATE_CCW,
} ps_tui_encoder_action_t;

typedef enum {
    PS_TUI_OLED_HALIGN_LEFT = 1,
    PS_TUI_OLED_HALIGN_RIGHT,
    PS_TUI_OLED_HALIGN_CENTER,
} ps_tui_oled_halign_t;

typedef struct {
    struct {
        i2c_inst_t *i2c;
        uint baudrate;
        uint basepin;
    } i2c;

    struct {
        uint8_t i2c_address;
        enum {
            PS_TUI_EEPROM_24LC256 = 1,
            PS_TUI_EEPROM_24LC512,
        } ic;

        // private
        size_t _page_size;
    } eeprom;

    struct {
        uint button;
        uint a;
        uint b;
        uint64_t button_debounce_us;
        uint64_t rotate_debounce_us;

        // private
        void *_data;
        uint64_t _button_hit;
        uint64_t _rotate_hit;
        uint8_t _rotate_state;
        bool _a_value;
        bool _b_value;
    } encoder;

    struct {
        uint8_t i2c_address;
        enum {
            PS_TUI_OLED_SSD1306 = 1,
            PS_TUI_OLED_SH1106,
        } controller;

        // private
        struct {
            uint8_t data[129];
            bool rendered;
        } _ram_pages[8];
        uint8_t _render_cmds[4];  // FIXME: avoid hardcoding size
    } oled;
} ps_tui_t;

int ps_tui_init(ps_tui_t *tui);
int ps_tui_task(ps_tui_t *tui);

int ps_tui_eeprom_read(ps_tui_t *tui, uint16_t addr, uint8_t *data, size_t data_len);
int ps_tui_eeprom_write(ps_tui_t *tui, uint16_t addr, uint8_t *data, size_t data_len);

int ps_tui_oled_clear_line(ps_tui_t *tui, uint8_t line);
void ps_tui_oled_clear(ps_tui_t *tui);
int ps_tui_oled_line(ps_tui_t *tui, uint8_t line, const char *str, ps_tui_oled_halign_t align);
