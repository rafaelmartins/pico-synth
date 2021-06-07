#pragma once

#include <pico/mutex.h>
#include <note.h>
#include <phase.h>
#include <waveform.h>

typedef struct {
    uint8_t id;
    mutex_t mtx;
    phase_t phase;
    const note_t *note;
    waveform_type_t wf;
} oscillator_t;

void oscillator_init(oscillator_t *osc);
void oscillator_set_waveform(oscillator_t *osc, waveform_type_t wf);
void oscillator_note_on(oscillator_t *osc, uint8_t note);
void oscillator_note_off(oscillator_t *osc);
int16_t oscillator_sample_callback(void *data);
