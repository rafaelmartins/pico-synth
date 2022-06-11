/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <hardware/uart.h>
#include <pico/mutex.h>

typedef enum {
    PS_MIDI_MESSAGE_GROUP_CHANNEL = 1,
    PS_MIDI_MESSAGE_GROUP_SYSTEM,
    PS_MIDI_MESSAGE_GROUP_SYSTEM_RT,
    PS_MIDI_MESSAGE_GROUP_SYSEX,
} ps_midi_message_group_t;

typedef enum {
    PS_MIDI_MESSAGE_TYPE_CHANNEL_NOTE_OFF = 1,
    PS_MIDI_MESSAGE_TYPE_CHANNEL_NOTE_ON,
    PS_MIDI_MESSAGE_TYPE_CHANNEL_POLYPHONIC_PRESSURE,
    PS_MIDI_MESSAGE_TYPE_CHANNEL_CONTROL_CHANGE,
    PS_MIDI_MESSAGE_TYPE_CHANNEL_PROGRAM_CHANGE,
    PS_MIDI_MESSAGE_TYPE_CHANNEL_CHANNEL_PRESSURE,
    PS_MIDI_MESSAGE_TYPE_CHANNEL_PITCH_BEND,
} ps_midi_message_type_channel_t;

typedef enum {
    PS_MIDI_MESSAGE_TYPE_SYSTEM_TIME_CODE_QUARTER_FRAME = 1,
    PS_MIDI_MESSAGE_TYPE_SYSTEM_SONG_POSITION,
    PS_MIDI_MESSAGE_TYPE_SYSTEM_SONG_SELECT,
    PS_MIDI_MESSAGE_TYPE_SYSTEM_TUNE_REQUEST,
} ps_midi_message_type_system_t;

typedef enum {
    PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_TIMING_CLOCK = 1,
    PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_START,
    PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_CONTINUE,
    PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_STOP,
    PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_ACTIVE_SENSE,
    PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_SYSTEM_RESET,
} ps_midi_message_type_system_rt_t;

typedef enum {
    PS_MIDI_MESSAGE_TYPE_SYSEX_BEGIN = 1,
    PS_MIDI_MESSAGE_TYPE_SYSEX_DATA,
    PS_MIDI_MESSAGE_TYPE_SYSEX_END,
} ps_midi_message_type_sysex_t;

typedef struct {
    ps_midi_message_type_channel_t type;
    uint8_t channel;
    uint8_t data_len;
    uint8_t data[2];
} ps_midi_message_channel_t;

typedef struct {
    ps_midi_message_type_system_t type;
    uint8_t data_len;
    uint8_t data[2];
} ps_midi_message_system_t;

typedef struct {
    ps_midi_message_type_system_rt_t type;
} ps_midi_message_system_rt_t;

typedef struct {
    ps_midi_message_type_sysex_t type;
    uint8_t data;
} ps_midi_message_sysex_t;

typedef struct {
    ps_midi_message_group_t group;
    union {
        ps_midi_message_channel_t channel;
        ps_midi_message_system_t system;
        ps_midi_message_system_rt_t system_rt;
        ps_midi_message_sysex_t sysex;
    };
} ps_midi_message_t;

typedef struct {
    struct {
        uart_inst_t *uart;
        uint uart_rx;
    } uart;

    struct {
        bool enable;
        const char *product;
        const char *manufacturer;
        uint16_t pid;
        uint16_t vid;
        uint16_t version;
    } usb;

    // private
    mutex_t _mtx;
    bool _sysex;
    ps_midi_message_type_channel_t _channel_type;
    ps_midi_message_type_system_t _system_type;
    uint8_t _channel;
    uint8_t _data[2];
    uint8_t _data_len;
    uint8_t _data_idx;
} ps_midi_t;

void ps_midi_init(ps_midi_t *m);
void ps_midi_task(ps_midi_t *m);
void ps_midi_message_cb(const ps_midi_message_t *msg) __attribute__((weak));
