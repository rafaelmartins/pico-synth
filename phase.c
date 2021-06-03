#include <pico/assert.h>
#include <phase.h>


void
phase_reset(phase_t *p)
{
    hard_assert(p);

    p->pint = 0;
    p->pfrac = 0;
}


void
phase_step(phase_t *p, const phase_t *s)
{
    hard_assert(p);
    hard_assert(s);

    uint16_t pfrac = p->pfrac;
    p->pint += s->pint;
    p->pfrac += s->pfrac;
    if (p->pfrac < pfrac)
        p->pint++;
    if (p->pint >= PHASE_MAX)
        p->pint -= PHASE_MAX;
}


uint16_t
phase_get_index(phase_t *p)
{
    hard_assert(p);

    return p->pint;
}
