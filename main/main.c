#include <stdio.h>
#include <errno.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <errno.h>
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include <string.h>
#include <stdlib.h>
#include "wifi.h"
#include "ntp_time.h"

void app_main(void)
{
    int cnt = 0;
    wifi_init();
    wifi_start();
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    xTaskCreatePinnedToCore(sntp_task, "sntp_task", 2048, NULL, 1, NULL, 0);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    //wifi_stop();
    while (1)
    {
        //led_prog();
        printf("cnt: %d\n", cnt);
        //ledc_prog();
        //key_prog();
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
