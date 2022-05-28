#include <hardware/gpio.h>
#include <pico-synth/midi.h>
#include "driver-uart.h"


void
midi_uart_init(ps_midi_t *m)
{
    if (m == NULL || m->uart.uart == NULL)
        return;

    uart_init(m->uart.uart, 31250);
    gpio_set_function(m->uart.uart_rx, GPIO_FUNC_UART);
    gpio_pull_up(m->uart.uart_rx);
    uart_set_fifo_enabled(m->uart.uart, false);
    uart_set_format(m->uart.uart, 8, 1, UART_PARITY_NONE);
}


uint32_t
midi_uart_read(ps_midi_t *m, uint8_t *data, uint32_t data_len)
{
    if (m == NULL || m->uart.uart == NULL || data == NULL || data_len == 0)
        return 0;

    // 384us = time spent to read 12 bits from uart at 31250
    //
    // 12 bits is a little more than midi uart data format
    // (10 bits per data byte, 8 bits + start + stop).
    // it is a good timeout threshold to catch grouped data bytes without
    // false positives.
    uint32_t rv = 0;
    while (rv < data_len && uart_is_readable_within_us(m->uart.uart, 384))
        data[rv++] = uart_getc(m->uart.uart);
    return rv;
}
