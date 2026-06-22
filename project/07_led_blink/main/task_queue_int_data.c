/**
 * @file task_queue_int_data.c
 * @brief LED blink test using FreeRTOS tasks and GPIO driver.
 * @details This module initializes three GPIO pins as outputs and
 *          sequentially blinks each LED with a 5-second interval.
 *
 * @copyright (c) 2026 - All rights reserved.
 * @version 1.0.0
 * @date 2026-05-21
 *
 * MISRA C:2012 Compliance Notes:
 * - Rule 8.4: All functions have compatible declarations.
 * - Rule 8.7: Functions with internal linkage are declared static.
 * - Rule 10.3: Implicit conversions are avoided where possible.
 * - Rule 17.7: Return values are intentionally unused for gpio_set_level
 *   (Deviation: ESP-IDF API usage).
 */

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

/** @brief GPIO pin number for LED1. */
#define LED1_GPIO 17

/** @brief GPIO pin number for LED2. */
#define LED2_GPIO 4

/** @brief GPIO pin number for LED3. */
#define LED3_GPIO 16

/** @brief Active LOW: value to turn LED on. */
#define LED_ON 0

/** @brief Active LOW: value to turn LED off. */
#define LED_OFF 1

/**
 * @brief Initializes GPIO pins for LED output.
 * @details Configures LED1, LED2, and LED3 as push-pull outputs
 *          with no pull-up/pull-down and no interrupt. All LEDs
 *          are set to OFF state after initialization.
 */
static void init_gpio(void) {
  gpio_config_t io_conf = {.mode = GPIO_MODE_OUTPUT,
                           .pin_bit_mask = (1ULL << LED1_GPIO) |
                                           (1ULL << LED2_GPIO) |
                                           (1ULL << LED3_GPIO),
                           .pull_down_en = GPIO_PULLDOWN_DISABLE,
                           .pull_up_en = GPIO_PULLUP_DISABLE,
                           .intr_type = GPIO_INTR_DISABLE};

  (void)gpio_config(&io_conf);

  /* Turn all LEDs OFF initially */
  (void)gpio_set_level(LED1_GPIO, LED_OFF);
  (void)gpio_set_level(LED2_GPIO, LED_OFF);
  (void)gpio_set_level(LED3_GPIO, LED_OFF);
}

/**
 * @brief FreeRTOS task that sequentially blinks each LED.
 * @param[in] arg Unused task parameter (NULL).
 */
static void led_test_task(void *arg) {
  (void)arg; /* MISRA Rule 2.7: Unused parameter cast to void */

  for (;;) { /* MISRA Rule 15.4: Use for(;;) instead of while(1) */
    /* LED1 */
    (void)printf("Testing LED1 (GPIO17)\n");
    (void)gpio_set_level(LED1_GPIO, LED_ON);
    vTaskDelay(pdMS_TO_TICKS(5000));
    (void)gpio_set_level(LED1_GPIO, LED_OFF);

    /* LED2 */
    (void)printf("Testing LED2 (GPIO4)\n");
    (void)gpio_set_level(LED2_GPIO, LED_ON);
    vTaskDelay(pdMS_TO_TICKS(5000));
    (void)gpio_set_level(LED2_GPIO, LED_OFF);

    /* LED3 */
    (void)printf("Testing LED3 (GPIO16)\n");
    (void)gpio_set_level(LED3_GPIO, LED_ON);
    vTaskDelay(pdMS_TO_TICKS(5000));
    (void)gpio_set_level(LED3_GPIO, LED_OFF);
  }
}

/**
 * @brief Application entry point.
 * @details Initializes GPIO and creates the LED test task.
 */
void app_main(void) {
  init_gpio();

  (void)xTaskCreate(led_test_task, "led_test_task", 2048, NULL, 5, NULL);
}
