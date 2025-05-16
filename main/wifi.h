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
#include "esp_netif.h"
#include "esp_mac.h"


//#define WIFI_APSTA
//#define WIFI_STA
#define WIFI_AP

// 确保只有一个模式被定义
#if defined(WIFI_APSTA) && (defined(WIFI_STA) || defined(WIFI_AP))
#error "Only one of WIFI_APSTA, WIFI_STA, or WIFI_AP can be defined at a time."
#elif defined(WIFI_STA) && defined(WIFI_AP)
#error "Only one of WIFI_APSTA, WIFI_STA, or WIFI_AP can be defined at a time."
#endif

/* WiFi 连接配置（需用户修改） */
#define WIFI_SSID       "Autolife"      // 连接的 WiFi 名称
#define WIFI_PASSWORD   "13929215819"   // WiFi 密码

//#define WIFI_SSID       "SteinsXin"  //修改为自己的wifi名称
//#define WIFI_PASSWORD   "123456789"  //修改为自己的wifi密码

/* WiFi AP 配置 */
#define AP_SSID         "Autolife"      // AP模式下的SSID
#define AP_PASSWORD     ""    // AP模式下的密码（至少8字符）
#define AP_CHANNEL      6               // AP信道
#define AP_MAX_CONN     4               // 最大连接数

/**
 * @brief 初始化 WiFi 硬件和驱动
 */
void wifi_init(void);

/**
 * @brief 启动 WiFi STA 连接
 */
void wifi_sta_start(void);

/**
 * @brief 停止 WiFi STA 连接
 */
void wifi_sta_stop(void);

/**
 * @brief 启动 WiFi AP
 */
void wifi_ap_start(void);

/**
 * @brief 停止 WiFi AP
 */
void wifi_ap_stop(void);

#endif //MAIN_WIFI_H
