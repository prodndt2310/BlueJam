#pragma once
#include <stdbool.h>
#include <stdint.h>  // thêm để dùng uint32_t

void oled_init();
void oled_show(float battery_v, uint32_t jam_time_ms);