#include <pico/stdlib.h>
#include <pico/unique_id.h>
#include <tusb.h>

// we use tinyusb descriptor structs as much as possible. see inline comments
// for details on why we had to define some custom structs.

static const tusb_desc_device_t device_descriptor = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0x16c0, // free from v-usb
    .idProduct = 0x05dc, // free from v-usb
    .bcdDevice = 0x0100,
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
    .bLength = sizeof(uint8_t) + sizeof(uint8_t) + sizeof(uint16_t),
    .bDescriptorType = TUSB_DESC_STRING,
    .unicode_string = {0x0409},
};

#define MANUFACTURER u"rgm.io"
static const tusb_desc_string_t manufacturer = {
    .bLength = sizeof(MANUFACTURER),
    .bDescriptorType = TUSB_DESC_STRING,
    .unicode_string = MANUFACTURER,
};

#define PRODUCT u"pico-synth"
static const tusb_desc_string_t product = {
    .bLength = sizeof(PRODUCT),
    .bDescriptorType = TUSB_DESC_STRING,
    .unicode_string = PRODUCT,
};

static struct TU_ATTR_PACKED {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t unicode_string[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2];
} serial = {
    .bLength = sizeof(serial),
    .bDescriptorType = TUSB_DESC_STRING,
};

const uint16_t*
tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    const uint8_t *rv = NULL;

    switch (index) {
    case 0:
        rv = (const uint8_t*) &lang;
        break;
    case 1:
        rv = (const uint8_t*) &manufacturer;
        break;
    case 2:
        // FIXME: allow to configure the product name
        rv = (const uint8_t*) &product;
        break;
    case 3:
        if (serial.unicode_string[0] == 0) {
            char serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2];
            pico_get_unique_board_id_string(serial_str, sizeof(serial_str));
            for (size_t i = 0; serial_str[i] != 0; i++)
                serial.unicode_string[i] = serial_str[i];
        }
        rv = (const uint8_t*) &serial;
        break;
    }

    return (const uint16_t*) rv;
}
