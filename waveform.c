#include <stdint.h>
#include <waveform.h>
#include <data/waveforms.h>


uint16_t
waveform_get_sample(waveform_type_t type, uint16_t phase)
{
    if (phase > 0x2000)
        return 0;

    switch (type) {
        case WAVEFORM_SQUARE:
            return phase < 0x1000 ? 0 : 0x0fff;

        case WAVEFORM_SINE:
            return sine_table[phase];

        case WAVEFORM_TRIANGLE:
            return triangle_table[phase];

        case WAVEFORM_SAW_RIGHT:
            return saw_table[phase];

        case WAVEFORM_SAW_LEFT:
            return 0xfff - saw_table[phase];
    }

    return 0;
}
