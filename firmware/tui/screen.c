/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "driver-ec11.h"
#include "driver-oled.h"

// FIXME: make this thread safe? is it needed?
static char buf_selected[MAX_CHARS_PER_LINE + 1]   = "> ";
static char buf_unselected[MAX_CHARS_PER_LINE + 1] = "  ";
static char buf_line[MAX_CHARS_PER_LINE + 1];


static void
run_action(ps_tui_t *tui, const ps_tui_screen_action_t *act)
{
    hard_assert(tui);
    if (act == NULL)
        return;

    switch (act->type) {
    case PS_TUI_SCREEN_ACTION_FUNC:
        if (act->func != NULL)
            act->func(tui);
        break;

    case PS_TUI_SCREEN_ACTION_NEXT:
        ps_tui_screen_load(tui, act->next);
        break;
    }
}


static const char*
line_get(ps_tui_t *tui, const ps_tui_screen_line_t *l)
{
    if (tui == NULL || l == NULL)
        return NULL;

    switch (l->type) {
    case PS_TUI_SCREEN_LINE_FUNC:
        if (l->func != NULL) {
            memset(buf_line, 0, sizeof(buf_line));
            if (l->func(tui, buf_line, sizeof(buf_line)))
                return buf_line;
            return NULL;
        }
        break;

    case PS_TUI_SCREEN_LINE_CONTENT:
        return l->content;
    }

    return NULL;
}


static inline void
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


static inline void
select_byte_callback(ps_tui_t *tui, const ps_tui_screen_select_byte_t *sb, ps_tui_encoder_action_t act)
{
    hard_assert(tui);
    if (sb == NULL || sb->getter_func == NULL || sb->setter_func == NULL)
        return;

    uint8_t v = sb->getter_func(tui);

    switch (act) {
    case PS_TUI_ENCODER_ACTION_BUTTON:
        return run_action(tui, &sb->action);

    case PS_TUI_ENCODER_ACTION_ROTATE_CCW:
        if (v > sb->min)
            v--;
        break;

    case PS_TUI_ENCODER_ACTION_ROTATE_CW:
        if (v < sb->max)
            v++;
        break;
    }

    sb->setter_func(tui, v);
    ps_tui_screen_load(tui, tui->_current_screen);
}


void
ec11_callback(ps_tui_t *tui, ps_tui_encoder_action_t act)
{
    hard_assert(tui);
    if (tui->_current_screen == NULL)
        return;

    switch (tui->_current_screen->type) {
    case PS_TUI_SCREEN_FUNC:
        if (tui->_current_screen->func != NULL && tui->_current_screen->func->encoder_func != NULL)
            tui->_current_screen->func->encoder_func(tui, act);
        break;

    case PS_TUI_SCREEN_MENU:
        menu_callback(tui, tui->_current_screen->menu, act);
        break;

    case PS_TUI_SCREEN_LINES:
        if (tui->_current_screen->lines != NULL && act == PS_TUI_ENCODER_ACTION_BUTTON)
            run_action(tui, &tui->_current_screen->lines->action);
        break;

    case PS_TUI_SCREEN_SELECT_BYTE:
        select_byte_callback(tui, tui->_current_screen->select_byte, act);
        break;
    }
}


int
ps_tui_screen_load(ps_tui_t *tui, const ps_tui_screen_t *screen)
{
    hard_assert(tui);
    if (screen == NULL)
        return PICO_OK;  // FIXME: clear and render ?

    uint8_t v = 0;

    switch (screen->type) {
    case PS_TUI_SCREEN_FUNC:
        if (screen->func == NULL || screen->func->render_func == NULL)
            return PICO_OK;

        screen->func->render_func(tui);
        break;

    case PS_TUI_SCREEN_MENU:
        if (screen->menu == NULL)
            return PICO_OK;

        ps_tui_oled_clear(tui);
        ps_tui_oled_line(tui, 0, line_get(tui, &screen->menu->title), PS_TUI_OLED_HALIGN_CENTER);
        tui->_selected_line = 0;
        tui->_selected = 0;
        for (uint8_t i = 0; i < (screen->menu->num_items > 6 ? 6 : screen->menu->num_items); i++) {
            char *b = i == tui->_selected ? buf_selected : buf_unselected;
            memcpy(b + 2, screen->menu->items[i].content, sizeof(screen->menu->items[i].content));
            ps_tui_oled_line(tui, i + 2, b, PS_TUI_OLED_HALIGN_LEFT);
            b[2] = 0;
        }
        break;

    case PS_TUI_SCREEN_LINES:
        if (screen->lines == NULL)
            return PICO_OK;

        ps_tui_oled_line(tui, 0, line_get(tui, &screen->lines->line0), screen->lines->line0.align);
        ps_tui_oled_line(tui, 1, line_get(tui, &screen->lines->line1), screen->lines->line1.align);
        ps_tui_oled_line(tui, 2, line_get(tui, &screen->lines->line2), screen->lines->line2.align);
        ps_tui_oled_line(tui, 3, line_get(tui, &screen->lines->line3), screen->lines->line3.align);
        ps_tui_oled_line(tui, 4, line_get(tui, &screen->lines->line4), screen->lines->line4.align);
        ps_tui_oled_line(tui, 5, line_get(tui, &screen->lines->line5), screen->lines->line5.align);
        ps_tui_oled_line(tui, 6, line_get(tui, &screen->lines->line6), screen->lines->line6.align);
        ps_tui_oled_line(tui, 7, line_get(tui, &screen->lines->line7), screen->lines->line7.align);
        break;

    case PS_TUI_SCREEN_SELECT_BYTE:
        if (screen->select_byte == NULL || screen->select_byte->getter_func == NULL)
            return PICO_OK;

        ps_tui_oled_clear(tui);
        ps_tui_oled_line(tui, 0, line_get(tui, &screen->select_byte->title), PS_TUI_OLED_HALIGN_CENTER);

        v = screen->select_byte->getter_func(tui);
        if (v > screen->select_byte->max)
            v = screen->select_byte->max;
        if (v < screen->select_byte->min)
            v = screen->select_byte->min;

        if (screen->select_byte->to_string_func != NULL) {
            char tmp[sizeof(buf_line) - 6];
            screen->select_byte->to_string_func(tui, v, tmp, sizeof(tmp));
            snprintf(buf_line, sizeof(buf_line), "<< %s >>", tmp);
        }
        else {
            snprintf(buf_line, sizeof(buf_line), "<< %d >>", v);
        }
        ps_tui_oled_line(tui, 4, buf_line, PS_TUI_OLED_HALIGN_CENTER);
        break;
    }

    int rv = oled_render(tui);
    if (rv != PICO_OK)
        return rv;

    tui->_current_screen = screen;
    return PICO_OK;
}


int
ps_tui_screen_reload(ps_tui_t *tui, const ps_tui_screen_t *screen)
{
    if (screen == NULL)
        return PICO_OK;

    if (tui->_current_screen == screen)
        return ps_tui_screen_load(tui, tui->_current_screen);

    return PICO_OK;
}
