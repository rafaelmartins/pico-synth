#include <pico/mutex.h>
#include <note.h>
#include <phase.h>
#include <waveform.h>
#include <oscillator.h>


void
oscillator_init(oscillator_t *osc)
{
    hard_assert(osc);

    mutex_init(&osc->mtx);
    phase_reset(&osc->phase);
    osc->note = NULL;
    oscillator_set_waveform(osc, WAVEFORM_SINE);
}


void
oscillator_set_waveform(oscillator_t *osc, waveform_type_t wf)
{
    hard_assert(osc);

    mutex_enter_blocking(&osc->mtx);
    osc->wf = wf;
    mutex_exit(&osc->mtx);
}


waveform_type_t
oscillator_get_waveform(oscillator_t *osc)
{
    hard_assert(osc);

    mutex_enter_blocking(&osc->mtx);
    waveform_type_t rv = osc->wf;
    mutex_exit(&osc->mtx);

    return rv;
}


bool
oscillator_note_on(oscillator_t *osc, uint8_t note)
{
    hard_assert(osc);

    mutex_enter_blocking(&osc->mtx);
    bool rv = osc->note == NULL;
    if (rv) {
        osc->note = note_get(note);
        phase_reset(&osc->phase);
    }
    mutex_exit(&osc->mtx);

    return rv;
}


bool
oscillator_note_off(oscillator_t *osc, uint8_t note)
{
    hard_assert(osc);

    mutex_enter_blocking(&osc->mtx);
    bool rv = osc->note != NULL && osc->note->id == note;
    if (rv)
        osc->note = NULL;
    mutex_exit(&osc->mtx);

    return rv;
}


int16_t
oscillator_next_sample(oscillator_t *osc)
{
    hard_assert(osc);

    mutex_enter_blocking(&osc->mtx);
    int16_t sample = 0;
    if (osc->note != NULL)
        sample = waveform_get_sample(osc->wf, phase_step(&osc->phase, &osc->note->step));
    mutex_exit(&osc->mtx);

    return sample;
}
