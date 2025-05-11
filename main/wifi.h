//
// Created by Steins_Xin on 25-5-11.
//

#ifndef MAIN_WIFI_H
#define MAIN_WIFI_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

#define WIFI_SSID       "SteinsXin"  //修改为自己的wifi名称
#define WIFI_PASSWORD   "123456789"  //修改为自己的wifi密码

void wifi_init(void);
void wifi_start(void);
void wifi_stop(void);

#endif //MAIN_WIFI_H
