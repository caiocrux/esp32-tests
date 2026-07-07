/**
 * @file main.c
 *
 * @brief Display driver test application.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include <display.h>

static const char *TAG = "MAIN";

void app_main(void) {
  ESP_LOGI(TAG, "Starting display test...");
  Display_Init();
  Display_SetRotation(DISPLAY_ROTATION_180);
  static const uint16_t colors[] = {0xF800, 0x07E0, 0x001F};

  for (;;) {
    for (int i = 0; i < ((sizeof(colors) / sizeof(colors[0]))); i++) {
      Display_Fill(colors[i]);
      vTaskDelay(pdMS_TO_TICKS(3000));
    }
  }
}
