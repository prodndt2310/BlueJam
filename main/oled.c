#include "oled.h"
#include "ssd1306.h"
#include <stdio.h>

void oled_init() {
    ssd1306_i2c_init(OLED_SDA_PIN, OLED_SCL_PIN, SSD1306_I2C_ADDRESS);
    ssd1306_clear_screen();
}

void oled_show(float battery_v, uint32_t jam_time_ms) {
    char line1[20], line2[20];
    snprintf(line1, sizeof(line1), "Pin: %.2f V", battery_v);
    snprintf(line2, sizeof(line2), "Jam: %lu ms", (unsigned long)jam_time_ms);

    ssd1306_clear_screen();
    ssd1306_draw_string(0, 0, line1, 12, 1);
    ssd1306_draw_string(0, 16, line2, 12, 1);
    ssd1306_refresh_gram();
}