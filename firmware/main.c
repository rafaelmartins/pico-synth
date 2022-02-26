#include <tusb.h>
#include <pico/stdlib.h>


int
main(void)
{
    set_sys_clock_khz(133000, true);
    stdio_uart_init_full(uart0, 115200, 16, 17);
    tusb_init();

    while (1) {
        tud_task();
    }

    return 0;
}
