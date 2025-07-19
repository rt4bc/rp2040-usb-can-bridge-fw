/**
 * Copyright (c) 2024 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <hardware/watchdog.h>
#include <pico/binary_info.h>
#include <pico/bootrom.h>
#include <pico/multicore.h>
#include <pico/mutex.h>

#include <tusb.h>
#include <tusb_config.h>

#include "system.h"

void core1_entry();

int main(void) {

  // Initialize Board
  system_init();

  //
  multicore_launch_core1(core1_entry);

  // Initialize TinyUSB stack
  tusb_init();
  while (true) {
    tud_task(); // 处理TinyUSB事件
  }

  return 0;
}

void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {
  (void)itf;
  (void)rts;

  cdc_line_coding_t line_coding = {0};
  if (dtr) {
    tud_cdc_get_line_coding(&line_coding);

    if (line_coding.bit_rate == 1200) {
      rom_reset_usb_boot_fn func =
          (rom_reset_usb_boot_fn)rom_func_lookup(ROM_FUNC_RESET_USB_BOOT);
      func(25, 0);
    } else if (line_coding.bit_rate == 2400) {
      watchdog_reboot(0, 0, 0);
    }
  } else {
    tud_cdc_write_clear();
  }
}
