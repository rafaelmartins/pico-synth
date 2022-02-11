#pragma once

typedef struct oled_tui_menu {
    char title[15];
    uint8_t selected;
    uint8_t selected_line;
    uint8_t num_items;
    char items[][15];
} oled_tui_menu_t;

typedef enum {
    OLED_TUI_MENU_SCROLL_NONE,
    OLED_TUI_MENU_SCROLL_UP,
    OLED_TUI_MENU_SCROLL_DOWN,
} oled_tui_menu_scroll_direction_t;

int oled_tui_menu_render(oled_tui_t *s, oled_tui_menu_t *m, oled_tui_menu_scroll_direction_t scr);
