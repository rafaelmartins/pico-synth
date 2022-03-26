#include <pico-synth/tui.h>

static const ps_tui_screen_lines_t lines = {
    .line1 = {"pico-synth", PS_TUI_OLED_HALIGN_CENTER},
    .line5 = {"(c) 2021-2022", PS_TUI_OLED_HALIGN_CENTER},
    .line6 = {"@rafaelmartins", PS_TUI_OLED_HALIGN_CENTER},
};

const ps_tui_screen_t screen_splash = {
    .type = PS_TUI_SCREEN_LINES,
    .screen.lines = &lines,
};
