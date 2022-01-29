#include <stddef.h>
#include <note.h>
#include <data/notes.h>


const note_t*
note_get(uint8_t note)
{
    if (note > note_last)
        return NULL;
    return &notes[note];
}
