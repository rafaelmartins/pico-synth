#pragma once

#include <pico/mutex.h>

typedef enum {
    MIDI_COMMAND_UNSET = 0,
    MIDI_COMMAND_NOTE_OFF = 0x08,
    MIDI_COMMAND_NOTE_ON,
    MIDI_COMMAND_POLYPHONIC_PRESSURE,
    MIDI_COMMAND_CONTROL_CHANGE,
    MIDI_COMMAND_PROGRAM_CHANGE,
    MIDI_COMMAND_CHANNEL_PRESSURE,
    MIDI_COMMAND_PITCH_BEND,
    MIDI_COMMAND_SYSTEM,
} midi_command_type_t;

typedef enum {
    MIDI_COMMAND_SYSTEM_SYSEX1,
    MIDI_COMMAND_SYSTEM_TIME_CODE_QUARTER_FRAME,
    MIDI_COMMAND_SYSTEM_SONG_POSITION,
    MIDI_COMMAND_SYSTEM_SONG_SELECT,
    MIDI_COMMAND_SYSTEM_UNDEFINED1,
    MIDI_COMMAND_SYSTEM_UNDEFINED2,
    MIDI_COMMAND_SYSTEM_TUNE_REQUEST,
    MIDI_COMMAND_SYSTEM_SYSEX2,
    MIDI_COMMAND_SYSTEM_RT_TIMING_CLOCK,
    MIDI_COMMAND_SYSTEM_RT_UNDEFINED1,
    MIDI_COMMAND_SYSTEM_RT_START,
    MIDI_COMMAND_SYSTEM_RT_CONTINUE,
    MIDI_COMMAND_SYSTEM_RT_STOP,
    MIDI_COMMAND_SYSTEM_RT_UNDEFINED2,
    MIDI_COMMAND_SYSTEM_RT_ACTIVE_SENSE,
    MIDI_COMMAND_SYSTEM_RT_SYSTEM_RESET,
} midi_command_system_type_t;

typedef void (*midi_channel_cb_t)(midi_command_type_t cmd, uint8_t channel,
    uint8_t *data, uint8_t data_len);
typedef void (*midi_system_cb_t)(midi_command_system_type_t cmd, uint8_t *data,
    uint8_t data_len);

typedef struct {
    // private
    mutex_t mtx;
    midi_channel_cb_t cb_channel;
    midi_system_cb_t cb_system;
    uint8_t last_status;
    uint8_t data[2];
    uint8_t data_len;
    uint8_t data_idx;
} midi_t;

void midi_init(midi_t *m);
void midi_set_channel_callback(midi_t *m, midi_channel_cb_t cb);
void midi_set_system_callback(midi_t *m, midi_system_cb_t cb);
void midi_push_byte(midi_t *m, uint8_t b);
