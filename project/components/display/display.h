/**
 * @file display.h
 *
 * @brief Generic LCD display driver.
 *
 * This module provides a hardware abstraction for LCD displays.
 * The implementation currently uses the ESP-IDF esp_lcd driver
 * with an ILI9341 controller.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_ops.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    DISPLAY_ROTATION_0 = 0,
    DISPLAY_ROTATION_90,
    DISPLAY_ROTATION_180,
    DISPLAY_ROTATION_270
} display_rotation_t;

typedef struct
{
    uint16_t width;
    uint16_t height;
} display_resolution_t;

/**
 * Initialize display hardware.
 */
esp_err_t Display_Init(void);

/**
 * Reset display.
 */
esp_err_t Display_Reset(void);

/**
 * Turn display on or off.
 */
esp_err_t Display_Enable(bool enable);

/**
 * Enable or disable backlight.
 */
esp_err_t Display_Backlight(bool enable);

/**
 * Change display rotation.
 */
esp_err_t Display_SetRotation(display_rotation_t rotation);

/**
 * Draw bitmap.
 */
esp_err_t Display_DrawBitmap(uint16_t x,
                             uint16_t y,
                             uint16_t width,
                             uint16_t height,
                             const void *bitmap);

/**
 * Get display resolution.
 */
display_resolution_t Display_GetResolution(void);
/**
 * @brief Fill the entire display with a single color.
 *
 * @param color RGB565 color.
 *
 * @return ESP_OK on success.
 */
esp_err_t Display_Fill(uint16_t color);

/**
 * Return esp_lcd panel handle.
 */
esp_lcd_panel_handle_t Display_GetPanelHandle(void);
#ifdef __cplusplus
}
#endif

#endif
