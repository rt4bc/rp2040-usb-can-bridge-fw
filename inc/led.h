#ifndef _LED_H
#define _LED_H

#define LED_BLUE 24
#define LED_GREEN 25

#define LED_DURATION 25

#include <stdint.h>

void led_init();
void led_blue_blink(uint8_t numblinks);
void led_green_on(void);
void led_green_off(void);
void led_blue_on(void);
void led_process(void);

#endif