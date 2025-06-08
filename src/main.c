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
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <pico/binary_info.h>
#include <pico/mutex.h>
#include <hardware/watchdog.h>
#include <hardware/clocks.h>

#include "main.h"
#include "led.h"

int main(void)
{

    // Initialize Board
    led_init();
#ifdef DEBUG_PRINT
    stdio_init_all();
    DEBUG_LOG("********************************\r\n");
    DEBUG_LOG("******RP2040 USB CAN BRIDGE*****\r\n");
    DEBUG_LOG("********************************\r\n");

    uint f_pll_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
    uint f_pll_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
    uint f_rosc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_ROSC_CLKSRC);
    uint f_clk_sys = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_SYS);
    uint f_clk_peri = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_PERI);
    uint f_clk_usb = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_USB);
    uint f_clk_adc = frequency_count_khz(CLOCKS_FC0_SRC_VALUE_CLK_ADC);

    DEBUG_LOG("pll_sys  = %dkHz\n", f_pll_sys);
    DEBUG_LOG("pll_usb  = %dkHz\n", f_pll_usb);
    DEBUG_LOG("rosc     = %dkHz\n", f_rosc);
    DEBUG_LOG("clk_sys  = %dkHz\n", f_clk_sys);
    DEBUG_LOG("clk_peri = %dkHz\n", f_clk_peri);
    DEBUG_LOG("clk_usb  = %dkHz\n", f_clk_usb);
    DEBUG_LOG("clk_adc  = %dkHz\n", f_clk_adc);
#endif

    // Initialize TinyUSB stack
    tusb_init();
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