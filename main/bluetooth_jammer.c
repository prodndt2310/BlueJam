#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "config.h"
#include "battery.h"
#include "oled.h"
#include "nrf24_jammer.h"

#ifdef SIMULATION_MODE
// Nếu bạn có test_sim.h, có thể include thay vì extern
extern void start_simulation_task(void);
#endif

static const char *TAG = "APP_MAIN";

void app_main(void)
{
    ESP_LOGI(TAG, "========== APP KHỞI ĐỘNG ==========");

    // 1) Khởi tạo OLED (nếu có)
    ESP_LOGI(TAG, "Init OLED...");
    oled_init();

    // 2) Khởi tạo nRF driver / logic (thực hoặc mock tuỳ cấu hình)
    ESP_LOGI(TAG, "Init nRF modules/logic...");
    nrf24_init_all();

    // 3) Khởi tạo LED trạng thái
    gpio_set_direction(LED_ONBOARD_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_ONBOARD_PIN, 1);

    // 4) Nếu bật chế độ mô phỏng, start task mô phỏng
    #ifdef SIMULATION_MODE
    ESP_LOGI(TAG, "SIMULATION_MODE enabled -> starting simulation task");
    start_simulation_task();
    #endif

    // 5) Vòng lặp chính (giữ chương trình chạy, có thể hiển thị status)
    while (1) {
        float vbat = read_battery_voltage();
        // Hiển thị điện áp và không truyền thời gian jam (task mô phỏng xuất log riêng)
        oled_show(vbat, 0);

        // Log nhẹ để quan sát trạng thái
        ESP_LOGD(TAG, "VBAT=%.2f V", vbat);

        // Giảm tải CPU, nhường cho các task khác (simulation task, mock, v.v.)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Không nên chạy tới đây
    gpio_set_level(LED_ONBOARD_PIN, 0);
}