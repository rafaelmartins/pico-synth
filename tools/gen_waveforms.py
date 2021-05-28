#!/usr/bin/env python3

import math

waveforms = {
    'sine': [int(0x0800 + (0x07ff * math.sin(2 * math.pi * i / 0x2000)))
             for i in range(0x2000)],
    'saw': [i * 0x0fff // 0x1ffe for i in range(0x2000)],
    'triangle': [0x0800 + i if i < 0x0800 else 0x17ff - i if i < 0x1800 else i - 0x1800
                 for i in range(0x2000)],
}


def generate():
    yield '#pragma once'
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
