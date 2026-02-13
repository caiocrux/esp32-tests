#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include <string.h>

/* =========================
 *  Parameter types
 * ========================= */

typedef struct {
  int a;
  int b;
  int c;
} my_struct_t;

/* =========================
 *  Tasks
 * ========================= */

void task_int(void *pvParam) {
  int value = *(int *)pvParam;

  while (1) {
    printf("[INT] value = %d\n", value);
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void task_array(void *pvParam) {
  int *arr = (int *)pvParam;

  while (1) {
    printf("[ARRAY] %d %d %d %d %d\n", arr[0], arr[1], arr[2], arr[3], arr[4]);
    vTaskDelay(pdMS_TO_TICKS(3000));
  }
}

void task_struct(void *pvParam) {
  my_struct_t *s = (my_struct_t *)pvParam;

  while (1) {
    printf("[STRUCT] a=%d b=%d c=%d\n", s->a, s->b, s->c);
    vTaskDelay(pdMS_TO_TICKS(4000));
  }
}

void task_string(void *pvParam) {
  const char *str = (const char *)pvParam;

  while (1) {
    printf("[STRING] %s\n", str);
    vTaskDelay(pdMS_TO_TICKS(5000));
  }
}

/* =========================
 *  app_main
 * ========================= */

void app_main(void) {

  static int int_param = 42;
  static int array_param[5] = {1, 2, 3, 4, 5};
  static my_struct_t struct_param = {.a = 10, .b = 20, .c = 30};

  static const char string_param[] = "Hello from ESP32";

  /* ---- Task creation ---- */

  xTaskCreate(task_int, "task_int", 2048, &int_param, 1, NULL);

  xTaskCreate(task_array, "task_array", 2048, array_param, 1, NULL);

  xTaskCreate(task_struct, "task_struct", 2048, &struct_param, 1, NULL);

  xTaskCreate(task_string, "task_string", 2048, (void *)string_param, 1, NULL);
}
