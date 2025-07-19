#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/uart.h"
#include "hardware/xosc.h"
#include "pico/stdio_uart.h"

#include "led.h"
#include "system.h"

void system_init(void) {

  // start the xosc external 12MHz Osc
  xosc_init();

  // config the PLL_SYS to 133Mhz
  // python .\vcocalc.py 133
  // Requested:
  // 133.0 MHz Achieved : 133.0 MHz REFDIV : 1 FBDIV : 133(VCO = 1596.0 MHz)
  // PD1 : 6 P
  // D2 : 2 p
  // pll_init(pll_sys, 1, 1596 * MHZ, 6, 2);
  set_sys_clock_pll(1596 * MHZ, 6, 2);

#ifdef DEBUG_PRINT
  // Initialize UART0 DEBUG_UART
  stdio_uart_init_full(DEBUG_UART, DEBUG_UART_BAUDRATE, DEBUG_UART_TX_PIN,
                       DEBUG_UART_RX_PIN);

  DEBUG_LOG("********************************\r\n");
  DEBUG_LOG("******RP2040 USB CAN BRIDGE*****\r\n");
  DEBUG_LOG("********************************\r\n");

  uint f_pll_sys =
      frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_SYS_CLKSRC_PRIMARY);
  uint f_pll_usb =
      frequency_count_khz(CLOCKS_FC0_SRC_VALUE_PLL_USB_CLKSRC_PRIMARY);
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

  // Initialize UART1
  uart_init(MIKRO_UART, MIKRO_UART_BAUDRATE);
  gpio_set_function(MIKRO_UART_TX_PIN, GPIO_FUNC_UART);
  gpio_set_function(MIKRO_UART_RX_PIN, GPIO_FUNC_UART);

  // Initialize MIKRO SPI
  spi_init(MIKRO_SPI, MIKRO_SPI_SPEED);
  gpio_set_function(MIKRO_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(MIKRO_SPI_TX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(MIKRO_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_dir(MIKRO_SPI_CS_PIN, GPIO_OUT);

  // Initialize CANFD SPI
  spi_init(CANFD_SPI, CANFD_SPI_SPEED);
  gpio_set_function(CANFD_SPI_SCK_PIN, GPIO_FUNC_SPI);
  gpio_set_function(CANFD_SPI_TX_PIN, GPIO_FUNC_SPI);
  gpio_set_function(CANFD_SPI_RX_PIN, GPIO_FUNC_SPI);
  gpio_set_dir(CANFD_SPI_CS_PIN, GPIO_OUT);
  gpio_set_dir(CANFD_INT0_PIN, GPIO_IN);
  gpio_set_dir(CANFD_INT_RX_PIN, GPIO_IN);
  gpio_set_dir(CANFD_INT_TX_PIN, GPIO_IN);
  gpio_set_dir(CANFD_PHY_STDBY_PIN, GPIO_OUT);

  // Initialize LED GPIOs
  led_init();
}
