#pragma once

#include <oled-tui.h>
#include <oscillator.h>

void controller_init(oled_tui_t *t, oscillator_t *osc1, oscillator_t *osc2,
    oscillator_t *osc3);
void controller_midi_task(void);
uint16_t controller_usb_request_cb(uint8_t cmd, bool write, uint16_t val,
    uint16_t idx, uint8_t *buf);
