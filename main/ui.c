/*
 * SPDX-FileCopyrightText: 2026 Jaroslav Burian
 * SPDX-License-Identifier: MIT
 *
 * Screen composition.
 *
 * LVGL is used as a stateless renderer: on every wake the object tree is built
 * from scratch, rendered once into a 1bpp buffer and flushed to the panel, then
 * the board goes back to sleep. No event loop, no timers.
 *
 * 296x128 landscape:
 *
 *   +------------------------------------------+
 *   | Kartouzska                 16:03   [bat] |  header, 24 px
 *   +------------------------------------------+
 *   |  6 Kralovo Pole...   7'  | 6 Stary L.. 1'|  5 rows, 20 px each
 *   |  1 Reckovice        10'  | 1 Bystrc,.. 2'|
 *   |  ...                     | ...           |
 *   +--------------------------+---------------+
 */
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "driver/spi_master.h"
#include "esp_check.h"
#include "esp_heap_caps.h"
#include "esp_lcd_epaper.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "salina.h"

static const char *TAG = "ui";

/* Fixed metrics, driven by the font sizes. Everything else - column width, how
 * many rows fit, the buffer size - is derived from the panel at runtime, so a
 * larger display simply shows more departures. */
#define HEADER_H   24
#define ROW_H      20
#define ROW_TOP    (HEADER_H + 4)
#define COL_GAP    2
/* Widths measured from the font: "N92" is 28.1 px, "00:47" is 39.6 px. The
 * The line number is left aligned like the destination next to it; the gutter
 * keeps a gap even when the line fills its box, which "N90" very nearly does. */
#define COL_LINE_W 29
#define COL_GUTTER 5
#define COL_TIME_W 42
#define COL_PAD    2

extern const lv_font_t font_salina_14;
extern const lv_font_t font_salina_bold_18;

static esp_lcd_panel_handle_t s_panel;
static lv_display_t *s_disp;
static bool s_flush_done;
static uint8_t *s_draw_buf;

/* Panel geometry and the layout derived from it. */
static int s_width;
static int s_height;
static int s_col_w;
static int s_rows;

static uint32_t lvgl_tick_ms(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}

static void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    /* An I1 buffer starts with a two-colour palette; index 1 is white, which is
     * exactly what the e-paper driver expects. */
    px_map += 8;

    esp_lcd_panel_draw_bitmap(s_panel, area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map);
    /* Every wake is a cold start, so the controller has no usable baseline for
     * a differential update - it is a full refresh whether we ask or not. */
    epaper_panel_refresh(s_panel, EPD_REFRESH_FULL);
    lv_display_flush_ready(disp);
    s_flush_done = true;
}

