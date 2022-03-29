#include <pico-synth/tui.h>

#define SETTINGS_VERSION 1

typedef struct {
    uint8_t oscillator_waveform;
    uint8_t oscillator_midi_channel;
    uint8_t _padding[1022];
} __attribute__((packed)) voice_t;

typedef struct {
    uint16_t version;
    voice_t voices[2];
    uint8_t _padding[2046];
} __attribute__((packed)) settings_t;

typedef struct {
    settings_t settings;
    uint8_t current_voice;
} settings_ctx_t;
