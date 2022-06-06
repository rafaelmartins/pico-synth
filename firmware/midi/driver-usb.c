/*
 * pico-synth: A Raspberry Pi Pico based digital synthesizer.
 *
 * SPDX-FileCopyrightText: 2021-2022 Rafael G. Martins <rafael@rafaelmartins.eng.br>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <pico/stdlib.h>
#include <pico/unique_id.h>
#include <tusb.h>
#include <pico-synth/midi.h>
#include "driver-usb.h"

// we use tinyusb descriptor structs as much as possible. see inline comments
// for details on why we had to define some custom structs.

static tusb_desc_device_t device_descriptor = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x16c0,  // default vid, free from v-usb
    .idProduct = 0x05dc, // default pid, free from v-usb
    .bcdDevice = 0x0000,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01,
};

const uint8_t*
tud_descriptor_device_cb(void)
{
    return (const uint8_t*) &device_descriptor;
}


static const struct TU_ATTR_PACKED {
    tusb_desc_configuration_t config_descriptor;
    tusb_desc_interface_t audio_ctrl_interface;

    // tinyusb defines this struct only for audio class 2.0
    struct TU_ATTR_PACKED {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bDescriptorSubtype;
        uint16_t bcdADC;
        uint16_t wTotalLength;
        uint8_t bInCollection;
        uint8_t baInterfaceNr;
    } audio_ctrl_interface_header;

    tusb_desc_interface_t midi_streaming_interface;
    midi_desc_header_t midi_streaming_header;
    midi_desc_in_jack_t midi_streaming_in_jack;
    midi_desc_out_jack_t midi_streaming_out_jack;

    // tinyusb misses an endpoint struct with the 2 last audio-specific fields
    struct TU_ATTR_PACKED {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bEndpointAddress;
        uint8_t bmAttributes;
        uint16_t wMaxPacketSize;
        uint8_t bInterval;
        uint8_t bRefresh;
        uint8_t bSynchAddress;
    } endpoint;

    // tinyusb misses this struct
    struct TU_ATTR_PACKED {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bDescriptorSubtype;
        uint8_t bNumEmbMIDIJack;
        uint8_t baAssocJackID;
    } midi_streaming_endpoint;

} config_descriptor = {
    .config_descriptor = {
        .bLength = sizeof(config_descriptor.config_descriptor),
        .bDescriptorType = TUSB_DESC_CONFIGURATION,
        .wTotalLength = sizeof(config_descriptor),
        .bNumInterfaces = 2,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = TU_BIT(7) | TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP,
        .bMaxPower = 50,
    },
    .audio_ctrl_interface = {
        .bLength = sizeof(config_descriptor.audio_ctrl_interface),
        .bDescriptorType = TUSB_DESC_INTERFACE,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = 0,
        .bInterfaceClass = TUSB_CLASS_AUDIO,
        .bInterfaceSubClass = AUDIO_SUBCLASS_CONTROL,
        .bInterfaceProtocol = AUDIO_FUNC_PROTOCOL_CODE_UNDEF,
        .iInterface = 0,
    },
    .audio_ctrl_interface_header = {
        .bLength = sizeof(config_descriptor.audio_ctrl_interface_header),
        .bDescriptorType = TUSB_DESC_CS_INTERFACE,
        .bDescriptorSubtype = AUDIO_CS_AC_INTERFACE_HEADER,
        .bcdADC = 0x0100,
        .wTotalLength = sizeof(config_descriptor.audio_ctrl_interface_header),
        .bInCollection = 1,
        .baInterfaceNr = 1,
    },
    .midi_streaming_interface = {
        .bLength = sizeof(config_descriptor.midi_streaming_interface),
        .bDescriptorType = TUSB_DESC_INTERFACE,
        .bInterfaceNumber = 1,
        .bAlternateSetting = 0,
        .bNumEndpoints = 1,
        .bInterfaceClass = TUSB_CLASS_AUDIO,
        .bInterfaceSubClass = AUDIO_SUBCLASS_MIDI_STREAMING,
        .bInterfaceProtocol = AUDIO_FUNC_PROTOCOL_CODE_UNDEF,
        .iInterface = 0,
    },
    .midi_streaming_header = {
        .bLength = sizeof(config_descriptor.midi_streaming_header),
        .bDescriptorType = TUSB_DESC_CS_INTERFACE,
        .bDescriptorSubType = MIDI_CS_INTERFACE_HEADER,
        .bcdMSC = 0x0100,
        .wTotalLength = sizeof(config_descriptor.midi_streaming_header) +
                        sizeof(config_descriptor.midi_streaming_in_jack) +
                        sizeof(config_descriptor.midi_streaming_out_jack),
    },
    .midi_streaming_in_jack = {
        .bLength = sizeof(config_descriptor.midi_streaming_in_jack),
        .bDescriptorType = TUSB_DESC_CS_INTERFACE,
        .bDescriptorSubType = MIDI_CS_INTERFACE_IN_JACK,
        .bJackType = MIDI_JACK_EMBEDDED,
        .bJackID = 1,
        .iJack = 0,
    },
    .midi_streaming_out_jack = {
        .bLength = sizeof(config_descriptor.midi_streaming_out_jack),
        .bDescriptorType = TUSB_DESC_CS_INTERFACE,
        .bDescriptorSubType = MIDI_CS_INTERFACE_OUT_JACK,
        .bJackType = MIDI_JACK_EXTERNAL,
        .bJackID = 2,
        .bNrInputPins = 1,
        .baSourceID = 1,
        .baSourcePin = 1,
        .iJack = 0,
    },
    .endpoint = {
        .bLength = sizeof(config_descriptor.endpoint),
        .bDescriptorType = TUSB_DESC_ENDPOINT,
        .bEndpointAddress = 0x01,
        .bmAttributes = TUSB_XFER_BULK,
        .wMaxPacketSize = 64,
        .bInterval = 0,
        .bRefresh = 0,
        .bSynchAddress = 0,
    },
    .midi_streaming_endpoint = {
        .bLength = sizeof(config_descriptor.midi_streaming_endpoint),
        .bDescriptorType = TUSB_DESC_CS_ENDPOINT,
        .bDescriptorSubtype = MIDI_CS_ENDPOINT_GENERAL,
        .bNumEmbMIDIJack = 1,
        .baAssocJackID = 1,
    },
};

const uint8_t*
tud_descriptor_configuration_cb(uint8_t index)
{
    return (const uint8_t*) &config_descriptor;
}


static const tusb_desc_string_t lang = {
    .bLength = 2 * sizeof(uint8_t) + sizeof(uint16_t),
    .bDescriptorType = TUSB_DESC_STRING,
    .unicode_string = {0x0409},
};

#define MANUFACTURER u"rgm.io"
static tusb_desc_string_t default_manufacturer = {
    .bLength = sizeof(MANUFACTURER),
    .bDescriptorType = TUSB_DESC_STRING,
    .unicode_string = MANUFACTURER,
};

#define PRODUCT u"pico-synth midi library"
static tusb_desc_string_t default_product = {
    .bLength = sizeof(PRODUCT),
    .bDescriptorType = TUSB_DESC_STRING,
    .unicode_string = PRODUCT,
};

static struct TU_ATTR_PACKED {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t unicode_string[0xff];
} string_descriptors[3];

static const uint8_t*
get_string_descriptor(uint8_t idx, tusb_desc_string_t *def)
{
    if (string_descriptors[idx - 1].bLength != 0)
        return (const uint8_t*) &string_descriptors[idx - 1];
    return (const uint8_t*) def;
}

const uint16_t*
tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    const uint8_t *rv = NULL;

    switch (index) {
    case 0:
        rv = (const uint8_t*) &lang;
        break;
    case 1:
        rv = get_string_descriptor(index, &default_manufacturer);
        break;
    case 2:
        rv = get_string_descriptor(index, &default_product);
        break;
    case 3:
        rv = get_string_descriptor(index, NULL);
        break;
    }

    return (const uint16_t*) rv;
}


static void
set_string_descriptor(uint8_t idx, const char *str)
{
    string_descriptors[idx].bLength = 2 * sizeof(uint8_t) + sizeof(uint16_t) * strlen(str);
    string_descriptors[idx].bDescriptorType = TUSB_DESC_STRING;
    for (uint8_t i = 0; str[i] != 0; i++)
        string_descriptors[idx].unicode_string[i] = str[i];
}

void
midi_usb_init(ps_midi_t *m)
{
    if (m == NULL || !m->usb.enable)
        return;

    if (m->usb.vid != 0)
        device_descriptor.idVendor = m->usb.vid;

    if (m->usb.pid != 0)
        device_descriptor.idProduct = m->usb.pid;

    if (m->usb.version != 0)
        device_descriptor.bcdDevice = m->usb.version;

    if (m->usb.manufacturer != NULL)
        set_string_descriptor(0, m->usb.manufacturer);

    if (m->usb.product != NULL)
        set_string_descriptor(1, m->usb.product);

    char serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * sizeof(uint16_t)];
    pico_get_unique_board_id_string(serial_str, sizeof(serial_str));
    set_string_descriptor(2, serial_str);

    tusb_init();
}


uint32_t
midi_usb_read(ps_midi_t *m, uint8_t *data, uint32_t data_len)
{
    if (m == NULL || !m->usb.enable)
        return 0;

    tud_task();

    if (data == NULL || data_len == 0)
        return 0;

    if (tud_midi_available())
        return tud_midi_stream_read(data, data_len);

    return 0;
}
