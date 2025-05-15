/**
 * @file wifi.h
 * @author Steins_Xin
 * @brief WiFi 连接管理模块头文件
 * @version 1.0
 * @date 2024-05-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MAIN_WIFI_H
#define MAIN_WIFI_H

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

/* WiFi 连接配置（需用户修改） */
#define WIFI_SSID       "Autolife"      // 连接的 WiFi 名称
#define WIFI_PASSWORD   "13929215819"   // WiFi 密码

//#define WIFI_SSID       "SteinsXin"  //修改为自己的wifi名称
//#define WIFI_PASSWORD   "123456789"  //修改为自己的wifi密码

/**
 * @brief 初始化 WiFi 硬件和驱动
 */
void wifi_init(void);

/**
 * @brief 启动 WiFi 连接
 */
void wifi_start(void);

/**
 * @brief 停止 WiFi 连接
 */
void wifi_stop(void);

#endif //MAIN_WIFI_H
