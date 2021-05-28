#pragma once

#include <hardware/i2c.h>

typedef struct {
    i2c_inst_t *i2c;
    uint8_t _data[1025];
} ssd1306_t;

typedef enum {
    SSD1306_LINE_ALIGN_LEFT = 0,
    SSD1306_LINE_ALIGN_RIGHT,
    SSD1306_LINE_ALIGN_CENTER,
} ssd1306_line_align_t;

void ssd1306_init(ssd1306_t *s);
int ssd1306_render(ssd1306_t *s);
void ssd1306_clear(ssd1306_t *s);
int ssd1306_add_pixel(ssd1306_t *s, uint8_t x, uint8_t y, bool on);
int ssd1306_add_string(ssd1306_t *s, uint8_t x, uint8_t y, const char *string);
int ssd1306_add_string_line(ssd1306_t *s, uint8_t line, const char *string,
    ssd1306_line_align_t align);
size_t ssd1306_get_string_width(const char *string);
