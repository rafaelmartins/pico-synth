#include <pico/mutex.h>
#include <string.h>
#include <midi.h>


void
midi_init(midi_t *m)
{
    hard_assert(m);

    mutex_init(&m->mtx);
    m->cb_channel = NULL;
    m->cb_system = NULL;
    m->last_status = 0;
    memset(m->data, 0, sizeof(m->data));
    m->data_len = 0;
    m->data_idx = 0;
}


void
midi_set_channel_callback(midi_t *m, midi_channel_cb_t cb)
{
    mutex_enter_blocking(&m->mtx);
    m->cb_channel = cb;
    mutex_exit(&m->mtx);
}


void
midi_set_system_callback(midi_t *m, midi_system_cb_t cb)
{
    mutex_enter_blocking(&m->mtx);
    m->cb_system = cb;
    mutex_exit(&m->mtx);
}


static inline int8_t
get_data_len(uint8_t b)
{
    switch ((midi_command_type_t) (b >> 4)) {
        case MIDI_COMMAND_PROGRAM_CHANGE:
        case MIDI_COMMAND_CHANNEL_PRESSURE:
            return 1;
        case MIDI_COMMAND_NOTE_OFF:
        case MIDI_COMMAND_NOTE_ON:
        case MIDI_COMMAND_POLYPHONIC_PRESSURE:
        case MIDI_COMMAND_CONTROL_CHANGE:
        case MIDI_COMMAND_PITCH_BEND:
            return 2;
        case MIDI_COMMAND_SYSTEM:
            switch ((midi_command_system_type_t) (b & 0xf)) {
                case MIDI_COMMAND_SYSTEM_SYSEX1:
                    return -1;
                case MIDI_COMMAND_SYSTEM_UNDEFINED1:
                case MIDI_COMMAND_SYSTEM_UNDEFINED2:
                case MIDI_COMMAND_SYSTEM_TUNE_REQUEST:
                case MIDI_COMMAND_SYSTEM_SYSEX2:
                case MIDI_COMMAND_SYSTEM_RT_TIMING_CLOCK:
                case MIDI_COMMAND_SYSTEM_RT_UNDEFINED1:
                case MIDI_COMMAND_SYSTEM_RT_START:
                case MIDI_COMMAND_SYSTEM_RT_CONTINUE:
                case MIDI_COMMAND_SYSTEM_RT_STOP:
                case MIDI_COMMAND_SYSTEM_RT_UNDEFINED2:
                case MIDI_COMMAND_SYSTEM_RT_ACTIVE_SENSE:
                case MIDI_COMMAND_SYSTEM_RT_SYSTEM_RESET:
                    return 0;
                case MIDI_COMMAND_SYSTEM_TIME_CODE_QUARTER_FRAME:
                case MIDI_COMMAND_SYSTEM_SONG_SELECT:
                    return 1;
                case MIDI_COMMAND_SYSTEM_SONG_POSITION:
                    return 2;
            }
            return 0;
    }
    return 0;
}


static inline void
call_callback(midi_t *m)
{
    switch ((midi_command_type_t) (m->last_status >> 4)) {
        case MIDI_COMMAND_NOTE_OFF:
        case MIDI_COMMAND_NOTE_ON:
        case MIDI_COMMAND_POLYPHONIC_PRESSURE:
        case MIDI_COMMAND_CONTROL_CHANGE:
        case MIDI_COMMAND_PROGRAM_CHANGE:
        case MIDI_COMMAND_CHANNEL_PRESSURE:
        case MIDI_COMMAND_PITCH_BEND:
            if (m->cb_channel == NULL)
                return;
            m->cb_channel(m->last_status >> 4, m->last_status & 0xf,
                m->data, m->data_idx);
            break;
        case MIDI_COMMAND_SYSTEM:
            if (m->cb_system == NULL)
                return;
            m->cb_system(m->last_status & 0xf, m->data, m->data_idx);
            break;
    }

    memset(m->data, 0, sizeof(m->data));
    m->data_idx = 0;
}


static inline void
handle_command(midi_t *m, uint8_t b)
{
    m->last_status = b;
    memset(m->data, 0, sizeof(m->data));
    m->data_len = get_data_len(b);
    m->data_idx = 0;
    if (m->data_len == 0)
        return call_callback(m);
}


static inline void
handle_data(midi_t *m, uint8_t b)
{
    if (m->last_status >> 4 == MIDI_COMMAND_UNSET)
        return;
    if (m->data_len < 0)
        return;
    m->data[m->data_idx++] = b;
    if (m->data_idx == m->data_len)
        return call_callback(m);
}


void
midi_push_byte(midi_t *m, uint8_t b)
{
    hard_assert(m);

    mutex_enter_blocking(&m->mtx);

    if (b >> 4 < 0x8)
        handle_data(m, b);
    else
        handle_command(m, b);

    mutex_exit(&m->mtx);
}
