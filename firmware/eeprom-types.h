/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <stdint.h>

// WARNING: must be careful to edit these structs, to keep alignment untouched.
// WARNING: version should be bumped whenever some new field is added.
#define EEPROM_SETTINGS_VERSION 1
#define EEPROM_PRESET_COUNT 128
#define EEPROM_VOICE_COUNT 2

typedef struct __attribute__((packed)) {
    char name[8];

    struct __attribute__((packed)) {
        uint8_t waveform;
        uint8_t _padding[15];
    } oscillator;

    struct __attribute__((packed)) {
        uint8_t attack;
        uint8_t decay;
        uint8_t sustain;
        uint8_t release;
        uint8_t _padding[12];
    } adsr;

    uint8_t _padding[216];
} eeprom_preset_t;

typedef struct __attribute__((packed)) {
    uint8_t midi_channel;
    uint8_t preset;
    uint8_t _padding[254];
} eeprom_voice_t;

typedef struct __attribute__((packed)) {
    uint16_t version;
    eeprom_preset_t presets[EEPROM_PRESET_COUNT];
    eeprom_voice_t voices[EEPROM_VOICE_COUNT];
} eeprom_settings_t;
