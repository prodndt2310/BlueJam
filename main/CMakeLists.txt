idf_component_register(
    SRCS
        "bluetooth_jammer.c"
        "battery.c"
        "oled.c"
        "nrf24_jammer.c"

        
    INCLUDE_DIRS "."
    REQUIRES ssd1306
)