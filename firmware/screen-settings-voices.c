#include <string.h>
#include <pico-synth/tui.h>
#include "settings.h"

extern const ps_tui_screen_t screen_settings;
const ps_tui_screen_t screen_settings_voices;

static void
voice_title_cb(ps_tui_t *tui, char *buf, size_t buflen)
{
    memcpy(buf, "Voice  ", buflen);
    buf[6] = ((settings_ctx_t*) tui->ctx_data)->current_voice + '1';
}

static const ps_tui_screen_menu_t menu_voice = {
    .title = {
        .type = PS_TUI_SCREEN_LINE_CALLBACK,
        .callback = voice_title_cb,
    },
    .items = {
        {
            .content = "... Back",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .action.next = &screen_settings_voices,
            },
        },
    },
    .num_items = 1,
};

const ps_tui_screen_t screen_settings_voice = {
    .type = PS_TUI_SCREEN_MENU,
    .screen.menu = &menu_voice,
};


static void
voice1_select_cb(ps_tui_t *tui)
{
    settings_ctx_t *ctx = tui->ctx_data;
    hard_assert(ctx);

    ctx->current_voice = 0;
    ps_tui_screen_load(tui, &screen_settings_voice);
}

static void
voice2_select_cb(ps_tui_t *tui)
{
    settings_ctx_t *ctx = tui->ctx_data;
    hard_assert(ctx);

    ctx->current_voice = 1;
    ps_tui_screen_load(tui, &screen_settings_voice);
}

static const ps_tui_screen_menu_t menu_voices = {
    .title = "Voices",
    .items = {
        {
            .content = "Voice 1",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_CALLBACK,
                .action.callback = voice1_select_cb,
            },
        },
        {
            .content = "Voice 2",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_CALLBACK,
                .action.callback = voice2_select_cb,
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

const ps_tui_screen_t screen_settings_voices = {
    .type = PS_TUI_SCREEN_MENU,
    .screen.menu = &menu_voices,
};
