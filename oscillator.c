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
    oscillator_set_waveform(osc, WAVEFORM_SINE); // FIXME
}


void
oscillator_set_waveform(oscillator_t *osc, waveform_type_t wf)
{
    hard_assert(osc);

    mutex_enter_blocking(&osc->mtx);
    osc->wf = wf;
    mutex_exit(&osc->mtx);
}


void
oscillator_note_on(oscillator_t *osc, uint8_t note)
{
    hard_assert(osc);

    mutex_enter_blocking(&osc->mtx);
    osc->note = note_get(note);
    mutex_exit(&osc->mtx);
}


void
oscillator_note_off(oscillator_t *osc)
{
    hard_assert(osc);

    mutex_enter_blocking(&osc->mtx);
    osc->note = NULL;
    mutex_exit(&osc->mtx);
}


uint16_t
oscillator_sample_callback(void *data)
{
    if (data == NULL)
        return 0;

    oscillator_t *osc = data;

    mutex_enter_blocking(&osc->mtx);
    uint16_t sample = waveform_get_sample(osc->wf, phase_get_index(&osc->phase));
    if (osc->note != NULL)
        phase_step(&osc->phase, &osc->note->step);
    mutex_exit(&osc->mtx);

    return sample;
}
