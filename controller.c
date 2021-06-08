#include <string.h>
#include <pico/assert.h>
#include <tusb.h>
#include <oscillator.h>
#include <ssd1306.h>
#include <controller.h>

#define assert_channel(c) hard_assert(c >= CHANNEL_1 && c < _NUM_CHANNELS)

typedef enum {
    CHANNEL_1,
    CHANNEL_2,
    CHANNEL_3,
    _NUM_CHANNELS,
} channel_t;

struct channel {
    oscillator_t *osc;
    uint8_t midi_channel;
    char str[22];
};

static struct channel channels[_NUM_CHANNELS];
static char midi_buf[64];
static ssd1306_t *display = NULL;


static void
display_render(void)
{
    hard_assert(display);

    ssd1306_clear(display);
    ssd1306_add_string_line(display, 0, "Pico Synth", SSD1306_LINE_ALIGN_CENTER);
    for (size_t i = 0; i < _NUM_CHANNELS; i++)
        ssd1306_add_string_line(display, 3 + (2 * i), channels[i].str,
            SSD1306_LINE_ALIGN_LEFT);
    ssd1306_render(display);
}


static void
set_midi_channel(channel_t c, uint8_t midi_channel)
{
    channels[c].midi_channel = midi_channel;
    char *str = channels[c].str + 10;

    if (midi_channel == 0xff) {
        *str++ = ' ';
        *str++ = ' ';
        *str++ = ' ';
        *str++ = ' ';
    }
    else {
        *str++ = '(';
        *str++ = (midi_channel / 10) + '0';
        *str++ = (midi_channel % 10) + '0';
        *str++ = ')';
    }
}


void
controller_init(ssd1306_t *disp, oscillator_t *osc1, oscillator_t *osc2, oscillator_t *osc3)
{
    hard_assert(!display);

    display = disp;

    channels[0].osc = osc1;
    channels[1].osc = osc2;
    channels[2].osc = osc3;

    for (size_t i = 0; i < _NUM_CHANNELS; i++) {
        strcpy(channels[i].str, "Channel       : ");
        channels[i].str[8] = '1' + i;
        set_midi_channel(i, i);
    }

    display_render();
}


static void
note_on(channel_t c, uint8_t note)
{
    assert_channel(c);

    oscillator_note_on(channels[c].osc, note);
    strncpy(channels[c].str + 16, channels[c].osc->note->name, sizeof(channels[c].str) - 16);
    display_render();
}


static void
note_off(channel_t c)
{
    assert_channel(c);

    oscillator_note_off(channels[c].osc);
    strncpy(channels[c].str + 16, "                ", sizeof(channels[c].str) - 16);
    display_render();
}


void
controller_midi_task(void)
{
    if (!tud_midi_available())
        return;

    uint32_t count = tud_midi_stream_read(midi_buf, sizeof(midi_buf));

    bool off = (count >= 2) && ((midi_buf[0] >> 4) == 0x8);
    bool on  = (count >= 3) && ((midi_buf[0] >> 4) == 0x9);

    if ((!on) && (!off))
        return;

    uint8_t chan = midi_buf[0] & 0x0f;
    for (size_t i = 0; i < _NUM_CHANNELS; i++) {
        if (channels[i].midi_channel != 0xff && channels[i].midi_channel == chan) {
            if (off || (on && midi_buf[2] == 0))
                note_off(i);
            else if (on)
                note_on(i, midi_buf[1]);
        }
    }
}


typedef enum {
    CMD_MIDI_CHANNEL = 1,
} usb_command_t;

uint16_t
controller_usb_request_cb(uint8_t cmd, bool write, uint16_t val, uint16_t idx, uint8_t *buf)
{
    switch ((usb_command_t) cmd) {
        case CMD_MIDI_CHANNEL: {
            if (idx < CHANNEL_1 || idx >= _NUM_CHANNELS)
                return 0;

            if (!write) {
                buf[0] = channels[idx].midi_channel;
                return 1;
            }

            set_midi_channel(idx, val);
            display_render();
            break;
        }
    }

    return 0;
}
