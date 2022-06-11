/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include "driver-oled.h"

extern const uint8_t font_bitmaps[];
extern const uint16_t font_descriptors[];

static const uint8_t ssd1306_startup_cmds[] = {
    0x00,        // Co = 0; D/|C = 0
    0xA1,        // set segment remap (reverse direction)
    0xC8,        // set COM output scan direction (COM[N-1] to COM0)
    0x8D, 0x14,  // charge pump setting (enable during display on)
    0xAF,        // display on (page 28, 37, app: page 5)
};

static const uint8_t sh1106_startup_cmds[] = {
    0x00,  // Co = 0; D/|C = 0
    0xA1,  // set segment remap (reverse direction)
    0xC8,  // set COM output scan direction (COM[N-1] to COM0)
    0xAF,  // display on
};

static const uint8_t render_cmds[] = {
    0x00,  // Co = 0; D/|C = 0
    0xB0,  // set page address (page number set later during render)
    0x00,  // set column address 4 lower bits (0 by default)
    0x10,  // set column address 4 higher bits (0)
};


int
oled_init(ps_tui_t *tui)
{
    hard_assert(tui);
    hard_assert(tui->i2c.i2c);

    // broadcasts not supported, we assume that address 0 means "use default"
    if (tui->oled.i2c_address == 0)
        tui->oled.i2c_address = 0x3C;

    // we need to copy render commands because we change them when rendering
    // every ram page, and this would cause race conditions when running more
    // than one oled display in parallel.
    memcpy(tui->oled._render_cmds, render_cmds, sizeof(tui->oled._render_cmds));

    const uint8_t *startup_cmds = NULL;
    size_t startup_cmds_size = 0;

    switch (tui->oled.controller) {
        case PS_TUI_OLED_SSD1306:
            startup_cmds = ssd1306_startup_cmds;
            startup_cmds_size = sizeof(ssd1306_startup_cmds);
            break;

        case PS_TUI_OLED_SH1106:
            startup_cmds = sh1106_startup_cmds;
            startup_cmds_size = sizeof(sh1106_startup_cmds);
            // offset 2 pixels because sh1106 is 132x64 (centered) instead of 128x64
            tui->oled._render_cmds[2] = 0x02;
            break;
    }

    if (startup_cmds == NULL)
        return PICO_ERROR_GENERIC;

    for (size_t i = 0; i < RAM_PAGES; i++)
        tui->oled._ram_pages[i].data[0] = 0x40;  // Co = 0; D/|C = 1

    ps_tui_oled_clear(tui);

    int rv = i2c_write_blocking(tui->i2c.i2c, tui->oled.i2c_address, startup_cmds,
        startup_cmds_size, false);
    if (rv < 0)
        return rv;

    return oled_render(tui);
}


int
ps_tui_oled_clear_line(ps_tui_t *tui, uint8_t line)
{
    hard_assert(tui);

    if (line >= MAX_LINES)
        return PICO_ERROR_GENERIC;

    memset(tui->oled._ram_pages[line].data + 1, 0, sizeof(tui->oled._ram_pages[line].data) - 1);
    tui->oled._ram_pages[line].rendered = false;

    return PICO_OK;
}


void
ps_tui_oled_clear(ps_tui_t *tui)
{
    hard_assert(tui);

    for (uint8_t i = 0; i < RAM_PAGES; i++)
        ps_tui_oled_clear_line(tui, i);
}


int
__not_in_flash_func(oled_render)(ps_tui_t *tui)
{
    hard_assert(tui);
    hard_assert(tui->i2c.i2c);

    for (size_t i = 0; i < RAM_PAGES; i++) {
        if (tui->oled._ram_pages[i].rendered)
            continue;

        tui->oled._render_cmds[1] &= ~0b111;
        tui->oled._render_cmds[1] |= i;
        int rv = i2c_write_blocking(tui->i2c.i2c, tui->oled.i2c_address, tui->oled._render_cmds,
            sizeof(tui->oled._render_cmds), false);
        if (rv < 0)
            return rv;

        rv = i2c_write_blocking(tui->i2c.i2c, tui->oled.i2c_address, tui->oled._ram_pages[i].data,
            sizeof(tui->oled._ram_pages[i].data), false);
        if (rv < 0)
            return rv;

        tui->oled._ram_pages[i].rendered = true;
    }

    return 0;
}


int
ps_tui_oled_line(ps_tui_t *tui, uint8_t line, const char *str, ps_tui_oled_halign_t align)
{
    hard_assert(tui);

    if (str == NULL)
        return PICO_OK;

    if (line >= MAX_LINES)
        return PICO_ERROR_GENERIC;

    uint8_t slen = 0;
    for (uint8_t i = 0; str[i] != '\0' && i < MAX_CHARS_PER_LINE;) {
        slen += FONT_WIDTH;
        if (str[++i] != '\0')
            slen++;
    }

    if (slen > SCREEN_WIDTH)
        return PICO_ERROR_GENERIC;

    uint8_t x = 0;
    switch (align) {
        case PS_TUI_OLED_HALIGN_LEFT:
            break;
        case PS_TUI_OLED_HALIGN_RIGHT:
            x = SCREEN_WIDTH - slen;
            break;
        case PS_TUI_OLED_HALIGN_CENTER:
            x = (SCREEN_WIDTH - slen) / 2;
            break;
    }

    memset(tui->oled._ram_pages[line].data + 1, 0, sizeof(tui->oled._ram_pages[line].data) - 1);

    for (uint8_t i = 0; str[i] != '\0' && i < MAX_CHARS_PER_LINE;) {
        uint16_t offset = *(font_descriptors + str[i]);
        const uint8_t *bm = font_bitmaps + offset;

        for (uint8_t j = 0; j < FONT_HEIGHT; ++j) {
            for (uint8_t k = 0; k < FONT_WIDTH; ++k) {
                if ((bm[j] << k) & (1 << 7))
                    tui->oled._ram_pages[line].data[x + k + 1] |= (1 << j);
                else
                    tui->oled._ram_pages[line].data[x + k + 1] &= ~(1 << j);
            }
        }

        x += FONT_WIDTH;
        if (str[++i] != '\0')
            x++;
    }

    tui->oled._ram_pages[line].rendered = false;

    return PICO_OK;
}
