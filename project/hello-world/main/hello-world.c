#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void MyTask(void *pvParam) {

  while (1) {
    printf("running from my first task!!\n");
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}


void app_main(void) {
  /* Print chip information */
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  printf("This is %s chip with %d CPU core(s), %s%s%s%s, ", CONFIG_IDF_TARGET,
         chip_info.cores,
         (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
         (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
         (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
         (chip_info.features & CHIP_FEATURE_IEEE802154)
             ? ", 802.15.4 (Zigbee/Thread)"
             : "");
  // task handler
  TaskHandle_t myTaskHandle = NULL;

  xTaskCreate(MyTask,   /* Pointer to the function that implements the task.*/
              "Task 1", /* Text name for the task. */
              1000,     /* Stack depth in words. */
              NULL,     /* This example does not use the task parameter. */
              1,        /* This task will run at priority 1. */
              &myTaskHandle);
  vTaskDelay(5000 / portTICK_PERIOD_MS);
  if (myTaskHandle != NULL) {
    vTaskDelete(myTaskHandle);
  }
}
