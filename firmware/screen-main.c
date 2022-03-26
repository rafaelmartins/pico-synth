#include <pico-synth/tui.h>

static void
render_callback(ps_tui_t *tui)
{
    ps_tui_oled_clear(tui);
    ps_tui_oled_line(tui, 0, "pico-synth", PS_TUI_OLED_HALIGN_CENTER);
}


static void
encoder_callback(ps_tui_t *tui, ps_tui_encoder_action_t act)
{
    if (act != PS_TUI_ENCODER_ACTION_BUTTON)
        return;

    extern const ps_tui_screen_t screen_settings;
    ps_tui_screen_load(tui, &screen_settings);
}


static ps_tui_screen_callback_t callback = {
    .render_callback = render_callback,
    .encoder_callback = encoder_callback,
};

const ps_tui_screen_t screen_main = {
    .type = PS_TUI_SCREEN_CALLBACK,
    .screen.callback = &callback,
};
