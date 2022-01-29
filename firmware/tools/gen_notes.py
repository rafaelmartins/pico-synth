#!/usr/bin/env python3

import math

freqs = [
    8.18,
    8.66,
    9.18,
    9.72,
    10.30,
    10.91,
    11.56,
    12.25,
    12.98,
    13.75,
    14.57,
    15.43,
    16.35,
    17.32,
    18.35,
    19.45,
    20.60,
    21.83,
    23.12,
    24.50,
    25.96,
    27.50,
    29.14,
    30.87,
    32.70,
    34.65,
    36.71,
    38.89,
    41.20,
    43.65,
    46.25,
    49.00,
    51.91,
    55.00,
    58.27,
    61.74,
    65.41,
    69.30,
    73.42,
    77.78,
    82.41,
    87.31,
    92.50,
    98.00,
    103.83,
    110.00,
    116.54,
    123.47,
    130.81,
    138.59,
    146.83,
    155.56,
    164.81,
    174.61,
    185.00,
    196.00,
    207.65,
    220.00,
    233.08,
    246.94,
    261.63,
    277.18,
    293.66,
    311.13,
    329.63,
    349.23,
    369.99,
    392.00,
    415.30,
    440.00,
    466.16,
    493.88,
    523.25,
    554.37,
    587.33,
    622.25,
    659.26,
    698.46,
    739.99,
    783.99,
    830.61,
    880.00,
    932.33,
    987.77,
    1046.50,
    1108.73,
    1174.66,
    1244.51,
    1318.51,
    1396.91,
    1479.98,
    1567.98,
    1661.22,
    1760.00,
    1864.66,
    1975.53,
    2093.00,
    2217.46,
    2349.32,
    2489.02,
    2637.02,
    2793.83,
    2959.96,
    3135.96,
    3322.44,
    3520.00,
    3729.31,
    3951.07,
    4186.01,
    4434.92,
    4698.64,
    4978.03,
    5274.04,
    5587.65,
    5919.91,
    6271.93,
    6644.88,
    7040.00,
    7458.62,
    7902.13,
    8372.02,
    8869.84,
    9397.27,
    9956.06,
    10548.08,
    11175.30,
    11839.82,
    12543.85,
]

names = [
    "C",
    "C#",
    "D",
    "D#",
    "E",
    "F",
    "F#",
    "G",
    "G#",
    "A",
    "A#",
    "B",
]

cpufreq = 133000000
cycles_per_sample = 34
samples_per_second = 96000


def note_to_string(note):
    name = names[note % 12]
    oct = (note // 12) - 1;
    return '%s%d' % (name, oct)


def get_div():
    return cpufreq / (samples_per_second * cycles_per_sample)


def get_step(f):
    return 0x2000 / (samples_per_second / f)


def generate():
    div = get_div()
    yield '#pragma once'
    yield ''
    yield '#include <note.h>'
    yield ''
    yield '#define note_clkdiv %f' % div
    yield '#define note_last %d' % (len(freqs) - 1)
    yield ''
    yield 'static const note_t notes[] = {'

    for i, f in enumerate(freqs):
        name = note_to_string(i)
        step = get_step(f)
        yield '    {'
        yield '        .id   = %d,' % i
        yield '        .name = "%s",' % name
        yield '        .step = {'
        yield '            .pint  = %d,' % int(step)
        yield '            .pfrac = %d,' % int((step - float(int(step))) * (1 << 16))
        yield '        },'
        yield '    },'

    yield '};'


if __name__ == '__main__':
    import os

    rootdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    with open(os.path.join(rootdir, 'data', 'notes.h'), 'w') as fp:
        for l in generate():
            print(l, file=fp)
