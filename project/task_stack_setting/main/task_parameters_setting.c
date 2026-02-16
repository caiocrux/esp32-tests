#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>

/* Stack size defined in words (FreeRTOS unit) */
#define TASK_STACK_WORDS (2048U)
/* Period for monitoring task stack usage */
#define MONITOR_PERIOD_MS (3000U)

/**
 * @brief Prints minimum remaining stack for a task
 *
 * Behaviour:
 *  - Reads the task high watermark
 *  - Reports worst observed stack usage margin
 *
 * Parameters:
 *  - task : handle of the task to inspect
 *
 * Notes:
 *  - Watermark represents minimum free stack since task start
 *  - Value never increases during runtime
 */
void analyze_task_stack(TaskHandle_t task) {

  UBaseType_t watermark = uxTaskGetStackHighWaterMark(task);

  printf("\n[STACK ANALYSIS] %s\n", pcTaskGetName(task));
  printf("[STACK] %s minimum free stack: %u words (%u bytes)\n",
         pcTaskGetName(task), watermark, watermark * sizeof(StackType_t));
}

/**
 * @brief Periodic string printing task
 *
 * Behaviour:
 *  - Receives constant string parameter
 *  - Prints periodically
 *  - Generates stable stack usage pattern
 *
 * Preconditions:
 *  - Valid string pointer provided
 */
void task_string(void *pvParam) {
  const char *str = (const char *)pvParam;

  while (1) {
    printf("[STRING] %s\n", str);
    vTaskDelay(pdMS_TO_TICKS(2500));
  }
}

/**
 * @brief Main monitoring loop
 *
 * Behaviour:
 *  - Creates worker task
 *  - Periodically checks stack watermark
 *
 * Demonstrates:
 *  - Runtime stack monitoring
 *  - Use of uxTaskGetStackHighWaterMark()
 */
void app_main(void) {

  static const char string_param[] = "Hello from ESP32";

  /* ---- Task creation ---- */
  TaskHandle_t task_handle;

  xTaskCreate(task_string, "task_string", TASK_STACK_WORDS,
              (void *)string_param, 1, &task_handle);

  while (1) {
    analyze_task_stack(task_handle);
    vTaskDelay(pdMS_TO_TICKS(MONITOR_PERIOD_MS));
  }
}
