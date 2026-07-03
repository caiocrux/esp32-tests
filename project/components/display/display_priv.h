#ifndef DISPLAY_PRIV_H
#define DISPLAY_PRIV_H

#include "esp_err.h"

esp_err_t Display_SPI_Init(void);

esp_err_t Display_Backlight_Init(void);

esp_err_t Display_Panel_Init(void);

#endif
