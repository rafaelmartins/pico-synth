/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <pico-synth/midi.h>
#include "driver-uart.h"
#include "driver-usb.h"

// FIXME: solve conflicts when both USB and UART send data at the same time

typedef enum {
    NOTE_OFF = 0x8,
    NOTE_ON,
    POLYPHONIC_PRESSURE,
    CONTROL_CHANGE,
    PROGRAM_CHANGE,
    CHANNEL_PRESSURE,
    PITCH_BEND,
    SYSTEM,
} midi_command_t;

typedef enum {
    SYSTEM_SYSEX1,
    SYSTEM_TIME_CODE_QUARTER_FRAME,
    SYSTEM_SONG_POSITION,
    SYSTEM_SONG_SELECT,
    SYSTEM_UNDEFINED1,
    SYSTEM_UNDEFINED2,
    SYSTEM_TUNE_REQUEST,
    SYSTEM_SYSEX2,
    SYSTEM_RT_TIMING_CLOCK,
    SYSTEM_RT_UNDEFINED1,
    SYSTEM_RT_START,
    SYSTEM_RT_CONTINUE,
    SYSTEM_RT_STOP,
    SYSTEM_RT_UNDEFINED2,
    SYSTEM_RT_ACTIVE_SENSE,
    SYSTEM_RT_SYSTEM_RESET,
} midi_system_subcommand_t;


void
ps_midi_init(ps_midi_t *m)
{
    hard_assert(m);

    mutex_init(&m->_mtx);
    m->_sysex = false;
    m->_channel = 0;
    m->_system_type = 0;
    m->_channel_type = 0;
    memset(m->_data, 0, sizeof(m->_data));
    m->_data_len = 0;
    m->_data_idx = 0;

    midi_uart_init(m);
    midi_usb_init(m);
}


static inline bool
is_data(uint8_t status)
{
    return status < 0x80;
}


static inline uint8_t
parse_status(uint8_t status, ps_midi_message_group_t *group, uint8_t *type)
{
    *type = 0;
    *group = 0;

    if (is_data(status))
        return 0;

    switch ((midi_command_t) (status >> 4)) {
    case NOTE_OFF:
        *type = PS_MIDI_MESSAGE_TYPE_CHANNEL_NOTE_OFF;
        *group = PS_MIDI_MESSAGE_GROUP_CHANNEL;
        return 2;

    case NOTE_ON:
        *type = PS_MIDI_MESSAGE_TYPE_CHANNEL_NOTE_ON;
        *group = PS_MIDI_MESSAGE_GROUP_CHANNEL;
        return 2;

    case POLYPHONIC_PRESSURE:
        *type = PS_MIDI_MESSAGE_TYPE_CHANNEL_POLYPHONIC_PRESSURE;
        *group = PS_MIDI_MESSAGE_GROUP_CHANNEL;
        return 2;

    case CONTROL_CHANGE:
        *type = PS_MIDI_MESSAGE_TYPE_CHANNEL_CONTROL_CHANGE;
        *group = PS_MIDI_MESSAGE_GROUP_CHANNEL;
        return 2;

    case PROGRAM_CHANGE:
        *type = PS_MIDI_MESSAGE_TYPE_CHANNEL_PROGRAM_CHANGE;
        *group = PS_MIDI_MESSAGE_GROUP_CHANNEL;
        return 1;

    case CHANNEL_PRESSURE:
        *type = PS_MIDI_MESSAGE_TYPE_CHANNEL_CHANNEL_PRESSURE;
        *group = PS_MIDI_MESSAGE_GROUP_CHANNEL;
        return 1;

    case PITCH_BEND:
        *type = PS_MIDI_MESSAGE_TYPE_CHANNEL_PITCH_BEND;
        *group = PS_MIDI_MESSAGE_GROUP_CHANNEL;
        return 2;

    case SYSTEM:
        switch ((midi_system_subcommand_t) (status & 0xf)) {
        case SYSTEM_SYSEX1:
            *type = PS_MIDI_MESSAGE_TYPE_SYSEX_BEGIN;
            *group = PS_MIDI_MESSAGE_GROUP_SYSEX;
            return 0;

        case SYSTEM_TIME_CODE_QUARTER_FRAME:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_TIME_CODE_QUARTER_FRAME;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM;
            return 1;

        case SYSTEM_SONG_POSITION:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_SONG_POSITION;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM;
            return 2;

        case SYSTEM_SONG_SELECT:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_SONG_SELECT;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM;
            return 1;

        case SYSTEM_UNDEFINED1:
        case SYSTEM_UNDEFINED2:
            return 0;

        case SYSTEM_TUNE_REQUEST:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_TUNE_REQUEST;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM;
            return 0;

        case SYSTEM_SYSEX2:
            *type = PS_MIDI_MESSAGE_TYPE_SYSEX_END;
            *group = PS_MIDI_MESSAGE_GROUP_SYSEX;
            return 0;

        case SYSTEM_RT_TIMING_CLOCK:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_TIMING_CLOCK;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM_RT;
            return 0;

        case SYSTEM_RT_UNDEFINED1:
            return 0;

        case SYSTEM_RT_START:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_START;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM_RT;
            return 0;

        case SYSTEM_RT_CONTINUE:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_CONTINUE;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM_RT;
            return 0;

        case SYSTEM_RT_STOP:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_STOP;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM_RT;
            return 0;

        case SYSTEM_RT_UNDEFINED2:
            return 0;

        case SYSTEM_RT_ACTIVE_SENSE:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_ACTIVE_SENSE;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM_RT;
            return 0;

        case SYSTEM_RT_SYSTEM_RESET:
            *type = PS_MIDI_MESSAGE_TYPE_SYSTEM_RT_SYSTEM_RESET;
            *group = PS_MIDI_MESSAGE_GROUP_SYSTEM_RT;
            return 0;
        }
    }

    return 0;
}


