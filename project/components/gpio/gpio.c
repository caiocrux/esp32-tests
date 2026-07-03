/*
 * gpio.c
 *
 * Copyright 2026 Caio Felipe Soares da Cruz <caio.crux@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
#include "driver/gpio.h"
#include <esp_log.h>
#include <gpio.h>
#include <stdio.h>

static const char *TAG = "GPIO";
esp_err_t GPIO_ConfigOutput(gpio_num_t pin) {

  gpio_config_t config = {.mode = GPIO_MODE_OUTPUT,
                          .pin_bit_mask = (1ULL << pin),
                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
                          .pull_up_en = GPIO_PULLUP_DISABLE,
                          .intr_type = GPIO_INTR_DISABLE};

  esp_err_t ret = gpio_config(&config);

  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "%s: gpio_config(%d) failed (%s)", __func__, pin,
             esp_err_to_name(ret));
  }
  return ret;
}

esp_err_t GPIO_Write(gpio_num_t pin, bool value) {

  esp_err_t ret = gpio_set_level(pin, value);

  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "%s: gpio_set_level(%d) failed", __func__, pin);
  }
  return ret;
}

bool GPIO_Read(gpio_num_t pin) { return (bool)gpio_get_level(pin); }
