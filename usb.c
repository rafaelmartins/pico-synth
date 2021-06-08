#include <pico/stdlib.h>
#include <pico/unique_id.h>
#include <tusb.h>
#include <controller.h>

static uint8_t buf[1024];

static const tusb_desc_device_t desc_device = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = 0x0110,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = 0x16c0, // free from v-usb
    .idProduct          = 0x05dc, // free from v-usb
    .bcdDevice          = 0x0100,
    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,
    .bNumConfigurations = 0x01
};

const uint8_t*
tud_descriptor_device_cb(void)
{
    return (const uint8_t*) &desc_device;
}


static const uint8_t descriptor_conf[] = {
    TUD_CONFIG_DESCRIPTOR(1, 2, 0, TUD_CONFIG_DESC_LEN + TUD_MIDI_DESC_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    TUD_MIDI_DESCRIPTOR(0, 0, 0x01, TUSB_DIR_IN_MASK | 0x01, 64),
};

const uint8_t*
tud_descriptor_configuration_cb(uint8_t index)
{
    return descriptor_conf;
}


#define _DESCRIPTION_HEADER(len) (uint16_t) ((TUSB_DESC_STRING << 8) | (2 * (len) + 2))
static const uint16_t lang[] = {_DESCRIPTION_HEADER(1), 0x0409};
static const uint16_t manufacturer[] = {_DESCRIPTION_HEADER(6), 'r', 'g', 'm', '.', 'i', 'o'};
static const uint16_t product[] = {_DESCRIPTION_HEADER(10), 'p', 'i', 'c', 'o', '-', 's', 'y', 'n', 't', 'h'};
static uint16_t serial[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1] = {_DESCRIPTION_HEADER(PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2)};
#undef _DESCRIPTION_HEADER

const uint16_t*
tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    switch (index) {
        case 0:
            return lang;
        case 1:
            return manufacturer;
        case 2:
            return product;
        case 3:
            if (serial[1] == 0) {
                char serial_str[PICO_UNIQUE_BOARD_ID_SIZE_BYTES * 2 + 1];
                pico_get_unique_board_id_string(serial_str, sizeof(serial_str));
                for (size_t i = 0; serial_str[i] != 0; i++)
                    serial[i + 1] = serial_str[i];
            }
            return serial;
    }

    return NULL;
}


bool
tud_vendor_control_request_cb(uint8_t rhport, tusb_control_request_t const * request)
{
    uint16_t l = controller_usb_request_cb(
        request->bRequest,
        request->bmRequestType_bit.direction == TUSB_DIR_OUT,
        request->wValue,
        request->wIndex,
        buf);
    return tud_control_xfer(rhport, request, buf, l);
}
