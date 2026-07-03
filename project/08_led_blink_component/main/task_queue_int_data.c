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
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <led.h>
#include <stdio.h>

static void led_test_task(void *arg) {
  (void)arg;
  Led_Type led1 = {0};

  Led(&led1, CONFIG_RGB_LED_RED_GPIO, LED_RED, LED_ACTIVE_LOW);
  for (;;) { /* MISRA Rule 15.4: Use for(;;) instead of while(1) */
    LED_Toggle(&led1);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

/**
 * @brief Application entry point.
 * @details Initializes GPIO and creates the LED test task.
 */
void app_main(void) {
  (void)xTaskCreate(led_test_task, "led_test_task", 4096, NULL, 5, NULL);
}
