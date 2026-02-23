#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

#define TASK_STACK_WORDS (2048U)
#define TASK_WDT_PERIOD_MS (1000U)
#define FAULT_AFTER_ITER (8)
#define TASK_PRIORITY (2)

/**
 * @brief Simulates a task deadlock
 *
 * Behaviour:
 *  - Registers itself in Task Watchdog
 *  - Periodically feeds watchdog
 *  - After N iterations stops feeding
 *
 * Expected Result:
 *  - Watchdog detects task stall
 */
static void faulty_task(void *arg) {
  (void)arg;

  if (esp_task_wdt_add(NULL) != ESP_OK) {
    printf("[FAULTY] Failed to register with watchdog\n");
    vTaskDelete(NULL);
    return;
  }

  for (int counter = 0;; counter++) {
    printf("[FAULTY] iteration %d\n", counter);

    if (counter < FAULT_AFTER_ITER) {
      esp_task_wdt_reset();
    } else if (counter == FAULT_AFTER_ITER) {
      printf("[FAULTY] watchdog feed stopped\n");
    }

    vTaskDelay(pdMS_TO_TICKS(TASK_WDT_PERIOD_MS));
  }
}

/**
 * @brief Periodic healthy task
 *
 * Behaviour:
 *  - Registers in Task Watchdog
 *  - Always feeds watchdog
 *
 * Purpose:
 *  - Demonstrates per-task watchdog supervision
 */
static void healthy_task(void *arg) {
  (void)arg;

  if (esp_task_wdt_add(NULL) != ESP_OK) {
    printf("[HEALTHY] Failed to register with watchdog\n");
    vTaskDelete(NULL);
    return;
  }

  for (;;) {
    printf("[HEALTHY] running normally\n");
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(TASK_WDT_PERIOD_MS));
  }
}

/**
 * @brief Demonstrates ESP-IDF Task Watchdog behaviour
 *
 * Creates:
 *  - One healthy task (feeds watchdog)
 *  - One faulty task (stops feeding)
 *
 * Result:
 *  - Watchdog triggers identifying faulty task
 */

void app_main(void) {
  printf("\n=== ESP32 Task Watchdog Demonstration ===\n");

  BaseType_t ret;

  ret = xTaskCreate(healthy_task, "healthy_task", TASK_STACK_WORDS, NULL,
                    TASK_PRIORITY, NULL);
  if (ret != pdPASS) {
    printf("Failed to create healthy_task\n");
  }

  ret = xTaskCreate(faulty_task, "faulty_task", TASK_STACK_WORDS, NULL,
                    TASK_PRIORITY, NULL);
  if (ret != pdPASS) {
    printf("Failed to create faulty_task\n");
  }
}
