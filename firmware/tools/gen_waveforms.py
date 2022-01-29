#!/usr/bin/env python3

import math

amp_min = 0
amp_off = 0x0800
amp_max = 0x0fff
samples = 0x2000

waveforms = {
    'sine': [int(amp_off + ((amp_off - 1) * math.sin(2 * math.pi * i / samples)))
             for i in range(samples)],
    'saw': [i * amp_max // (samples - 2) for i in range(samples)],
    'triangle': [amp_off + i if i < amp_off else
                 (3 * amp_off) - 1 - i if i < (3 * amp_off) else
                 i - (3 * amp_off)
                 for i in range(samples)],
}


def generate():
    yield '#pragma once'
    yield ''
    yield '#define waveform_amplitude_min 0x%04x' % amp_min
    yield '#define waveform_amplitude_offset 0x%04x' % amp_off
    yield '#define waveform_amplitude_max 0x%04x' % amp_max
    yield '#define waveform_samples 0x%04x' % samples

    for name, l in waveforms.items():
        yield ''
        yield 'static const uint16_t %s_table[] = {' % name
        for i in range(0, len(l) // 8):
            yield '    %s,' % ', '.join(['0x%03x' % j for j in l[i * 8: (i + 1) * 8]])
        yield '};'


if __name__ == '__main__':
    import os

    rootdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    with open(os.path.join(rootdir, 'data', 'waveforms.h'), 'w') as fp:
        for l in generate():
            print(l, file=fp)
