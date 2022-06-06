/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

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
