#include "nrf24_jammer.h"
#include "config.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define JAM_PACKET_SIZE 32

// Danh sách kênh của PA (jam mạnh nhất)
static const uint8_t pa_channels[] = {0, 7, 14, 21, 28, 35, 37, 38, 39, 40, 41, 49, 56, 63, 70, 77, 78};
#define PA_CH_COUNT (sizeof(pa_channels)/sizeof(pa_channels[0]))

// Danh sách kênh còn lại cho 3 module thường
static uint8_t non_pa_channels[79 - PA_CH_COUNT];
static int non_pa_count = 0;

// Lệch pha: mỗi module random seed khác nhau, offset index khác nhau
static uint32_t jammer_phase[4] = {0, 100, 200, 300}; // ms lệch nhau

void nrf24_init_all() {
    // TODO: Khởi tạo 4 module nRF24 với thông số pin đã cấu hình
    // Gợi ý: mỗi module 1 SPI, hoặc dùng cùng SPI nhưng CS riêng
    // Dùng thư viện RF24 nếu có, nếu không sẽ phải port lại
    // ...
    // Tạo danh sách non_pa_channels[]
    int idx = 0;
    for (uint8_t ch = 0; ch < 79; ++ch) {
        bool is_pa = false;
        for (int j = 0; j < PA_CH_COUNT; ++j) {
            if (ch == pa_channels[j]) {
                is_pa = true;
                break;
            }
        }
        if (!is_pa) non_pa_channels[idx++] = ch;
    }
    non_pa_count = idx;
}

void nrf24_jam_lech_pha(uint32_t elapsed_ms) {
    uint8_t noise[JAM_PACKET_SIZE];
    for (int i = 0; i < JAM_PACKET_SIZE; ++i) noise[i] = esp_random() % 256;

    // Module PA: đánh lần lượt các kênh trong pa_channels, lệch pha
    static int pa_idx = 0;
    if (elapsed_ms % 4 == 0) {
        // Chỉ jam khi đến lượt pha này
        uint8_t ch = pa_channels[pa_idx];
        // TODO: setChannel PA, ghi noise
        // nrf24_pa_set_channel(ch);
        // nrf24_pa_write(noise, JAM_PACKET_SIZE);
        pa_idx = (pa_idx + 1) % PA_CH_COUNT;
    }

    // 3 module thường: random trong non_pa_channels, lệch pha
    for (int m = 0; m < 3; ++m) {
        if (elapsed_ms % 4 == m+1) { // lệch pha cho từng module
            int rand_idx = (esp_random() + m * 17) % non_pa_count;
            uint8_t ch = non_pa_channels[rand_idx];
            // TODO: setChannel module m, ghi noise
            // nrf24_m_set_channel(ch);
            // nrf24_m_write(noise, JAM_PACKET_SIZE);
        }
    }
}