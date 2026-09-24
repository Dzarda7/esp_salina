idf_component_register(SRCS "main.c"
                            "net.c"
                            "idsjmk.c"
                            "battery.c"
                            "ui.c"
                            "config.c"
                            "portal.c"
                            "dns.c"
                            "fonts/font_salina_14.c"
                            "fonts/font_salina_bold_18.c"
                       INCLUDE_DIRS "."
                       EMBED_TXTFILES "portal.html"
                       REQUIRES esp_epaper esp_wifi esp_netif esp_http_client
                                esp-tls nvs_flash esp_adc esp_lcd esp_driver_spi esp_timer esp_driver_gpio
                                esp_http_server)
