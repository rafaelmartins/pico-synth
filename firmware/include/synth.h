#pragma once

#include <hardware/uart.h>
#include <pico/util/queue.h>
#include <ec11.h>
#include <eeprom-i2c.h>
#include <mcp4822.h>
#include <midi.h>
#include <oled-tui.h>
#include <oscillator.h>

#ifndef SYNTH_NUM_OSCILLATORS
#define SYNTH_NUM_OSCILLATORS 6
#endif

#ifndef SYNTH_QUEUE_SIZE
#define SYNTH_QUEUE_SIZE 64
#endif

typedef struct {
    mcp4822_t dac;
    eeprom_i2c_t eeprom;
    ec11_t encoder;
    oled_tui_t tui;

    struct {
        mcp4822_dac_t channel;
        uint8_t midi_channel;
        oscillator_t oscillator;
    } oscillators[SYNTH_NUM_OSCILLATORS];

    struct {
        uart_inst_t *uart;
        midi_t midi_uart;
        midi_t midi_usb;
    } midi;

    queue_t queues[2];
} synth_t;

int synth_init(synth_t *s);
void synth_core1(void);
void synth_task(void);
