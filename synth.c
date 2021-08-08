#include <tusb.h>
#include <midi.h>
#include <oscillator.h>
#include <synth.h>

static synth_t *synth = NULL;
static uint8_t midi_buf[16];
static uint16_t sample_buf[SYNTH_NUM_OSCILLATORS];

static void
note_on(uint8_t ch, uint8_t note)
{
    for (size_t i = 0; i < SYNTH_NUM_OSCILLATORS; i++)
        if (ch == synth->oscillators[i].midi_channel)
            if (oscillator_note_on(&(synth->oscillators[i].oscillator), note))
                return;
}


static void
note_off(uint8_t ch, uint8_t note)
{
    for (size_t i = 0; i < SYNTH_NUM_OSCILLATORS; i++)
        if (ch == synth->oscillators[i].midi_channel)
            if (oscillator_note_off(&(synth->oscillators[i].oscillator), note))
                return;
}


static void
midi_channel_cb(midi_command_type_t cmd, uint8_t ch, uint8_t *data, uint8_t data_len)
{

    switch (cmd) {
        case MIDI_COMMAND_NOTE_ON:
            if (data[1] != 0)
                return note_on(ch, data[0]);

        case MIDI_COMMAND_NOTE_OFF:
            return note_off(ch, data[0]);
    }
}


static int16_t
sample_cb(mcp4822_dac_t dac, void *data)
{
    int16_t rv = 0;

    for (size_t i = 0; i < SYNTH_NUM_OSCILLATORS; i++) {
        if (dac != synth->oscillators[i].channel || synth->oscillators[i].oscillator.note == NULL)
            continue;

        rv += oscillator_next_sample(&(synth->oscillators[i].oscillator)) / SYNTH_NUM_OSCILLATORS;
    }

    return rv;
}


int
synth_init(synth_t *s)
{
    hard_assert(!synth);
    hard_assert(s);

    synth = s;

    mcp4822_init(&(s->dac));
    mcp4822_set_cb(&(s->dac), MCP4822_DAC_A, sample_cb);
    mcp4822_set_cb(&(s->dac), MCP4822_DAC_B, sample_cb);

    int rv = eeprom_i2c_init(&(s->eeprom));
    if (rv != PICO_OK)
        return rv;

    ec11_init(&(s->encoder));

    rv = oled_tui_init(&(s->tui));
    if (rv < 0)
        return rv;

    for (size_t i = 0; i < SYNTH_NUM_OSCILLATORS; i++)
        oscillator_init(&(s->oscillators[i].oscillator));

    midi_init(&(s->midi.midi_uart));
    midi_init(&(s->midi.midi_usb));

    midi_set_channel_callback(&(s->midi.midi_uart), midi_channel_cb);
    midi_set_channel_callback(&(s->midi.midi_usb), midi_channel_cb);

    return PICO_OK;
}


void
synth_core1(void)
{
    hard_assert(synth);

    while (true)
        mcp4822_update(&(synth->dac));
}


void
synth_task(void)
{
    hard_assert(synth);

    if (tud_midi_available()) {
        uint32_t count = tud_midi_stream_read(midi_buf, sizeof(midi_buf));
        for (uint32_t i = 0; i < count; i++)
            midi_push_byte(&(synth->midi.midi_usb), midi_buf[i]);
    }

    if (uart_is_readable(synth->midi.uart))
        midi_push_byte(&(synth->midi.midi_uart), uart_getc(synth->midi.uart));

    ec11_task(&(synth->encoder));
}
