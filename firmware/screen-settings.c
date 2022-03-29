#include <pico-synth/tui.h>

extern ps_tui_screen_t screen_main;
extern ps_tui_screen_t screen_settings_voices;

static const ps_tui_screen_menu_t menu = {
    .title = "Settings",
    .items = {
        {
            .content = "Voices",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .action.next = &screen_settings_voices,
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
    .screen.menu = &menu,
};
