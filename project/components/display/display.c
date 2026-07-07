#include "display.h"
#include "display_priv.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_check.h"
#include "esp_log.h"

#include "esp_check.h"
#include "esp_lcd_ili9341.h"
#include "esp_lcd_io_spi.h"
#include "esp_log.h"

#include "sdkconfig.h"
static const char *TAG = "DISPLAY";

static esp_lcd_panel_io_handle_t s_io = NULL;

static esp_lcd_panel_handle_t s_panel = NULL;

#define DISPLAY_MAX_WIDTH                                                      \
  ((CONFIG_DISPLAY_WIDTH > CONFIG_DISPLAY_HEIGHT) ? CONFIG_DISPLAY_WIDTH       \
                                                  : CONFIG_DISPLAY_HEIGHT)

static uint16_t s_line_buffer[DISPLAY_MAX_WIDTH];

static display_resolution_t s_resolution = {.width = CONFIG_DISPLAY_WIDTH,
                                            .height = CONFIG_DISPLAY_HEIGHT};

esp_err_t Display_Backlight_Init(void) {
  gpio_config_t config = {.pin_bit_mask = (1ULL << CONFIG_DISPLAY_PIN_BCKL),
                          .mode = GPIO_MODE_OUTPUT,
                          .pull_up_en = GPIO_PULLUP_DISABLE,
                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
                          .intr_type = GPIO_INTR_DISABLE};

  ESP_RETURN_ON_ERROR(gpio_config(&config), TAG,
                      "Failed to configure backlight GPIO");

  gpio_set_level(CONFIG_DISPLAY_PIN_BCKL, 0);

  ESP_LOGI(TAG, "Backlight GPIO initialized");

  return ESP_OK;
}

esp_err_t Display_SPI_Init(void) {
  spi_bus_config_t buscfg = {
      .mosi_io_num = CONFIG_DISPLAY_PIN_MOSI,
      .miso_io_num = CONFIG_DISPLAY_PIN_MISO,
      .sclk_io_num = CONFIG_DISPLAY_PIN_CLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = CONFIG_DISPLAY_WIDTH * 40 * sizeof(uint16_t),
  };

  ESP_RETURN_ON_ERROR(spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO),
                      TAG, "SPI initialization failed");

  ESP_LOGI(TAG, "SPI initialized");

  return ESP_OK;
}
esp_err_t Display_Panel_Init(void) {
  esp_lcd_panel_io_spi_config_t io_config = {
      .dc_gpio_num = CONFIG_DISPLAY_PIN_DC,
      .cs_gpio_num = CONFIG_DISPLAY_PIN_CS,
      .pclk_hz = CONFIG_DISPLAY_SPI_CLOCK,
      .lcd_cmd_bits = 8,
      .lcd_param_bits = 8,
      .spi_mode = 0,
      .trans_queue_depth = 10,
  };

  ESP_RETURN_ON_ERROR(
      esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)SPI2_HOST, &io_config,
                               &s_io),
      TAG, "Failed creating panel IO");

  ESP_LOGI(TAG, "Panel IO created");

  esp_lcd_panel_dev_config_t panel_config = {
      .reset_gpio_num = CONFIG_DISPLAY_PIN_RST,
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_RGB,
      .bits_per_pixel = 16,
  };

  ESP_RETURN_ON_ERROR(esp_lcd_new_panel_ili9341(s_io, &panel_config, &s_panel),
                      TAG, "Failed creating ILI9341");

  // ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(s_panel), TAG, "Panel reset
  // failed");

  ESP_RETURN_ON_ERROR(esp_lcd_panel_init(s_panel), TAG, "Panel init failed");
  // ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(s_panel, true));
  // ESP_ERROR_CHECK(esp_lcd_panel_mirror(s_panel, true, true));
  ESP_RETURN_ON_ERROR(esp_lcd_panel_disp_on_off(s_panel, true), TAG,
                      "Display ON failed");

  ESP_LOGI(TAG, "ILI9341 initialized");

  return ESP_OK;
}

esp_err_t Display_Enable(bool enable) {
  return esp_lcd_panel_disp_on_off(s_panel, enable);
}

