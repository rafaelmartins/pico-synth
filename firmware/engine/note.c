#include <pico-synth/engine.h>
#include "note-data.h"


const ps_engine_note_t*
ps_engine_get_note(uint8_t note)
{
    if (note > notes_last)
        return NULL;

    return &notes[note];
}


const char*
ps_engine_get_note_name(uint8_t note)
{
    const ps_engine_note_t *n = ps_engine_get_note(note);
    if (n == NULL)
        return NULL;

    return n->name;
}
