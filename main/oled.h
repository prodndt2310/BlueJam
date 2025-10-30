#pragma once
#include <stdbool.h>
void oled_init();
void oled_show(float battery_v, uint32_t jam_time_ms);