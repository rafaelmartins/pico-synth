#pragma once

#include <pico-synth/tui.h>

void ec11_init(ps_tui_t *tui);
void ec11_task(ps_tui_t *tui);
void ec11_callback(ps_tui_t *tui, ps_tui_encoder_action_t act) __attribute__((weak));
