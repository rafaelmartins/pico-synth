/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <hardware/i2c.h>

#define PS_TUI_EEPROM_BACKLOG_SIZE 1024

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
        void *base_address;
        size_t max_offset;

        // private
        size_t _page_size;
        struct {
            size_t addr;
            size_t len;
        } _backlog[PS_TUI_EEPROM_BACKLOG_SIZE];
        size_t _backlog_idx;
    } eeprom;

    struct {
        uint button;
        uint a;
        uint b;
        uint64_t button_debounce_us;
        uint64_t rotate_debounce_us;

        // private
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

    void *ctx_data;

    // private
    uint8_t _selected;
    uint8_t _selected_line;
    const struct ps_tui_screen *_current_screen;
    uint64_t _current_screen_us;
} ps_tui_t;

typedef bool (*ps_tui_screen_line_func_t) (ps_tui_t *tui, char *buf, size_t buflen);

typedef struct {
    char content[22];
    ps_tui_oled_halign_t align;
    ps_tui_screen_line_func_t func;
    enum {
        PS_TUI_SCREEN_LINE_CONTENT,
        PS_TUI_SCREEN_LINE_FUNC,
    } type;
} ps_tui_screen_line_t;

typedef void (*ps_tui_screen_action_func_t) (ps_tui_t *tui);

typedef struct {
    enum {
        PS_TUI_SCREEN_ACTION_FUNC = 1,
        PS_TUI_SCREEN_ACTION_NEXT,
    } type;
    union {
        ps_tui_screen_action_func_t func;
        const struct ps_tui_screen *next;
    };
} ps_tui_screen_action_t;

typedef struct {
    void (*render_func) (ps_tui_t *tui);
    void (*encoder_func) (ps_tui_t *tui, ps_tui_encoder_action_t act);
} ps_tui_screen_func_t;

typedef struct {
    ps_tui_screen_line_t title;
    uint8_t num_items;
    struct {
        char content[20];
        ps_tui_screen_action_t action;
    } items[];
} ps_tui_screen_menu_t;

typedef struct {
    ps_tui_screen_line_t line0;
    ps_tui_screen_line_t line1;
    ps_tui_screen_line_t line2;
    ps_tui_screen_line_t line3;
    ps_tui_screen_line_t line4;
    ps_tui_screen_line_t line5;
    ps_tui_screen_line_t line6;
    ps_tui_screen_line_t line7;
    ps_tui_screen_action_t action;
} ps_tui_screen_lines_t;

typedef void (*ps_tui_screen_select_byte_to_string_func_t) (ps_tui_t *tui, uint8_t b, char *buf, size_t buflen);
typedef uint8_t (*ps_tui_screen_select_byte_getter_func_t) (ps_tui_t *tui);
typedef void (*ps_tui_screen_select_byte_setter_func_t) (ps_tui_t *tui, uint8_t b);

typedef struct {
    ps_tui_screen_line_t title;
    ps_tui_screen_select_byte_to_string_func_t to_string_func;
    ps_tui_screen_select_byte_getter_func_t getter_func;
    ps_tui_screen_select_byte_setter_func_t setter_func;
    ps_tui_screen_action_t action;
    uint8_t min;
    uint8_t max;
} ps_tui_screen_select_byte_t;

typedef struct ps_tui_screen {
    enum {
        PS_TUI_SCREEN_FUNC = 1,
        PS_TUI_SCREEN_MENU,
        PS_TUI_SCREEN_LINES,
        PS_TUI_SCREEN_SELECT_BYTE,
    } type;
    union {
        const ps_tui_screen_func_t *func;
        const ps_tui_screen_menu_t *menu;
        const ps_tui_screen_lines_t *lines;
        const ps_tui_screen_select_byte_t *select_byte;
    };
    struct {
        uint32_t delay_ms;
        ps_tui_screen_action_t action;
    } auto_action;
} ps_tui_screen_t;

int ps_tui_init(ps_tui_t *tui);
int ps_tui_task(ps_tui_t *tui);

int ps_tui_eeprom_read(ps_tui_t *tui, void *data, size_t data_len);
int ps_tui_eeprom_write(ps_tui_t *tui, void *data, size_t data_len);
void ps_tui_eeprom_write_lazy(ps_tui_t *tui, void *data, size_t data_len);
int ps_tui_eeprom_sync(ps_tui_t *tui);
int ps_tui_eeprom_erase(ps_tui_t *tui);

int ps_tui_oled_clear_line(ps_tui_t *tui, uint8_t line);
void ps_tui_oled_clear(ps_tui_t *tui);
int ps_tui_oled_line(ps_tui_t *tui, uint8_t line, const char *str, ps_tui_oled_halign_t align);

int ps_tui_screen_load(ps_tui_t *tui, const ps_tui_screen_t *screen);
int ps_tui_screen_reload(ps_tui_t *tui, const ps_tui_screen_t *screen);
