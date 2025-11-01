#include "battery.h"
#include "driver/adc.h"
#include "esp_log.h"

#define VBAT_DIVIDER_RATIO 3.0f

float read_battery_voltage() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11); // chỉnh lại theo chân ADC
    int raw = adc1_get_raw(ADC1_CHANNEL_3);
    float v = ((float)raw / 4095.0f) * 3.6f * VBAT_DIVIDER_RATIO;
    return v;
}