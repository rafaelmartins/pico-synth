#include <string.h>
#include <pico-synth/tui.h>
#include "settings.h"
#include "screens.h"


// splash

static const ps_tui_screen_lines_t screen_splash_lines = {
    .line1 = {"pico-synth", PS_TUI_OLED_HALIGN_CENTER},
    .line5 = {"(c) 2021-2022", PS_TUI_OLED_HALIGN_CENTER},
    .line6 = {"@rafaelmartins", PS_TUI_OLED_HALIGN_CENTER},
};

const ps_tui_screen_t screen_splash = {
    .type = PS_TUI_SCREEN_LINES,
    .screen.lines = &screen_splash_lines,
};


// main

static void
screen_main_render_callback(ps_tui_t *tui)
{
    ps_tui_oled_clear(tui);
    ps_tui_oled_line(tui, 0, "pico-synth", PS_TUI_OLED_HALIGN_CENTER);
}

static void
screen_main_encoder_callback(ps_tui_t *tui, ps_tui_encoder_action_t act)
{
    if (act != PS_TUI_ENCODER_ACTION_BUTTON)
        return;

    ps_tui_screen_load(tui, &screen_settings);
}

static const ps_tui_screen_callback_t screen_main_callback = {
    .render_callback = screen_main_render_callback,
    .encoder_callback = screen_main_encoder_callback,
};

const ps_tui_screen_t screen_main = {
    .type = PS_TUI_SCREEN_CALLBACK,
    .screen.callback = &screen_main_callback,
};


// settings

static const ps_tui_screen_menu_t screen_settings_menu = {
    .title = "Settings",
    .items = {
        {
            .content = "Voices",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .action.next = &screen_voice_select,
            },
        },
        {
            .content = "... Back",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .action.next = &screen_main,
            },
        },
    },
    .num_items = 2,
};

const ps_tui_screen_t screen_settings = {
    .type = PS_TUI_SCREEN_MENU,
    .screen.menu = &screen_settings_menu,
};


// voice select

static void
screen_voice_select_1_cb(ps_tui_t *tui)
{
    settings_ctx_t *ctx = tui->ctx_data;
    hard_assert(ctx);

    ctx->current_voice = 0;
    ps_tui_screen_load(tui, &screen_voice);
}

static void
screen_voice_select_2_cb(ps_tui_t *tui)
{
    settings_ctx_t *ctx = tui->ctx_data;
    hard_assert(ctx);

    ctx->current_voice = 1;
    ps_tui_screen_load(tui, &screen_voice);
}

static const ps_tui_screen_menu_t screen_voice_select_menu = {
    .title = "Voices",
    .items = {
        {
            .content = "Voice 1",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_CALLBACK,
                .action.callback = screen_voice_select_1_cb,
            },
        },
        {
            .content = "Voice 2",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_CALLBACK,
                .action.callback = screen_voice_select_2_cb,
            },
        },
        {
            .content = "... Back",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .action.next = &screen_settings,
            },
        },
    },
    .num_items = 3,
};

const ps_tui_screen_t screen_voice_select = {
    .type = PS_TUI_SCREEN_MENU,
    .screen.menu = &screen_voice_select_menu,
};


// voice

static void
screen_voice_title_cb(ps_tui_t *tui, char *buf, size_t buflen)
{
    if (buflen < 7)
        return;

    memcpy(buf, "Voice  ", buflen);
    buf[6] = ((settings_ctx_t*) tui->ctx_data)->current_voice + '1';
}

static const ps_tui_screen_menu_t screen_voice_menu = {
    .title = {
        .type = PS_TUI_SCREEN_LINE_CALLBACK,
        .callback = screen_voice_title_cb,
    },
    .items = {
        {
            .content = "... Back",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .action.next = &screen_voice_select,
            },
        },
    },
    .num_items = 1,
};

const ps_tui_screen_t screen_voice = {
    .type = PS_TUI_SCREEN_MENU,
    .screen.menu = &screen_voice_menu,
};
