#include <hardware/i2c.h>
#include <string.h>
#include <oled-tui.h>

#include "font.h"

#define FONT_WIDTH 5
#define FONT_HEIGHT 7
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MAX_CHARS_PER_LINE ((uint8_t) (SCREEN_WIDTH / (FONT_WIDTH + 1)))
#define RAM_PAGES          ((uint8_t) (SCREEN_HEIGHT / 8))
// we abuse the fact that our rendered lines match nicely with the ram pages
#define MAX_LINES RAM_PAGES


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
oled_tui_init(oled_tui_t *s)
{
    hard_assert(s);
    hard_assert(s->i2c);

    // broadcasts not supported, we assume that address 0 means "use default"
    if (s->i2c_address == 0)
        s->i2c_address = 0x3C;

    // we need to copy render commands because we change them when rendering
    // every ram page, and this would cause race conditions when running more
    // than one oled display in parallel.
    memcpy(s->render_cmds, render_cmds, sizeof(s->render_cmds));

    const uint8_t *startup_cmds = NULL;
    size_t startup_cmds_size = 0;

    switch (s->oled_controller) {
        case OLED_TUI_SSD1306:
            startup_cmds = ssd1306_startup_cmds;
            startup_cmds_size = sizeof(ssd1306_startup_cmds);
            break;

        case OLED_TUI_SH1106:
            startup_cmds = sh1106_startup_cmds;
            startup_cmds_size = sizeof(sh1106_startup_cmds);
            // offset 2 pixels because sh1106 is 132x64 (centered) instead of 128x64
            s->render_cmds[2] = 0x02;
            break;
    }

    if (startup_cmds == NULL)
        return PICO_ERROR_GENERIC;

    for (size_t i = 0; i < RAM_PAGES; i++)
        s->_ram_pages[i].data[0] = 0x40;  // Co = 0; D/|C = 1

    oled_tui_clear(s);

    int rv = i2c_write_blocking(s->i2c, s->i2c_address, startup_cmds,
        startup_cmds_size, false);
    if (rv < 0)
        return rv;

    return oled_tui_render(s);
}


void
oled_tui_clear(oled_tui_t *s)
{
    hard_assert(s);

    for (size_t i = 0; i < RAM_PAGES; i++) {
        memset(s->_ram_pages[i].data + 1, 0, sizeof(s->_ram_pages[i].data - 1));
        s->_ram_pages[i].rendered = false;
    }
}


int
__not_in_flash_func(oled_tui_render)(oled_tui_t *s)
{
    hard_assert(s);
    hard_assert(s->i2c);

    for (size_t i = 0; i < RAM_PAGES; i++) {
        if (s->_ram_pages[i].rendered)
            continue;

        s->render_cmds[1] &= ~0b111;
        s->render_cmds[1] |= i;
        int rv = i2c_write_blocking(s->i2c, s->i2c_address, s->render_cmds,
            sizeof(s->render_cmds), false);
        if (rv < 0)
            return rv;

        rv = i2c_write_blocking(s->i2c, s->i2c_address, s->_ram_pages[i].data,
            sizeof(s->_ram_pages[i].data), false);
        if (rv < 0)
            return rv;

        s->_ram_pages[i].rendered = true;
    }

    return 0;
}


int
oled_tui_line(oled_tui_t *s, uint8_t line, const char *str, oled_tui_halign_t align)
{
    hard_assert(s);

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

    if (str == NULL)
        return PICO_ERROR_GENERIC;

    uint8_t x = 0;
    switch (align) {
        case OLED_TUI_HALIGN_LEFT:
            break;
        case OLED_TUI_HALIGN_RIGHT:
            x = SCREEN_WIDTH - slen;
            break;
        case OLED_TUI_HALIGN_CENTER:
            x = (SCREEN_WIDTH - slen) / 2;
            break;
    }

    memset(s->_ram_pages[line].data + 1, 0, sizeof(s->_ram_pages[line].data - 1));

    for (uint8_t i = 0; str[i] != '\0' && i < MAX_CHARS_PER_LINE;) {
        uint16_t offset = *(font_descriptors + str[i]);
        const uint8_t *bm = font_bitmaps + offset;

        for (uint8_t j = 0; j < FONT_HEIGHT; ++j) {
            for (uint8_t k = 0; k < FONT_WIDTH; ++k) {
                if ((bm[j] << k) & (1 << 7))
                    s->_ram_pages[line].data[x + k + 1] |= (1 << j);
                else
                    s->_ram_pages[line].data[x + k + 1] &= ~(1 << j);
            }
        }

        x += FONT_WIDTH;
        if (str[++i] != '\0')
            x++;
    }

    s->_ram_pages[line].rendered = false;

    return PICO_OK;
}
