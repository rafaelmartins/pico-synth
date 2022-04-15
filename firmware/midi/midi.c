#include <string.h>
#include <tusb.h>
#include <hardware/gpio.h>
#include <pico-synth/midi.h>

// FIXME: solve conflicts when both USB and UART send data at the same time


void
ps_midi_init(ps_midi_t *m)
{
    hard_assert(m);

    mutex_init(&m->_mtx);
    m->_last_status = 0;
    memset(m->_data, 0, sizeof(m->_data));
    m->_data_len = 0;
    m->_data_idx = 0;

    if (m->uart != NULL) {
        uart_init(m->uart, 31250);
        gpio_set_function(m->uart_rx, GPIO_FUNC_UART);
        gpio_pull_up(m->uart_rx);
    }

    if (m->with_usb)
        tusb_init();
}


static inline int8_t
get_data_len(uint8_t b)
{
    switch ((ps_midi_command_type_t) (b >> 4)) {
    case PS_MIDI_COMMAND_PROGRAM_CHANGE:
    case PS_MIDI_COMMAND_CHANNEL_PRESSURE:
        return 1;
    case PS_MIDI_COMMAND_NOTE_OFF:
    case PS_MIDI_COMMAND_NOTE_ON:
    case PS_MIDI_COMMAND_POLYPHONIC_PRESSURE:
    case PS_MIDI_COMMAND_CONTROL_CHANGE:
    case PS_MIDI_COMMAND_PITCH_BEND:
        return 2;
    case PS_MIDI_COMMAND_SYSTEM:
        switch ((ps_midi_command_system_type_t) (b & 0xf)) {
        case PS_MIDI_COMMAND_SYSTEM_SYSEX1:
            return -1;
        case PS_MIDI_COMMAND_SYSTEM_UNDEFINED1:
        case PS_MIDI_COMMAND_SYSTEM_UNDEFINED2:
        case PS_MIDI_COMMAND_SYSTEM_TUNE_REQUEST:
        case PS_MIDI_COMMAND_SYSTEM_SYSEX2:
        case PS_MIDI_COMMAND_SYSTEM_RT_TIMING_CLOCK:
        case PS_MIDI_COMMAND_SYSTEM_RT_UNDEFINED1:
        case PS_MIDI_COMMAND_SYSTEM_RT_START:
        case PS_MIDI_COMMAND_SYSTEM_RT_CONTINUE:
        case PS_MIDI_COMMAND_SYSTEM_RT_STOP:
        case PS_MIDI_COMMAND_SYSTEM_RT_UNDEFINED2:
        case PS_MIDI_COMMAND_SYSTEM_RT_ACTIVE_SENSE:
        case PS_MIDI_COMMAND_SYSTEM_RT_SYSTEM_RESET:
            return 0;
        case PS_MIDI_COMMAND_SYSTEM_TIME_CODE_QUARTER_FRAME:
        case PS_MIDI_COMMAND_SYSTEM_SONG_SELECT:
            return 1;
        case PS_MIDI_COMMAND_SYSTEM_SONG_POSITION:
            return 2;
        }
        return 0;

    case PS_MIDI_COMMAND_UNSET:
        return 0;
    }
    return 0;
}


static inline void
call_callback(ps_midi_t *m)
{
    switch ((ps_midi_command_type_t) (m->_last_status >> 4)) {
    case PS_MIDI_COMMAND_NOTE_OFF:
    case PS_MIDI_COMMAND_NOTE_ON:
    case PS_MIDI_COMMAND_POLYPHONIC_PRESSURE:
    case PS_MIDI_COMMAND_CONTROL_CHANGE:
    case PS_MIDI_COMMAND_PROGRAM_CHANGE:
    case PS_MIDI_COMMAND_CHANNEL_PRESSURE:
    case PS_MIDI_COMMAND_PITCH_BEND:
        if (ps_midi_channel_cb != NULL)
            ps_midi_channel_cb(m->_last_status >> 4, m->_last_status & 0xf, m->_data, m->_data_idx);
        break;
    case PS_MIDI_COMMAND_SYSTEM:
        if (ps_midi_system_cb != NULL)
            ps_midi_system_cb(m->_last_status & 0xf, m->_data, m->_data_idx);
        break;
    case PS_MIDI_COMMAND_UNSET:
        break;
    }

    memset(m->_data, 0, sizeof(m->_data));
    m->_data_idx = 0;
}


static inline void
handle_command(ps_midi_t *m, uint8_t b)
{
    m->_last_status = b;
    memset(m->_data, 0, sizeof(m->_data));
    m->_data_len = get_data_len(b);
    m->_data_idx = 0;
    if (m->_data_len == 0)
        call_callback(m);
}


static inline void
handle_data(ps_midi_t *m, uint8_t b)
{
    if (m->_last_status >> 4 == PS_MIDI_COMMAND_UNSET)
        return;
    if (m->_data_len < 0)
        return;
    m->_data[m->_data_idx++] = b;
    if (m->_data_idx == m->_data_len)
        call_callback(m);
}


static void
push_byte(ps_midi_t *m, uint8_t b)
{
    hard_assert(m);

    mutex_enter_blocking(&m->_mtx);

    if ((b >> 4) < 0x8)
        handle_data(m, b);
    else
        handle_command(m, b);

    mutex_exit(&m->_mtx);
}


void
ps_midi_task(ps_midi_t *m)
{
    hard_assert(m);

    if (m->uart != NULL && uart_is_readable(m->uart))
        push_byte(m, uart_getc(m->uart));

    if (m->with_usb) {
        tud_task();

        if (tud_midi_available()) {
            uint8_t midi_buf[16];
            uint32_t count = tud_midi_stream_read(midi_buf, sizeof(midi_buf));
            for (uint32_t i = 0; i < count; i++)
                push_byte(m, midi_buf[i]);
        }
    }
}
