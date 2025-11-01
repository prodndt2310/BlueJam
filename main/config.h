#pragma once

// #define SIMULATION_MODE 1 
// ===================== SPI & NRF24L01 =====================

// ----------- FSPI bus (dùng cho NRF1 và NRF2) -------------
#define FSPI_SCK    12
#define FSPI_MOSI   13
#define FSPI_MISO   14

#define NRF1_CS     15
#define NRF1_CE     16

#define NRF2_CS     17
#define NRF2_CE     18

// ----------- HSPI bus (dùng cho NRF3 và NRF4) -------------
#define HSPI_SCK    36
#define HSPI_MOSI   37
#define HSPI_MISO   38

#define NRF3_CS     39
#define NRF3_CE     40

#define NRF4_CS     41
#define NRF4_CE     42

// ===================== OLED I2C ==========================
#define OLED_SDA    21
#define OLED_SCL    19   

// ================== LED ONBOARD & ADC BATTERY =============
#define LED_ONBOARD_PIN   48   
#define BATTERY_ADC_PIN   3    

// ================== JAMMER THAM SỐ ========================
#define JAM_TIME_MS       3000
#define REST_TIME_MS      1000
