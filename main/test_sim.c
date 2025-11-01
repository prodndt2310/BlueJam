// Simulation task: exercises scheduling logic without RF.
// This file implements the same scheduling logic as the jammer,
// but calls the mock driver so nothing is transmitted over RF.
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "nrf24_mock.h"
#include "config.h"

static const char *TAG = "test_sim";

// PA channels (same as in your jammer logic)
static const uint8_t pa_channels[] = {0, 7, 14, 21, 28, 35, 37, 38, 39, 40, 41, 49, 56, 63, 70, 77, 78};
#define PA_CH_COUNT (sizeof(pa_channels)/sizeof(pa_channels[0]))

static uint8_t non_pa_channels[79];
static int non_pa_count = 0;

#define JAM_PACKET_SIZE 32

static void build_non_pa_list()
{
    int idx = 0;
    for (uint8_t ch = 0; ch < 79; ++ch) {
        bool is_pa = false;
        for (int j = 0; j < PA_CH_COUNT; ++j) {
            if (ch == pa_channels[j]) { is_pa = true; break; }
        }
        if (!is_pa) non_pa_channels[idx++] = ch;
    }
    non_pa_count = idx;
    ESP_LOGI(TAG, "non_pa_count=%d", non_pa_count);
}

static void sim_task(void *pv)
{
    ESP_LOGI(TAG, "Simulation task start (dry-run)");
    nrf24_mock_init("PA-mock");
    // Could init other named mocks if desired
    build_non_pa_list();

    uint8_t noise[JAM_PACKET_SIZE];
    for (int i = 0; i < JAM_PACKET_SIZE; ++i) noise[i] = esp_random() & 0xFF;

    uint64_t start_ms = esp_timer_get_time() / 1000;
    uint64_t pa_idx = 0;
    const int sim_interval_ms = 50; // scheduling resolution for simulation
    uint64_t last_report = start_ms;

    while (1) {
        uint64_t now_ms = esp_timer_get_time() / 1000;
        uint64_t elapsed = now_ms - start_ms;

        // PA module: rotate through pa_channels at a slower cadence
        if ((elapsed / 100) % 4 == 0) { // change condition to simulate scheduling
            uint8_t ch = pa_channels[pa_idx % PA_CH_COUNT];
            nrf24_mock_set_channel(ch);
            nrf24_mock_write(noise, JAM_PACKET_SIZE);
            pa_idx++;
        }

        // 3 random modules (simulate offset phasing)
        for (int m = 0; m < 3; ++m) {
            // phase offset: module m acts when (elapsed + offset) mod 4 == m+1
            uint32_t offset = (m + 1) * 25;
            if (((elapsed + offset) / 100) % 4 == (m + 1) % 4) {
                uint32_t r = esp_random();
                int idx = (r + m * 17) % non_pa_count;
                uint8_t ch = non_pa_channels[idx];
                // Use mock named per module for clearer logs
                nrf24_mock_set_channel(ch);
                nrf24_mock_write(noise, JAM_PACKET_SIZE);
            }
        }

        // Periodic report every 5s
        if (now_ms - last_report >= 5000) {
            const nrf24_mock_stats_t *s = nrf24_mock_get_stats();
            ESP_LOGI(TAG, "SIM REPORT: writes=%u set_channel_calls=%u last_ch=%u total_write_time_us=%llu",
                     s->writes, s->set_channel_calls, s->last_channel, (unsigned long long)s->total_write_time_us);
            last_report = now_ms;
        }

        vTaskDelay(pdMS_TO_TICKS(sim_interval_ms));
    }

    vTaskDelete(NULL);
}

void start_simulation_task()
{
    xTaskCreate(sim_task, "sim_task", 4 * 1024, NULL, 5, NULL);
}

// Optionally call start_simulation_task() from your app_main or main task when SIMULATION_MODE is enabled.