esp_err_t salina_ui_init(void)
{
    const spi_bus_config_t bus_cfg = {
        .mosi_io_num = BOARD_PIN_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = BOARD_PIN_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096,
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(BOARD_SPI_HOST, &bus_cfg, SPI_DMA_CH_AUTO), TAG, "spi bus");

    const esp_lcd_panel_io_spi_config_t io_cfg = {
        .cs_gpio_num = BOARD_PIN_CS,
        .dc_gpio_num = BOARD_PIN_DC,
        .spi_mode = 0,
        .pclk_hz = BOARD_PIXEL_CLK_HZ,
        .trans_queue_depth = 10,
        .lcd_cmd_bits = 8,
        .lcd_param_bits = 8,
    };
    esp_lcd_panel_io_handle_t io = NULL;
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)BOARD_SPI_HOST, &io_cfg, &io),
                        TAG,
                        "panel io");

    esp_lcd_epaper_config_t epd_cfg = {
        .panel = BOARD_EPD_PANEL,
        .busy_gpio_num = BOARD_PIN_BUSY,
        .power_ctrl = board_panel_power,
        .rotation = BOARD_EPD_ROTATION,
        .refresh_mode = EPD_REFRESH_FULL,
    };
    const esp_lcd_panel_dev_config_t panel_cfg = {
        .reset_gpio_num = BOARD_PIN_RST,
        .bits_per_pixel = 1,
        .vendor_config = &epd_cfg,
    };
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_epaper(io, &panel_cfg, &s_panel), TAG, "panel");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_reset(s_panel), TAG, "reset");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_init(s_panel), TAG, "init");

    uint16_t panel_w = 0;
    uint16_t panel_h = 0;
    ESP_RETURN_ON_ERROR(epaper_panel_get_size(s_panel, &panel_w, &panel_h), TAG, "panel size");
    s_width = panel_w;
    s_height = panel_h;
    s_col_w = (s_width - COL_GAP) / SALINA_COLUMNS;
    s_rows = (s_height - ROW_TOP) / ROW_H;
    ESP_RETURN_ON_FALSE(s_rows > 0 && s_col_w > COL_LINE_W + COL_GUTTER + COL_TIME_W + COL_PAD * 2,
                        ESP_ERR_INVALID_SIZE,
                        TAG,
                        "panel too small for this layout");
    ESP_LOGI(TAG, "%dx%d panel, %d rows per column", s_width, s_height, s_rows);

    lv_init();
    /* LVGL needs a tick source even when it is only used to render one frame. */
    lv_tick_set_cb(lvgl_tick_ms);
    s_disp = lv_display_create(s_width, s_height);
    ESP_RETURN_ON_FALSE(s_disp, ESP_ERR_NO_MEM, TAG, "no display");
    lv_display_set_color_format(s_disp, LV_COLOR_FORMAT_I1);

    /* 8 bytes of I1 palette, then one bit per pixel. LVGL refuses a buffer that
     * is not aligned for the colour format and then renders nothing at all. */
    const size_t buf_size = 8 + (size_t)((s_width + 7) / 8) * s_height;
    s_draw_buf = heap_caps_aligned_alloc(64, buf_size, MALLOC_CAP_8BIT);
    ESP_RETURN_ON_FALSE(s_draw_buf, ESP_ERR_NO_MEM, TAG, "no mem for draw buffer");

    lv_display_set_buffers(s_disp, s_draw_buf, NULL, buf_size, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(s_disp, flush_cb);
    return ESP_OK;
}

/** @brief A black label with no decoration of its own. */
static lv_obj_t *make_label_mode(lv_obj_t *parent,
                                 const lv_font_t *font,
                                 int x,
                                 int y,
                                 int w,
                                 lv_text_align_t align,
                                 lv_label_long_mode_t long_mode,
                                 const char *text)
{
    lv_obj_t *label = lv_label_create(parent);

    lv_obj_set_pos(label, x, y);
    /* Pin the height to a single line: without it a label wide enough to wrap
     * grows downwards and overlaps the row below instead of truncating. */
    lv_obj_set_size(label, w, lv_font_get_line_height(font));
    lv_label_set_long_mode(label, long_mode);
    lv_obj_set_style_text_font(label, font, 0);
    lv_obj_set_style_text_color(label, lv_color_black(), 0);
    lv_obj_set_style_text_align(label, align, 0);
    lv_label_set_text(label, text);
    return label;
}

/** @brief Label that truncates with an ellipsis when it does not fit. */
static lv_obj_t *
make_label(lv_obj_t *parent, const lv_font_t *font, int x, int y, int w, lv_text_align_t align, const char *text)
{
    return make_label_mode(parent, font, x, y, w, align, LV_LABEL_LONG_MODE_DOTS, text);
}

/**
 * @brief Label that must never be shortened.
 *
 * A departure time cut to "16:0..." is worse than useless, so these are given a
 * column wide enough for the longest value and are not allowed to ellipsise.
 */
static lv_obj_t *
make_label_exact(lv_obj_t *parent, const lv_font_t *font, int x, int y, int w, lv_text_align_t align, const char *text)
{
    return make_label_mode(parent, font, x, y, w, align, LV_LABEL_LONG_MODE_CLIP, text);
}

