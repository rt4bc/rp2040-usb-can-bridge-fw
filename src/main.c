/**
 * Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <pico/bootrom.h>
#include <pico/stdio.h>
#include <pico/binary_info.h>
#include <hardware/gpio.h>
#include <hardware/spi.h>
#include <bsp/board_api.h>
#include <tusb.h>

#include <tusb_config.h>
#include <pico/bootrom.h>
#include <hardware/watchdog.h>

int main(void)
{
    // Initialize TinyUSB stack
    board_init();
    tusb_init();

    // TinyUSB board init callback after init
    if (board_init_after_tusb)
    {
        board_init_after_tusb();
    }

    // let pico sdk use the first cdc interface for std io
    stdio_init_all();
    while (true)
    {
        tud_task(); // 处理TinyUSB事件
    }

    return 0;
}

void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
    (void)itf;
    (void)rts;

    cdc_line_coding_t line_coding = {0};
    if (dtr)
    {
        tud_cdc_get_line_coding(&line_coding);

        if (line_coding.bit_rate == 1200)
        {
            rom_reset_usb_boot_fn func = (rom_reset_usb_boot_fn)rom_func_lookup(ROM_FUNC_RESET_USB_BOOT);
            func(25, 0);
        }
        else if (line_coding.bit_rate == 2400)
        {
            watchdog_reboot(0, 0, 0);
        }
    }
    else
    {
        tud_cdc_write_clear();
    }
}