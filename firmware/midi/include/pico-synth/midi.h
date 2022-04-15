#pragma once

#include <hardware/uart.h>
#include <pico/mutex.h>

typedef enum {
    PS_MIDI_COMMAND_UNSET = 0,
    PS_MIDI_COMMAND_NOTE_OFF = 0x08,
    PS_MIDI_COMMAND_NOTE_ON,
    PS_MIDI_COMMAND_POLYPHONIC_PRESSURE,
    PS_MIDI_COMMAND_CONTROL_CHANGE,
    PS_MIDI_COMMAND_PROGRAM_CHANGE,
    PS_MIDI_COMMAND_CHANNEL_PRESSURE,
    PS_MIDI_COMMAND_PITCH_BEND,
    PS_MIDI_COMMAND_SYSTEM,
} ps_midi_command_type_t;

typedef enum {
    PS_MIDI_COMMAND_SYSTEM_SYSEX1,
    PS_MIDI_COMMAND_SYSTEM_TIME_CODE_QUARTER_FRAME,
    PS_MIDI_COMMAND_SYSTEM_SONG_POSITION,
    PS_MIDI_COMMAND_SYSTEM_SONG_SELECT,
    PS_MIDI_COMMAND_SYSTEM_UNDEFINED1,
    PS_MIDI_COMMAND_SYSTEM_UNDEFINED2,
    PS_MIDI_COMMAND_SYSTEM_TUNE_REQUEST,
    PS_MIDI_COMMAND_SYSTEM_SYSEX2,
    PS_MIDI_COMMAND_SYSTEM_RT_TIMING_CLOCK,
    PS_MIDI_COMMAND_SYSTEM_RT_UNDEFINED1,
    PS_MIDI_COMMAND_SYSTEM_RT_START,
    PS_MIDI_COMMAND_SYSTEM_RT_CONTINUE,
    PS_MIDI_COMMAND_SYSTEM_RT_STOP,
    PS_MIDI_COMMAND_SYSTEM_RT_UNDEFINED2,
    PS_MIDI_COMMAND_SYSTEM_RT_ACTIVE_SENSE,
    PS_MIDI_COMMAND_SYSTEM_RT_SYSTEM_RESET,
} ps_midi_command_system_type_t;

typedef struct {
    uart_inst_t *uart;
    uint uart_rx;
    bool with_usb;

    // private
    mutex_t _mtx;
    uint8_t _last_status;
    uint8_t _data[2];
    uint8_t _data_len;
    uint8_t _data_idx;
} ps_midi_t;

void ps_midi_init(ps_midi_t *m);
void ps_midi_task(ps_midi_t *m);
void ps_midi_channel_cb(ps_midi_command_type_t cmd, uint8_t channel, uint8_t *data, uint8_t data_len) __attribute__((weak));
void ps_midi_system_cb(ps_midi_command_system_type_t cmd, uint8_t *data, uint8_t data_len) __attribute__((weak));