/** @brief A solid black rectangle, used for the rules between areas. */
static void make_rule(lv_obj_t *parent, int x, int y, int w, int h)
{
    lv_obj_t *rule = lv_obj_create(parent);

    lv_obj_remove_style_all(rule);
    lv_obj_set_pos(rule, x, y);
    lv_obj_set_size(rule, w, h);
    lv_obj_set_style_bg_color(rule, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(rule, LV_OPA_COVER, 0);
}

static void render_column(lv_obj_t *scr, const salina_column_t *col, int x)
{
    const int shown = (col->count < s_rows) ? col->count : s_rows;

    for (int row = 0; row < shown; row++) {
        const int y = ROW_TOP + row * ROW_H;
        const salina_departure_t *d = &col->departures[row];
        const int dest_x = x + COL_PAD + COL_LINE_W + COL_GUTTER;
        const int dest_w = s_col_w - COL_PAD * 2 - COL_LINE_W - COL_GUTTER - COL_TIME_W;

        make_label_exact(scr, &font_salina_14, x + COL_PAD, y, COL_LINE_W, LV_TEXT_ALIGN_LEFT, d->line);
        make_label(scr, &font_salina_14, dest_x, y, dest_w, LV_TEXT_ALIGN_LEFT, d->destination);
        make_label_exact(scr, &font_salina_14, dest_x + dest_w, y, COL_TIME_W, LV_TEXT_ALIGN_RIGHT, d->when);
    }
    if (col->count == 0) {
        make_label(scr,
                   &font_salina_14,
                   x + COL_PAD,
                   ROW_TOP + ROW_H,
                   s_col_w - COL_PAD * 2,
                   LV_TEXT_ALIGN_CENTER,
                   "bez odjezdů");
    }
}

/** @brief Push whatever is on @p scr to the glass and wait for the refresh. */
static esp_err_t flush_screen(lv_obj_t *scr)
{
    ESP_LOGI(TAG, "rendering and refreshing");
    s_flush_done = false;
    lv_obj_invalidate(scr);

    /* Pump LVGL until the frame has reached the glass. The flush itself blocks
     * for the duration of the panel refresh, so a handful of iterations is all
     * this ever takes. */
    const int64_t deadline = esp_timer_get_time() + 15 * 1000 * 1000;
    while (!s_flush_done) {
        lv_timer_handler();
        if (esp_timer_get_time() > deadline) {
            ESP_LOGE(TAG, "LVGL never flushed the frame");
            return ESP_ERR_TIMEOUT;
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
    return ESP_OK;
}

esp_err_t salina_ui_render(const salina_data_t *data, const struct tm *now, int battery_mv)
{
    ESP_RETURN_ON_FALSE(data && now, ESP_ERR_INVALID_ARG, TAG, "null argument");

    lv_obj_t *scr = lv_screen_active();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    /* Header: stop on the left, time of this update and battery on the right. */
    /* Right hand side of the header, laid out from the right edge inwards:
     * battery voltage, the time of this update, and what that time means. */
    const int volts_w = 52;
    const int clock_w = 44;
    const int updated_w = 34;
    const int volts_x = s_width - volts_w - 3;
    const int clock_x = volts_x - clock_w - 4;
    const int updated_x = clock_x - updated_w - 2;

    make_label(scr, &font_salina_bold_18, 3, 1, updated_x - 6, LV_TEXT_ALIGN_LEFT, data->stop_name);

    make_label(scr, &font_salina_14, updated_x, 4, updated_w, LV_TEXT_ALIGN_RIGHT, "upd.");

    /* Without a synced clock the stamp would be a 1970 time that still looks
     * plausible, so say nothing instead. */
    char clock_text[8];
    if (salina_clock_is_set()) {
        strftime(clock_text, sizeof(clock_text), "%H:%M", now);
    } else {
        strlcpy(clock_text, "--:--", sizeof(clock_text));
    }
    make_label_exact(scr, &font_salina_14, clock_x, 4, clock_w, LV_TEXT_ALIGN_RIGHT, clock_text);

    /* The raw voltage says more than a four step battery glyph ever could. */
    char volts_text[16] = "";
    if (battery_mv > 0) {
        snprintf(volts_text, sizeof(volts_text), "%d.%02d V", (battery_mv / 1000) % 100, (battery_mv % 1000) / 10);
    }
    make_label_exact(scr, &font_salina_14, volts_x, 4, volts_w, LV_TEXT_ALIGN_RIGHT, volts_text);

    make_rule(scr, 0, HEADER_H, s_width, 2);
    make_rule(scr, s_col_w, ROW_TOP, 1, s_height - ROW_TOP);

    render_column(scr, &data->columns[0], 0);
    render_column(scr, &data->columns[1], s_col_w + COL_GAP);

    return flush_screen(scr);
}

/**
 * @brief What an unprovisioned board shows: how to reach its setup page.
 *
 * Drawn before the radio comes up, because the panel supply and a transmitting
 * Wi-Fi cannot share this board's rail.
 */
esp_err_t salina_ui_show_portal(const char *ssid, const char *url)
{
    ESP_RETURN_ON_FALSE(ssid && url, ESP_ERR_INVALID_ARG, TAG, "null argument");

    lv_obj_t *scr = lv_screen_active();
    lv_obj_clean(scr);
    lv_obj_set_style_bg_color(scr, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    /* Scanning this joins the network outright on both phone platforms.
     * "nopass" is what an open network is spelled as. */
    char payload[128];
    snprintf(payload, sizeof(payload), "WIFI:T:nopass;S:%s;;", ssid);

    const int qr_size = s_height - 16;
    lv_obj_t *qr = lv_qrcode_create(scr);
    lv_qrcode_set_size(qr, qr_size);
    lv_qrcode_set_dark_color(qr, lv_color_black());
    lv_qrcode_set_light_color(qr, lv_color_white());
    /* One bit per pixel and no greyscale to help the camera, so the white
     * border around the code is doing real work. */
    lv_qrcode_set_quiet_zone(qr, true);
    if (lv_qrcode_update(qr, payload, strlen(payload)) != LV_RESULT_OK) {
        ESP_LOGE(TAG, "QR payload too long");
        lv_obj_delete(qr);
        qr = NULL;
    }
    if (qr) {
        lv_obj_set_pos(qr, 8, 8);
    }

    const int text_x = (qr ? 8 + qr_size : 0) + 12;
    const int text_w = s_width - text_x - 4;
    make_label(scr, &font_salina_bold_18, text_x, 10, text_w, LV_TEXT_ALIGN_LEFT, "Setup");
    make_label(scr, &font_salina_14, text_x, 40, text_w, LV_TEXT_ALIGN_LEFT, ssid);
    make_label(scr, &font_salina_14, text_x, 66, text_w, LV_TEXT_ALIGN_LEFT, url);

    return flush_screen(scr);
}

void salina_ui_release(void)
{
    /* LVGL keeps a 64 kB pool and the panel holds an SPI bus. That is fine on
     * the departure path, where the radio is already off by the time any of it
     * exists, but the setup portal runs an access point, a station, a web
     * server and a TLS handshake afterwards, and the two together do not fit.
     * The image stays on the glass without any of this. */
    if (s_panel) {
        esp_lcd_panel_del(s_panel);
        s_panel = NULL;
    }
    spi_bus_free(BOARD_SPI_HOST);
    lv_deinit();
    s_disp = NULL;
    free(s_draw_buf);
    s_draw_buf = NULL;
}

void salina_ui_sleep(void)
{
    if (s_panel) {
        epaper_panel_sleep(s_panel);
    }
}
