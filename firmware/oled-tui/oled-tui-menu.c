#include <stdlib.h>
#include <string.h>
#include <oled-tui.h>
#include <oled-tui-menu.h>

static char buf_selected[17]   = "> ";
static char buf_unselected[17] = "  ";


int
oled_tui_menu_render(oled_tui_t *s, oled_tui_menu_t *m, oled_tui_menu_scroll_direction_t scr)
{
    hard_assert(s);
    if (m == NULL)
        return PICO_ERROR_NO_DATA;

    uint8_t scroll = 5;
    uint8_t start = 0;
    uint8_t end = 5;
    uint8_t offset = 4;
    uint8_t step = 1;

    switch (scr) {
        case OLED_TUI_MENU_SCROLL_NONE:
            oled_tui_line(s, 0, m->title, OLED_TUI_HALIGN_CENTER);
            m->selected_line = 0;
            m->selected = 0;
            for (uint8_t i = 0; i < (m->num_items > 6 ? 6 : m->num_items); i++) {
                char *b = i == m->selected ? buf_selected : buf_unselected;
                memcpy(b + 2, m->items[i], sizeof(m->items[i]));
                oled_tui_line(s, i + 2, b, OLED_TUI_HALIGN_LEFT);
                b[2] = 0;
            }
            break;

        case OLED_TUI_MENU_SCROLL_UP:
            if (m->selected == 0)
                return PICO_OK;

            scroll = 0;
            start = 1;
            end = 6;
            offset = 1;
            step = -1;

        case OLED_TUI_MENU_SCROLL_DOWN:
            if (scr != OLED_TUI_MENU_SCROLL_UP && m->selected >= m->num_items - 1)
                return PICO_OK;

            if (m->selected_line == scroll) {
                for (uint8_t i = start; i < end; i++) {
                    memcpy(buf_unselected + 2, m->items[m->selected + i - offset], sizeof(m->items[m->selected + i - offset]));
                    oled_tui_line(s, i + 2, buf_unselected, OLED_TUI_HALIGN_LEFT);
                    buf_unselected[2] = 0;
                }
            }
            else {
                memcpy(buf_unselected + 2, m->items[m->selected], sizeof(m->items[m->selected]));
                oled_tui_line(s, m->selected_line + 2, buf_unselected, OLED_TUI_HALIGN_LEFT);
                buf_unselected[2] = 0;

                m->selected_line += step;
            }
            m->selected += step;

            memcpy(buf_selected + 2, m->items[m->selected], sizeof(m->items[m->selected]));
            oled_tui_line(s, m->selected_line + 2, buf_selected, OLED_TUI_HALIGN_LEFT);
            buf_selected[2] = 0;
            break;
    }

    return oled_tui_render(s);
}
