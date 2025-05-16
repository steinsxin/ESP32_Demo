/**
 * @file main.c
 * @author Steins_Xin
 * @brief 主程序入口文件
 * @version 1.0
 * @date 2025-05-15
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>
#include <errno.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include <string.h>
#include <stdlib.h>

#include "wifi.h"
#include "ntp_time.h"
#include "udp_send.h"
#include "tcp_send.h"

/**
 * @brief 主程序入口
 */
void app_main(void) {
    // 初始化 WiFi 并连接
    wifi_init();

#ifdef WIFI_APSTA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    wifi_sta_start();
    wifi_ap_start();
#elifdef WIFI_STA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    wifi_sta_start();
#elifdef WIFI_AP
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    wifi_ap_start();
#endif

    ESP_LOGI("app_main", "WiFi initialized and started");

    // 等待 1 秒，确保 WiFi 已连接
    vTaskDelay(pdMS_TO_TICKS(1000));

    // 初始化并同步时间
    start_sntp();
    ESP_LOGI("app_main", "NTP time synchronization complete");

    // 初始化 TCP 连接
    if (tcp_send_init("192.168.31.60", 1034) == 0) {
        ESP_LOGI("app_main", "TCP connection initialized successfully");
    } else {
        ESP_LOGE("app_main", "Failed to initialize TCP connection");
    }

    // 主循环
    while (1) {
        // 获取当前时间
        struct tm now = get_current_time();
        ESP_LOGI("app_main", "Current time: %04d-%02d-%02d %02d:%02d:%02d",
                 now.tm_year + 1900, now.tm_mon + 1, now.tm_mday,
                 now.tm_hour, now.tm_min, now.tm_sec);

        // 发送 TCP 数据
        if (tcp_send("Hello SteinsXin TCP") < 0) {
            ESP_LOGE("app_main", "Failed to send TCP message");
        }

        // 等待 1 秒
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}