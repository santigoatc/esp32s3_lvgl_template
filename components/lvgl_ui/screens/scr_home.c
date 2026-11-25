#include <stdint.h>
#include <stdio.h>

#include "driver/temperature_sensor.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "scr_home.h"
#include "screens.h"
#include "ui.h"

static const char *TAG = "scr_home";
static uint64_t s_last_uptime_seconds = UINT64_MAX;
static temperature_sensor_handle_t s_temperature_sensor;

void scr_home_prepare(void)
{
    temperature_sensor_config_t config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 50);
    ESP_ERROR_CHECK(temperature_sensor_install(&config, &s_temperature_sensor));
    ESP_ERROR_CHECK(temperature_sensor_enable(s_temperature_sensor));

    /* Registra callbacks persistentes en widgets que sobreviven entre pantallas.
     * Ejemplo:
     *   lv_obj_add_event_cb(ui_btnVolver, on_volver_cb, LV_EVENT_CLICKED, NULL);
     */
}

void scr_home_init(void)
{
    s_last_uptime_seconds = UINT64_MAX;
    ESP_LOGD(TAG, "init");
}

void scr_home_uninit(void)
{
    /* Detén animaciones o timers locales al salir. */
    ESP_LOGD(TAG, "uninit");
}

void scr_home_step(void)
{
    uint64_t uptime_seconds = (uint64_t)(esp_timer_get_time() / 1000000);
    if (uptime_seconds == s_last_uptime_seconds) {
        return;
    }
    s_last_uptime_seconds = uptime_seconds;

    uint64_t hours = uptime_seconds / 3600;
    unsigned int minutes = (uptime_seconds / 60) % 60;
    unsigned int seconds = uptime_seconds % 60;
    char uptime_text[24];

    snprintf(uptime_text, sizeof(uptime_text), "%02llu:%02u:%02u",
             (unsigned long long)hours, minutes, seconds);
    lv_label_set_text(ui_Label6, uptime_text);

    float temperature_celsius;
    esp_err_t err = temperature_sensor_get_celsius(s_temperature_sensor, &temperature_celsius);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read chip temperature: %s", esp_err_to_name(err));
        return;
    }

    char temperature_text[24];
    snprintf(temperature_text, sizeof(temperature_text), "Temp: %.1f°C", temperature_celsius);
    lv_label_set_text(ui_Label5, temperature_text);
}