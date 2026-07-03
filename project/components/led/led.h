/** @file led.h
 *
 * @brief A description of the module’s purpose.
 *
 * @par
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

 */

#ifndef LIB_LED_LED_H_
#define LIB_LED_LED_H_

#include "driver/gpio.h"
#include <gpio.h>
#include <stdint.h>

typedef enum { LED_RED = 0, LED_GREEN, LED_BLUE } LedColors_Type;

typedef enum { LED_OFF = 0, LED_ON } LedState_Type;

typedef enum { LED_ACTIVE_HIGH = 0, LED_ACTIVE_LOW } LedPolarity;

/*  Led's attributes */
typedef struct {
  LedColors_Type m_color;
  LedState_Type m_state;
  LedPolarity m_polarity;
  gpio_num_t m_pin;
} Led_Type;

void Led(Led_Type *const self, gpio_num_t pin, LedColors_Type color,
         LedPolarity polarity);
void LED_setState(Led_Type *const self, LedState_Type state);
LedState_Type LED_getState(Led_Type *const self);
void LED_Toggle(Led_Type *const self);

#endif //  LIB_LED_LED_H_
/*** end of file ***/
