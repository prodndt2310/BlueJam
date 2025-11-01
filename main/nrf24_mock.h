#pragma once
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t writes;
    uint32_t set_channel_calls;
    uint32_t last_channel;
    uint64_t total_write_time_us;
} nrf24_mock_stats_t;

void nrf24_mock_init(const char* name);
void nrf24_mock_set_channel(uint8_t ch);
void nrf24_mock_write(const uint8_t *buf, size_t len);
const nrf24_mock_stats_t* nrf24_mock_get_stats(void);
void nrf24_mock_reset_stats(void);