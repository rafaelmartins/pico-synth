#include <stdint.h>
#include <waveform.h>
#include <data/waveforms.h>


static inline uint16_t
get_sample(waveform_type_t type, uint16_t phase)
{
    if (phase >= waveform_samples)
        return 0;

    switch (type) {
        case WAVEFORM_SQUARE:
            return phase < (waveform_samples / 2) ? waveform_amplitude_min
                : waveform_amplitude_max;

        case WAVEFORM_SINE:
            return sine_table[phase];

        case WAVEFORM_TRIANGLE:
            return triangle_table[phase];

        case WAVEFORM_SAW_RIGHT:
            return saw_table[phase];

        case WAVEFORM_SAW_LEFT:
            return waveform_amplitude_max - saw_table[phase];
    }

    return 0;
}


int16_t
waveform_get_sample(waveform_type_t type, uint16_t phase)
{
    return get_sample(type, phase) - waveform_amplitude_offset;
}