static void
call_channel(ps_midi_message_type_channel_t type, uint8_t channel, const uint8_t *data, uint8_t data_len, void *ctx_data)
{
    if (ps_midi_message_cb == NULL)
        return;

    ps_midi_message_t msg = {
        .group = PS_MIDI_MESSAGE_GROUP_CHANNEL,
        .channel = {
            .type = type,
            .channel = channel,
            .data_len = data_len,
        },
    };
    memcpy(msg.channel.data, data, data_len);

    ps_midi_message_cb(&msg, ctx_data);
}


static void
call_system(ps_midi_message_type_system_t type, const uint8_t *data, uint8_t data_len, void *ctx_data)
{
    if (ps_midi_message_cb == NULL)
        return;

    ps_midi_message_t msg = {
        .group = PS_MIDI_MESSAGE_GROUP_SYSTEM,
        .system = {
            .type = type,
            .data_len = data_len,
        },
    };
    memcpy(msg.system.data, data, data_len);

    ps_midi_message_cb(&msg, ctx_data);
}


static void
call_system_rt(ps_midi_message_type_system_rt_t type, void *ctx_data)
{
    if (ps_midi_message_cb == NULL)
        return;

    const ps_midi_message_t msg = {
        .group = PS_MIDI_MESSAGE_GROUP_SYSTEM_RT,
        .system_rt = {
            .type = type,
        },
    };

    ps_midi_message_cb(&msg, ctx_data);
}


static void
call_sysex(ps_midi_message_type_sysex_t type, uint8_t data, void *ctx_data)
{
    if (ps_midi_message_cb == NULL)
        return;

    const ps_midi_message_t msg = {
        .group = PS_MIDI_MESSAGE_GROUP_SYSEX,
        .sysex = {
            .type = type,
            .data = data,
        },
    };

    ps_midi_message_cb(&msg, ctx_data);
}


static void
push_byte(ps_midi_t *m, uint8_t b)
{
    if (is_data(b)) {
        if (m->_sysex) {
            call_sysex(PS_MIDI_MESSAGE_TYPE_SYSEX_DATA, b, m->ctx_data);
            return;
        }

        if (m->_channel_type == 0 && m->_system_type == 0)
            return;

        if (m->_data_len < 0)
            return;

        m->_data[m->_data_idx++] = b;
        if (m->_data_idx == m->_data_len) {
            if (m->_channel_type != 0)
                call_channel(m->_channel_type, m->_channel, m->_data, m->_data_len, m->ctx_data);
            else
                call_system(m->_system_type, m->_data, m->_data_len, m->ctx_data);
        }
        return;
    }

    ps_midi_message_group_t group;
    uint8_t type;
    uint8_t len = parse_status(b, &group, &type);
    if (group == 0 || type == 0)
        return;

    m->_channel = 0;
    m->_system_type = 0;
    m->_channel_type = 0;
    m->_data_len = 0;
    m->_data_idx = 0;

    switch (group) {
    case PS_MIDI_MESSAGE_GROUP_CHANNEL:
        m->_channel_type = type;
        m->_channel = b & 0xf;
    case PS_MIDI_MESSAGE_GROUP_SYSTEM:
        if (m->_channel_type == 0)
            m->_system_type = type;
        memset(m->_data, 0, sizeof(m->_data));
        m->_data_len = len;
        m->_data_idx = 0;
        break;

    case PS_MIDI_MESSAGE_GROUP_SYSTEM_RT:
        call_system_rt(type, m->ctx_data);
        break;

    case PS_MIDI_MESSAGE_GROUP_SYSEX:
        switch ((ps_midi_message_type_sysex_t) type) {
        case PS_MIDI_MESSAGE_TYPE_SYSEX_BEGIN:
            m->_sysex = true;
            call_sysex(type, 0, m->ctx_data);
            break;
        case PS_MIDI_MESSAGE_TYPE_SYSEX_END:
            m->_sysex = false;
            call_sysex(type, 0, m->ctx_data);
            break;
        case PS_MIDI_MESSAGE_TYPE_SYSEX_DATA:
            break;
        }
        break;
    }
}


static inline void
push_byte_array(ps_midi_t *m, uint8_t *data, uint32_t data_len)
{
    for (uint32_t i = 0; i < data_len; i++)
        push_byte(m, data[i]);
}


void
ps_midi_task(ps_midi_t *m)
{
    hard_assert(m);

    static uint8_t midi_buf[1024];

    push_byte_array(m, midi_buf, midi_uart_read(m, midi_buf, sizeof(midi_buf)));
    push_byte_array(m, midi_buf, midi_usb_read(m, midi_buf, sizeof(midi_buf)));
}
