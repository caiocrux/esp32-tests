#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>

/* Stack size expressed in FreeRTOS words */
#define TASK_STACK_WORDS (2048U)

/* Watchdog timeout in seconds (configured in menuconfig) */
#define TASK_WDT_PERIOD_MS (1000U)

/* Number of healthy iterations before failure */
#define FAULT_AFTER_ITER (8)

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

  int counter = 0;

  /* Register current task in TWDT */
  (void)esp_task_wdt_add(NULL);

  for (;;) {
    printf("[FAULTY] iteration %d\n", counter);

    if (counter < FAULT_AFTER_ITER) {
      /* Normal behaviour */
      (void)esp_task_wdt_reset();
    } else {
      /* Simulated lockup */
      printf("[FAULTY] watchdog feed stopped\n");
    }

    counter++;
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

  (void)esp_task_wdt_add(NULL);

  for (;;) {
    printf("[HEALTHY] running normally\n");

    /* Report liveness */
    (void)esp_task_wdt_reset();

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

  (void)xTaskCreate(healthy_task, "healthy_task", TASK_STACK_WORDS, NULL, 2,
                    NULL);

  (void)xTaskCreate(faulty_task, "faulty_task", TASK_STACK_WORDS, NULL, 2,
                    NULL);
}
