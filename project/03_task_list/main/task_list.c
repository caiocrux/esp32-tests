#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

// defines
#define WORKER_PERIOD_MS (2000U)
#define PERIODIC_PERIOD_MS (1000U)
#define CONTROLLER_PERIOD_MS (6000U)
#define MAIN_SUSPEND_TIME_MS (10000U)
#define MAIN_RESUME_DELAY_MS (5000U)

// Task handles
static TaskHandle_t worker_handle = NULL;
static TaskHandle_t periodic_handle = NULL;

// Worker task (self suspend)
static void worker_task(void *arg) {
  while (1) {
    printf("[WORKER] running then suspending itself\n");

    vTaskDelay(pdMS_TO_TICKS(WORKER_PERIOD_MS));

    printf("[WORKER] going to suspended state\n");
    vTaskSuspend(NULL); // self suspend

    printf("[WORKER] resumed!\n");
  }
}

// Periodic reference task
static void periodic_task(void *arg) {
  while (1) {
    printf("[PERIODIC] alive\n");
    vTaskDelay(pdMS_TO_TICKS(PERIODIC_PERIOD_MS));
  }
}

/* =========================
 * Controller task
 * resumes worker periodically
 * ========================= */
static void controller_task(void *arg) {
  while (1) {
    vTaskDelay(pdMS_TO_TICKS(CONTROLLER_PERIOD_MS));

    printf("[CTRL] resuming worker task\n");
    vTaskResume(worker_handle);
  }
}

/* =========================
 * app_main
 * ========================= */
void app_main(void) {
  printf("\n=== Suspend / Resume Demo ===\n");

  (void)xTaskCreate(worker_task, "worker", 2048, NULL, 2, &worker_handle);
  (void)xTaskCreate(periodic_task, "periodic", 2048, NULL, 1, &periodic_handle);
  (void)xTaskCreate(controller_task, "controller", 2048, NULL, 3, NULL);

  /* Demonstrate control from main context */
  vTaskDelay(pdMS_TO_TICKS(MAIN_SUSPEND_TIME_MS));

  printf("\n[MAIN] suspending periodic task\n");
  vTaskSuspend(periodic_handle);

  vTaskDelay(pdMS_TO_TICKS(MAIN_RESUME_DELAY_MS));

  printf("[MAIN] resuming periodic task\n\n");
  vTaskResume(periodic_handle);

  char pcWriteBuffer[1024] = {0};
  while (1) {
    vTaskList((char*)pcWriteBuffer);
    printf("name\t\tstate\tpriority\t\t\r\n");
    printf("\r\n %s\r\n", pcWriteBuffer);
    vTaskDelay(pdMS_TO_TICKS(MAIN_RESUME_DELAY_MS));
  }
}
