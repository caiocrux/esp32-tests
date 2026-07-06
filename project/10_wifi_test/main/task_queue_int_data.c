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
 */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "wifi_app.h"
#include <led.h>
#include <tasks_common.h>

const char *TAG = "main";

void app_main(void) {

  // Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_LOGI(TAG, "Starting wifi application");
  Led_Type wifi_led = {0};
  Led(&wifi_led, CONFIG_RGB_LED_RED_GPIO, LED_RED, LED_ACTIVE_LOW);
  LED_setState(&wifi_led, LED_ON);

  esp_log_level_set("wifi", ESP_LOG_NONE);
  // Start the WiFi application task
  ret = xTaskCreatePinnedToCore(
      &wifi_app_task, "wifi_app_task", WIFI_APP_TASK_STACK_SIZE, NULL,
      WIFI_APP_TASK_PRIORITY, NULL, WIFI_APP_TASK_CORE_ID);

  if (ret != pdPASS) {
    ESP_LOGE(TAG, "Failed creating WiFi task");
  }
}
