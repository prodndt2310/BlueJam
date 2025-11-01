#include "nrf24_mock.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <string.h>
#include <stdio.h>

static const char *TAG = "nrf24_mock";
static nrf24_mock_stats_t stats;
static const char *mock_name = "nRF-mock";

void nrf24_mock_init(const char* name) {
    if (name) mock_name = name;
    memset(&stats, 0, sizeof(stats));
    ESP_LOGI(TAG, "%s: init (dry-run)", mock_name);
}

void nrf24_mock_set_channel(uint8_t ch) {
    stats.set_channel_calls++;
    stats.last_channel = ch;
    ESP_LOGI(TAG, "%s: set_channel %u", mock_name, ch);
}

void nrf24_mock_write(const uint8_t *buf, size_t len) {
    uint64_t t0 = esp_timer_get_time();
    stats.writes++;
    // Log a short hex preview for visibility
    size_t tolog = len > 8 ? 8 : len;
    char tmp[64];
    size_t pos = 0;
    for (size_t i = 0; i < tolog && pos + 3 < sizeof(tmp); ++i) {
        pos += snprintf(tmp + pos, sizeof(tmp) - pos, "%02X ", buf[i]);
    }
    if (tolog) ESP_LOGI(TAG, "%s: write len=%u, first %u bytes: %s", mock_name, (unsigned)len, (unsigned)tolog, tmp);
    uint64_t t1 = esp_timer_get_time();
    stats.total_write_time_us += (t1 - t0);
}

const nrf24_mock_stats_t* nrf24_mock_get_stats(void) {
    return &stats;
}

void nrf24_mock_reset_stats(void) {
    memset(&stats, 0, sizeof(stats));
}