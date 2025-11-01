#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "config.h"
#include "battery.h"
#include "oled.h"
#include "nrf24_jammer.h"

void app_main(void) {
    // Khởi tạo các thành phần
    oled_init();
    nrf24_init_all();
    gpio_set_direction(LED_ONBOARD_PIN, GPIO_MODE_OUTPUT);

    while (1) {
        // Bật LED báo trạng thái
        gpio_set_level(LED_ONBOARD_PIN, 1);

        uint32_t jam_start = esp_log_timestamp();
        while (esp_log_timestamp() - jam_start < JAM_TIME_MS) {
            float vbat = read_battery_voltage();
            uint32_t elapsed = esp_log_timestamp() - jam_start;

            // Jam lệch pha
            nrf24_jam_lech_pha(elapsed);

            // OLED chỉ hiển thị thời gian JAM đang chạy và điện áp pin
            oled_show(vbat, elapsed);

            vTaskDelay(pdMS_TO_TICKS(50));
        }

        // Nghỉ, tắt LED
        gpio_set_level(LED_ONBOARD_PIN, 0);
        oled_show(read_battery_voltage(), 0);
        vTaskDelay(pdMS_TO_TICKS(REST_TIME_MS));
    }
}