#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include <pico/printf.h>

#define DEBUG_UART uart0
#define DEBUG_UART_BAUDRATE 115200
#define DEBUG_UART_TX_PIN 0
#define DEBUG_UART_RX_PIN 1

#define DEBUG_UART_DATA_BITS 8
#define DEBUG_UART_STOP_BITS 1
#define DEBUG_UART_PARITY UART_PARITY_NONE

#ifdef DEBUG_PRINT
#define DEBUG_LOG(fmt, ...) printf("DEBUG: " fmt, ##__VA_ARGS__)
#else
#define DEBUG_LOG(fmt, ...) ((void)0) // 什么都不做
#endif

#define MIKRO_UART uart1
#define MIKRO_UART_BAUDRATE 115200
#define MIKRO_UART_TX_PIN 4
#define MIKRO_UART_RX_PIN 5

#define MIKRO_SPI spi0
#define MIKRO_SPI_SCK_PIN 10
#define MIKRO_SPI_TX_PIN 11
#define MIKRO_SPI_RX_PIN 12
#define MIKRO_SPI_CS_PIN 13
#define MIKRO_SPI_SPEED 4 * 1000 * 1000

#define CANFD_SPI spi1
#define CANFD_SPI_SCK_PIN 18
#define CANFD_SPI_TX_PIN 19
#define CANFD_SPI_RX_PIN 16
#define CANFD_SPI_CS_PIN 17
#define CANFD_SPI_SPEED 4 * 1000 * 1000
#define CANFD_INT0_PIN 20
#define CANFD_INT_TX_PIN 21
#define CANFD_INT_RX_PIN 22
#define CANFD_PHY_STDBY_PIN 26

void system_init(void);

#endif
