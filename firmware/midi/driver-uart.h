#pragma once

#include <pico-synth/midi.h>

void midi_uart_init(ps_midi_t *m);
uint32_t midi_uart_read(ps_midi_t *m, uint8_t *data, uint32_t data_len);
