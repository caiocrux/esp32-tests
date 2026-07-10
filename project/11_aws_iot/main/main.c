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

const char *TAG = "MAIN";

int aws_iot_demo_main(int argc, char **argv);

void wifi_application_connected_events(void) {
  ESP_LOGI(TAG, "WiFi Application Connected!!");
  aws_iot_demo_main(0, NULL);
}

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
  wifi_app_set_callback(wifi_application_connected_events);
  wifi_app_init();
}
