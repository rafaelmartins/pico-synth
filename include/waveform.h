#pragma once

#include <stdint.h>

typedef enum {
    WAVEFORM_SQUARE = 1,
    WAVEFORM_SINE,
    WAVEFORM_TRIANGLE,
    WAVEFORM_SAW_RIGHT,
    WAVEFORM_SAW_LEFT,
} waveform_type_t;

uint16_t waveform_get_sample(waveform_type_t type, uint16_t phase);
