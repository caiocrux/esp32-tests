/*
 * led.c
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

#include <esp_log.h>
#include <led.h>
#include <stdio.h>

static const char *TAG = "LED";

static bool LED_Level(Led_Type *const self, LedState_Type state) {
  if (self->m_polarity == LED_ACTIVE_LOW) {
    return (state == LED_ON) ? false : true;
  }

  return (state == LED_ON);
}

void Led(Led_Type *const self, gpio_num_t pin, LedColors_Type color,
         LedPolarity polarity) {

  self->m_pin = pin;
  self->m_color = color;
  self->m_state = LED_OFF;
  self->m_polarity = polarity;

  (void)GPIO_ConfigOutput(pin);

  LED_setState(self, LED_OFF);
}

void LED_setState(Led_Type *const self, LedState_Type state) {
  if (self == NULL) {
    return;
  }
  self->m_state = state;

  ESP_LOGD(TAG, "%s: writing %d to pin %d \r\n", __FUNCTION__, state,
           self->m_pin);
  (void)GPIO_Write(self->m_pin, LED_Level(self, state));
}

LedState_Type LED_getState(Led_Type *const self) {
  bool pin_state = GPIO_Read(self->m_pin);
  self->m_state = (pin_state ? LED_ON : LED_OFF);
  return self->m_state;
}
void LED_Toggle(Led_Type *const self) {
  if (self == NULL) {
    return;
  }

  if (self->m_state == LED_ON) {
    LED_setState(self, LED_OFF);
  } else {
    LED_setState(self, LED_ON);
  }
}
