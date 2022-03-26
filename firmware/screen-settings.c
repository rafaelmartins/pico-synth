#include <pico-synth/tui.h>

extern const ps_tui_screen_t screen_main;

static const ps_tui_screen_menu_t menu = {
    .title = "Settings",
    .items = {
        {
            .content = "Oscillators",
            .action = {
                .type = PS_TUI_SCREEN_ACTION_NEXT,
                .action.next = NULL,
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
