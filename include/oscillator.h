#pragma once

#include <mcp4822.h>
#include <note.h>
#include <waveform.h>

typedef uint16_t (oscillator_sample_filter_cb_t)(uint8_t osc_id, uint8_t id,
    uint16_t phase, uint16_t sample);

typedef struct {
    uint8_t id;
    mcp4822_t *dac;
    uint16_t phase;
    bool on;
    bool to_on;
    bool to_off;
    const note_t *note;
    waveform_type_t wf[2];
    oscillator_sample_filter_cb_t *cb[2];
} oscillator_t;

void oscillator_init(oscillator_t *osc);
void oscillator_set_sample_callbacks(oscillator_t *osc,
    oscillator_sample_filter_cb_t *cb1, oscillator_sample_filter_cb_t *cb2);
void oscillator_set_waveforms(oscillator_t *osc, waveform_type_t wf1, waveform_type_t wf2);
void oscillator_note_on(oscillator_t *osc, uint8_t note);
void oscillator_note_off(oscillator_t *osc);
void oscillator_update(oscillator_t *osc);
