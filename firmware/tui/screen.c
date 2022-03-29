#include <string.h>
#include "driver-ec11.h"
#include "driver-oled.h"

// FIXME: make this thread safe? is it needed?
static char buf_selected[17]   = "> ";
static char buf_unselected[17] = "  ";
static char buf_line[17];


static void
run_action(ps_tui_t *tui, const ps_tui_screen_action_t *act)
{
    hard_assert(tui);
    if (act == NULL)
        return;

    switch (act->type) {
    case PS_TUI_SCREEN_ACTION_CALLBACK:
        if (act->action.callback != NULL)
            act->action.callback(tui);
        break;

    case PS_TUI_SCREEN_ACTION_NEXT:
        ps_tui_screen_load(tui, act->action.next);
        break;
    }
}


static const char*
line_get(ps_tui_t *tui, const ps_tui_screen_line_t *l)
{
    if (tui == NULL || l == NULL)
        return NULL;

    switch (l->type) {
    case PS_TUI_SCREEN_LINE_CALLBACK:
        if (l->callback != NULL) {
            memset(buf_line, 0, sizeof(buf_line));
            l->callback(tui, buf_line, sizeof(buf_line));
            return buf_line;
        }
        break;

    case PS_TUI_SCREEN_LINE_CONTENT:
        return l->content;
    }

    return NULL;
}


static void
menu_callback(ps_tui_t *tui, const ps_tui_screen_menu_t *m, ps_tui_encoder_action_t act)
{
    hard_assert(tui);
    if (m == NULL)
        return;

    uint8_t scroll = 5;
    uint8_t start = 0;
    uint8_t end = 5;
    uint8_t offset = 4;
    uint8_t step = 1;

    switch (act) {
    case PS_TUI_ENCODER_ACTION_BUTTON:
        run_action(tui, &m->items[tui->_selected].action);
        break;

    case PS_TUI_ENCODER_ACTION_ROTATE_CCW:
        if (tui->_selected == 0)
            return;

        scroll = 0;
        start = 1;
        end = 6;
        offset = 1;
        step = -1;

    case PS_TUI_ENCODER_ACTION_ROTATE_CW:
        if (act != PS_TUI_ENCODER_ACTION_ROTATE_CCW && tui->_selected >= m->num_items - 1)
            return;

        if (tui->_selected_line == scroll) {
            for (uint8_t i = start; i < end; i++) {
                memcpy(buf_unselected + 2, m->items[tui->_selected + i - offset].content, sizeof(m->items[tui->_selected + i - offset].content));
                ps_tui_oled_line(tui, i + 2, buf_unselected, PS_TUI_OLED_HALIGN_LEFT);
                buf_unselected[2] = 0;
            }
        }
        else {
            memcpy(buf_unselected + 2, m->items[tui->_selected].content, sizeof(m->items[tui->_selected].content));
            ps_tui_oled_line(tui, tui->_selected_line + 2, buf_unselected, PS_TUI_OLED_HALIGN_LEFT);
            buf_unselected[2] = 0;

            tui->_selected_line += step;
        }
        tui->_selected += step;

        memcpy(buf_selected + 2, m->items[tui->_selected].content, sizeof(m->items[tui->_selected].content));
        ps_tui_oled_line(tui, tui->_selected_line + 2, buf_selected, PS_TUI_OLED_HALIGN_LEFT);
        buf_selected[2] = 0;
        break;

    default:
        return;
    }

    oled_render(tui);
}


void
ec11_callback(ps_tui_t *tui, ps_tui_encoder_action_t act)
{
    hard_assert(tui);
    if (tui->_current_screen == NULL)
        return;

    switch (tui->_current_screen->type) {
    case PS_TUI_SCREEN_CALLBACK:
        if (tui->_current_screen->screen.callback != NULL && tui->_current_screen->screen.callback->encoder_callback != NULL)
            tui->_current_screen->screen.callback->encoder_callback(tui, act);
        break;

    case PS_TUI_SCREEN_MENU:
        menu_callback(tui, tui->_current_screen->screen.menu, act);
        break;

    case PS_TUI_SCREEN_LINES:
        if (tui->_current_screen->screen.lines != NULL && act == PS_TUI_ENCODER_ACTION_BUTTON)
            run_action(tui, &tui->_current_screen->screen.lines->action);
        break;
    }
}


int
ps_tui_screen_load(ps_tui_t *tui, const ps_tui_screen_t *screen)
{
    hard_assert(tui);
    if (screen == NULL)
        return PICO_OK;  // FIXME: clear and render ?

    switch (screen->type) {
    case PS_TUI_SCREEN_CALLBACK:
        if (screen->screen.callback == NULL || screen->screen.callback->render_callback == NULL)
            return PICO_OK;

        screen->screen.callback->render_callback(tui);
        break;

    case PS_TUI_SCREEN_MENU:
        if (screen->screen.menu == NULL)
            return PICO_OK;

        ps_tui_oled_clear(tui);
        ps_tui_oled_line(tui, 0, line_get(tui, &screen->screen.menu->title), PS_TUI_OLED_HALIGN_CENTER);
        tui->_selected_line = 0;
        tui->_selected = 0;
        for (uint8_t i = 0; i < (screen->screen.menu->num_items > 6 ? 6 : screen->screen.menu->num_items); i++) {
            char *b = i == tui->_selected ? buf_selected : buf_unselected;
            memcpy(b + 2, screen->screen.menu->items[i].content, sizeof(screen->screen.menu->items[i].content));
            ps_tui_oled_line(tui, i + 2, b, PS_TUI_OLED_HALIGN_LEFT);
            b[2] = 0;
        }
        break;

    case PS_TUI_SCREEN_LINES:
        if (screen->screen.lines == NULL)
            return PICO_OK;

        ps_tui_oled_line(tui, 0, line_get(tui, &screen->screen.lines->line0), screen->screen.lines->line0.align);
        ps_tui_oled_line(tui, 1, line_get(tui, &screen->screen.lines->line1), screen->screen.lines->line1.align);
        ps_tui_oled_line(tui, 2, line_get(tui, &screen->screen.lines->line2), screen->screen.lines->line2.align);
        ps_tui_oled_line(tui, 3, line_get(tui, &screen->screen.lines->line3), screen->screen.lines->line3.align);
        ps_tui_oled_line(tui, 4, line_get(tui, &screen->screen.lines->line4), screen->screen.lines->line4.align);
        ps_tui_oled_line(tui, 5, line_get(tui, &screen->screen.lines->line5), screen->screen.lines->line5.align);
        ps_tui_oled_line(tui, 6, line_get(tui, &screen->screen.lines->line6), screen->screen.lines->line6.align);
        ps_tui_oled_line(tui, 7, line_get(tui, &screen->screen.lines->line7), screen->screen.lines->line7.align);
        break;

    default:
        return PICO_OK;
    }

    int rv = oled_render(tui);
    if (rv != PICO_OK)
        return rv;

    tui->_current_screen = screen;
    return PICO_OK;

    return 0;
}
