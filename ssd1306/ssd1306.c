#include <hardware/i2c.h>
#include <pico/stdlib.h>
#include <string.h>
#include <ssd1306.h>
#include "ssd1306_font.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define MAX_LINES ((int) (SCREEN_HEIGHT / (FONT_HEIGHT + 1)))
#define MAX_CHARS ((int) (SCREEN_WIDTH / (FONT_WIDTH + 1)))

static uint8_t startup_cmds[27] = {
    0,
    0xAE,        // display off (page 28, 37)
    0xA8, 0x3F,  // set multiplex ratio (page 31, 37, app: page 5)
    0xD3, 0x00,  // set display offset (page 31, 37, app: page 5)
    0x40,        // set display start line (page 31, 36, app: page 5)
    0xA1,        // set segment remap (page 31, 36)
    0xC8,        // set COM output scan direction (page 40-42)
    0xDA, 0x12,  // set COM pins hardware configuration (page 40-42)
    0x81, 0x7F,  // set contrast control (page 36, app: page 5)
    0xA4,        // disable entire display on (page 28, 37)
    0xA6,        // set normal display (page 28, 37, app: page 5)
    0xD5, 0x80,  // set osc frequency (page 40, app: page 5)
    0x8D, 0x14,  // enable charge pump regulator (app: page 3, 5)
    0x20, 0x00,  // set memory addressing mode - horizontal (page 34-35)
    0xD9, 0xF1,  // set pre-charge period (page 32, 40)
    0xDB, 0x40,  // set Vcomh deselect level (page 32, 43)
    0x2E,        // deactivate scroll (page 29, 46)
    0xAF,        // display on (page 28, 37, app: page 5)
};

static uint8_t refresh_cmds[7] = {
    0,
    0x21, 0x00, 0x7F,  // set column address (page 30, 35)
    0x22, 0x00, 0x07,  // set page address (page 31, 36)
};


void
ssd1306_init(ssd1306_t *s)
{
    hard_assert(s);

    s->_data[0] = 0x40;
    ssd1306_clear(s);
    hard_assert(0 <= i2c_write_blocking(s->i2c, 0x3C, startup_cmds, 27, false));
    hard_assert(0 <= ssd1306_render(s));
}


int
__not_in_flash_func(ssd1306_render)(ssd1306_t *s)
{
    hard_assert(s);
    hard_assert(s->i2c);

    int rv = i2c_write_blocking(s->i2c, 0x3C, refresh_cmds, 7, false);
    if (rv < 0)
        return rv;

    return i2c_write_blocking(s->i2c, 0x3C, s->_data, 1025, false);
}


void
ssd1306_clear(ssd1306_t *s)
{
    hard_assert(s);
    memset(s->_data + 1, 0, 1024);
}


int
ssd1306_add_pixel(ssd1306_t *s, uint8_t x, uint8_t y, bool on)
{
    hard_assert(s);

    if ((x >= SCREEN_WIDTH) || (y >= SCREEN_HEIGHT))
        return PICO_ERROR_GENERIC;

    // line_start = line_width * line_number = SCREEN_WIDTH * (y / 8)
    // index = line_start + x
    uint16_t index = (SCREEN_WIDTH * (y / 8)) + x;

    // bit_ord = y & 0b111
    // bit_selected = 1 << bit_ord
    uint8_t bit_selected = (1 << (y & 0b111));

    if (on)
        s->_data[index + 1] |= bit_selected;
    else
        s->_data[index + 1] &= ~bit_selected;

    return PICO_OK;
}


int
ssd1306_add_string(ssd1306_t *s, uint8_t x, uint8_t y, const char *string)
{
    hard_assert(s);

    int rv = PICO_OK;

    if (string == NULL)
        return rv;

    for (size_t i = 0; string[i] != '\0' && i < MAX_CHARS;) {
        uint16_t offset = *(font_descriptors + string[i]);
        const uint8_t *bm = font_bitmaps + offset;

        for (uint8_t j = 0; j < FONT_HEIGHT; ++j) {
            for (uint8_t k = 0; k < FONT_WIDTH; ++k) {
                // XXX: won't work for font width >= 8
                rv = ssd1306_add_pixel(s, x + k, y + j, (bm[j] << k) & 0b10000000);
                if (rv != PICO_OK)
                    return rv;
            }
        }

        x += FONT_WIDTH;
        if (string[++i] != '\0')
            x++;
    }

    return rv;
}


int
ssd1306_add_string_line(ssd1306_t *s, uint8_t line, const char *string, ssd1306_line_align_t align)
{
    hard_assert(s);

    if (line >= MAX_LINES)
        return PICO_ERROR_GENERIC;

    size_t slen = ssd1306_get_string_width(string);
    if (slen > SCREEN_WIDTH)
        return PICO_ERROR_GENERIC;

    uint8_t x = 0;
    uint8_t y = line * (FONT_HEIGHT + 1);

    switch (align) {
        case SSD1306_LINE_ALIGN_LEFT:
            break;
        case SSD1306_LINE_ALIGN_RIGHT:
            x = SCREEN_WIDTH - slen;
            break;
        case SSD1306_LINE_ALIGN_CENTER:
            x = (SCREEN_WIDTH - slen) / 2;
            break;
    }

    return ssd1306_add_string(s, x, y, string);
}


size_t
ssd1306_get_string_width(const char *string)
{
    if (string == NULL)
        return 0;

    size_t x = 0;
    for (size_t i = 0; string[i] != '\0' && i < MAX_CHARS;) {
        x += FONT_WIDTH;
        if (string[++i] != '\0')
            x++;
    }

    return x;
}
