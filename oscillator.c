#include <mcp4822.h>
#include <note.h>
#include <waveform.h>
#include <oscillator.h>


void
oscillator_init(oscillator_t *osc)
{
    hard_assert(osc);
    hard_assert(osc->dac);

    osc->phase = 0;
    osc->on = false;
    osc->to_on = false;
    osc->to_off = false;
    osc->note = NULL;

    oscillator_set_sample_callbacks(osc, NULL, NULL);
    oscillator_set_waveforms(osc, WAVEFORM_SINE, WAVEFORM_SQUARE); // FIXME
}


void
oscillator_set_sample_callbacks(oscillator_t *osc,
    oscillator_sample_filter_cb_t *cb1, oscillator_sample_filter_cb_t *cb2)
{
    hard_assert(osc);

    // FIXME: wait for phase==0 to change?

    osc->cb[0] = cb1;
    osc->cb[1] = cb2;
}


void
oscillator_set_waveforms(oscillator_t *osc, waveform_type_t wf1, waveform_type_t wf2)
{
    hard_assert(osc);

    // FIXME: wait for phase==0 to change?

    osc->wf[0] = wf1;
    osc->wf[1] = wf2;
}


void
oscillator_note_on(oscillator_t *osc, uint8_t note)
{
    hard_assert(osc);

    osc->note = note_get(note);
    osc->to_on = osc->note != NULL;
}


void
oscillator_note_off(oscillator_t *osc)
{
    hard_assert(osc);

    osc->to_off = true;
}


void
oscillator_update(oscillator_t *osc)
{
    hard_assert(osc);

    if (osc->phase > 0x2000 || (osc->note != NULL && osc->phase > osc->note->end))
        osc->phase = 0;

    if (osc->to_off) {
        if (osc->phase == 0) {
            if (mcp4822_unset(osc->dac)) {
                osc->phase = 0;
                osc->on = false;
                osc->to_off = false;
            }
        }
        else if (osc->to_on) {
            osc->on = false;
            osc->to_off = false;
        }
    }

    if (!(osc->on || osc->to_on))
        return;

    uint16_t s1 = waveform_get_sample(osc->wf[0], osc->phase);
    uint16_t s2 = waveform_get_sample(osc->wf[1], osc->phase);

    if (osc->cb[0] != NULL)
        s1 = osc->cb[0](osc->id, 0, osc->phase, s1);

    if (osc->cb[1] != NULL)
        s2 = osc->cb[1](osc->id, 1, osc->phase, s2);

    if (osc->note != NULL && mcp4822_set(osc->dac, s1, s2)) {
        if (osc->to_on) {
            mcp4822_set_clkdiv(osc->dac, osc->note->div_int, osc->note->div_frac);
            osc->on = true;
            osc->to_on = false;
        }
        osc->phase += osc->note->step;
    }
}
