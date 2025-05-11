#include <stdio.h>
#include <esp_log.h>
#include "esp_rom_sys.h"
void app_main(void)
{
    while(1){
        ESP_LOGI("main","hhh");
        esp_rom_delay_us(1000*1000);
    }

}

