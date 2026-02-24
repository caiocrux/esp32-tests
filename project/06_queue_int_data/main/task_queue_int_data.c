#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdio.h>

#define TASK_STACK_WORDS (2048U)
#define TASK_DELAY (1000U)
#define TASK_PRIORITY (2)

#define QUEUE_LENGTH 10
#define QUEUE_ITEM_SIZE (sizeof(int))

/**
 * @brief Periodically sends integer values to a queue.
 *
 * Behaviour:
 *  - Blocks if queue is full
 *  - Demonstrates deterministic producer behaviour
 */
static void send_task(void *arg) {
  BaseType_t ret;
  QueueHandle_t QueueHandler = (QueueHandle_t)arg;
  int send_value = 1U;

  for (;;) {
    printf("[SEND_TASK] running normally\r\n");
    printf("[SEND_TASK] seding value %d to queue\r\n", send_value);

    ret = xQueueSendToBack(QueueHandler, (void *)&send_value, (TickType_t)10);

    if (ret != pdPASS) {
      printf("[SEND_TASK] Error to send data into Queue!\r\n");
      if (ret == errQUEUE_FULL) {
        printf("[SEND_TASK]  Buffer is Full!\r\n");
      }
    }
    vTaskDelay(pdMS_TO_TICKS(TASK_DELAY));
    send_value++;
    if (send_value >= QUEUE_LENGTH) {
      send_value = 0U;
    }
  }
}

/**
 * @brief Receives integer values from a queue.
 *
 * Behaviour:
 *  - Blocks until data available
 *  - Demonstrates consumer synchronization
 */
static void receive_task(void *arg) {

  BaseType_t ret;
  QueueHandle_t QueueHandler = (QueueHandle_t)arg;
  int rec_value = 0U;
  for (;;) {
    printf("[REC_TASK] running normally\r\n");
    ret = xQueueReceive(QueueHandler, &rec_value, (TickType_t)10);

    if (ret != pdPASS) {
      printf("[REC_TASK] error to receive the data from queue! \r\n");
    }
    printf("[REC_TASK] received the value %d \r\n", rec_value);
    vTaskDelay(pdMS_TO_TICKS(TASK_DELAY));
  }
}

/**
 * @brief Demonstrates FreeRTOS queue-based communication.
 *
 * Creates:
 *  - One sender task
 *  - One receive task
 *
 * Demonstrates:
 *  - Blocking queue operations
 *  - Deterministic producer/consumer pattern
 */
void app_main(void) {
  printf("\n=== ESP32 FreeRTOS Queue Demonstration ===\n");

  BaseType_t ret;
  QueueHandle_t QueueHandler;

  QueueHandler = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);

  if (QueueHandler != NULL) {
    ret = xTaskCreate(send_task, "send_task", TASK_STACK_WORDS,
                      (void *)QueueHandler, TASK_PRIORITY, NULL);
    if (ret != pdPASS) {
      printf("Failed to create send_task\r\n");
    }
    ret = xTaskCreate(receive_task, "rec_task", TASK_STACK_WORDS,
                      (void *)QueueHandler, TASK_PRIORITY, NULL);
    if (ret != pdPASS) {
      printf("Failed to create rec_task\r\n");
    }
  } else {
    printf("Could not create Queue! \r\n");
  }
}
