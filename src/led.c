//
// LED: Handles blinking of status light
//
#include "pico/stdlib.h"
#include "pico/time.h"

#include "led.h"

// LEDs on board
#define GREEN_LED 25
#define BLUE_LED 15

// Private variables
static uint32_t led_blue_laston = 0;
static uint32_t led_green_laston = 0;
static uint32_t led_blue_lastoff = 0;
static uint32_t led_green_lastoff = 0;

static inline uint32_t GETTICK(void)
{
    return (uint32_t)to_ms_since_boot(get_absolute_time());
}

// Initialize LED GPIOs
void led_init()
{
    gpio_init(LED_GREEN);
    gpio_init(LED_BLUE);

    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    gpio_put(LED_GREEN, 1);
    gpio_put(LED_BLUE, 1);
}

// Turn green LED on
void led_green_on(void)
{
    // Make sure the LED has been off for at least LED_DURATION before turning on again
    // This prevents a solid status LED on a busy canbus
    if (led_green_laston == 0 && GETTICK() - led_green_lastoff > LED_DURATION)
    {
        // Invert LED
        gpio_put(LED_GREEN, 1);
        led_green_laston = GETTICK();
    }
}

// Turn green LED on
void led_green_off(void)
{
    gpio_put(LED_GREEN, 0);
}

// Blink blue LED (blocking)
void led_blue_blink(uint8_t numblinks)
{
    uint8_t i;
    for (i = 0; i < numblinks; i++)
    {
        gpio_put(LED_BLUE, 1);
        sleep_ms(100);
        gpio_put(LED_BLUE, 0);
        sleep_ms(100);
    }
}

// Attempt to turn on status LED
void led_blue_on(void)
{
    // Make sure the LED has been off for at least LED_DURATION before turning on again
    // This prevents a solid status LED on a busy canbus
    if (led_blue_laston == 0 && GETTICK() - led_blue_lastoff > LED_DURATION)
    {
        gpio_put(LED_BLUE, 1);
        led_blue_laston = GETTICK();
    }
}

// Process time-based LED events
void led_process(void)
{
    // If LED has been on for long enough, turn it off
    if (led_blue_laston > 0 && GETTICK() - led_blue_laston > LED_DURATION)
    {
        gpio_put(LED_BLUE, 1);
        led_blue_laston = 0;
        led_blue_lastoff = GETTICK();
    }

    // If LED has been on for long enough, turn it off
    if (led_green_laston > 0 && GETTICK() - led_green_laston > LED_DURATION)
    {
        // Invert LED
        gpio_put(LED_GREEN, 1);
        led_green_laston = 0;
        led_green_lastoff = GETTICK();
    }
}