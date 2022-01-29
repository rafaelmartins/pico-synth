#include <stddef.h>
#include <pico/assert.h>
#include <phase.h>
#include <data/waveforms.h>

void
phase_reset(phase_t *p)
{
    hard_assert(p);

    p->pint = 0;
    p->pfrac = 0;
}


uint16_t
phase_step(phase_t *p, const phase_t *s)
{
    hard_assert(p);

    if (s == NULL)
        return 0;

    uint16_t pint = p->pint;
    uint16_t pfrac = p->pfrac;
    p->pint += s->pint;
    p->pfrac += s->pfrac;
    if (p->pfrac < pfrac)
        p->pint++;
    if (p->pint >= waveform_samples)
        p->pint -= waveform_samples;

    return pint;
}
