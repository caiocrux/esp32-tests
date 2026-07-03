/** @file led.h
 *
 * @brief A description of the module’s purpose.
 *
 * @par
 * Copyright 2022 Caio Felipe Soares da Cruz <caio.crux@gmail.com>
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

 */

#ifndef LIB_GPIO_GPIO_H_
#define LIB_GPIO_GPIO_H_

#include <stdbool.h>
#include <stdint.h>
#include "driver/gpio.h"

#define GPIO_LOW   (0U)
#define GPIO_HIGH  (1U)

// Function prototypes
esp_err_t GPIO_ConfigOutput(gpio_num_t pin);
esp_err_t GPIO_Write(gpio_num_t pin, bool value);
bool GPIO_Read(gpio_num_t pin);

#endif  // LIB_GPIO_GPIO_H_