esp_err_t Display_Backlight(bool enable) {
  int level;

#if CONFIG_DISPLAY_BACKLIGHT_ACTIVE_LOW
  level = enable ? 0 : 1;
#else
  level = enable ? 1 : 0;
#endif

  ESP_RETURN_ON_ERROR(gpio_set_level(CONFIG_DISPLAY_PIN_BCKL, level), TAG,
                      "Failed to set backlight");

  return ESP_OK;
}

esp_err_t Display_Reset(void) { return esp_lcd_panel_reset(s_panel); }

esp_err_t Display_DrawBitmap(uint16_t x, uint16_t y, uint16_t width,
                             uint16_t height, const void *bitmap) {

  esp_err_t ret;

  ret = esp_lcd_panel_draw_bitmap(s_panel, x, y, x + width, y + height, bitmap);

  ESP_LOGD(TAG, "Draw (%u,%u)-(%u,%u) ret=%s", x, y, x + width, y + height,
           esp_err_to_name(ret));

  return ret;
}
esp_err_t Display_Init(void) {
  ESP_RETURN_ON_ERROR(Display_Backlight_Init(), TAG, "Backlight init failed");
  ESP_RETURN_ON_ERROR(Display_SPI_Init(), TAG, "SPI init failed");
  ESP_RETURN_ON_ERROR(Display_Panel_Init(), TAG, "Panel init failed");
  ESP_RETURN_ON_ERROR(Display_Backlight(true), TAG,
                      "Panel Backlight init failed");
  return ESP_OK;
}

esp_err_t Display_Fill(uint16_t color) {
  display_resolution_t resolution = Display_GetResolution();

  for (uint16_t i = 0; i < resolution.width; i++) {
    s_line_buffer[i] = color;
  }

  for (uint16_t y = 0; y < resolution.height; y++) {
    ESP_RETURN_ON_ERROR(
        Display_DrawBitmap(0, y, resolution.width, 1, s_line_buffer), TAG,
        "Failed to draw line");
  }

  return ESP_OK;
}
esp_err_t Display_SetRotation(display_rotation_t rotation) {
  esp_err_t ret;

  switch (rotation) {
  case DISPLAY_ROTATION_0:

    ret = esp_lcd_panel_swap_xy(s_panel, false);
    ESP_RETURN_ON_ERROR(ret, TAG, "swap_xy failed");

    ret = esp_lcd_panel_mirror(s_panel, false, false);
    ESP_RETURN_ON_ERROR(ret, TAG, "mirror failed");

    s_resolution.width = 240;
    s_resolution.height = 320;
    break;

  case DISPLAY_ROTATION_90:

    ret = esp_lcd_panel_swap_xy(s_panel, false);
    ESP_RETURN_ON_ERROR(ret, TAG, "swap_xy failed");

    ret = esp_lcd_panel_mirror(s_panel, false, true);
    ESP_RETURN_ON_ERROR(ret, TAG, "mirror failed");

    s_resolution.width = 320;
    s_resolution.height = 240;
    break;

  case DISPLAY_ROTATION_180:

    ret = esp_lcd_panel_swap_xy(s_panel, true);
    ESP_RETURN_ON_ERROR(ret, TAG, "swap_xy failed");

    ret = esp_lcd_panel_mirror(s_panel, true, true);
    ESP_RETURN_ON_ERROR(ret, TAG, "mirror failed");

    s_resolution.width = 240;
    s_resolution.height = 320;
    break;

  case DISPLAY_ROTATION_270:

    ret = esp_lcd_panel_swap_xy(s_panel, true);
    ESP_RETURN_ON_ERROR(ret, TAG, "swap_xy failed");

    ret = esp_lcd_panel_mirror(s_panel, false, true);
    ESP_RETURN_ON_ERROR(ret, TAG, "mirror failed");

    s_resolution.width = 320;
    s_resolution.height = 240;
    break;

  default:
    return ESP_ERR_INVALID_ARG;
  }

  ESP_LOGD(TAG, "Rotation=%d (%ux%u)", rotation, s_resolution.width,
           s_resolution.height);

  return ESP_OK;
}
display_resolution_t Display_GetResolution(void) { return s_resolution; }

esp_lcd_panel_handle_t Display_GetPanelHandle(void) { return s_panel; }
