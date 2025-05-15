/**
 * @file ntp_time.c
 * @author Steins_Xin
 * @brief NTP 时间同步实现
 * @version 1.0
 * @date 2025-05-11
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "ntp_time.h"
#include <string.h>
#include "esp_log.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "ntp_time";

/**
 * @brief 初始化 SNTP 服务
 */
void start_sntp(void) {
    ESP_LOGI(TAG, "Initializing SNTP...");

    setenv("TZ", "CST-8", 1); // 设置为东八区
    tzset();

    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_setservername(1, "time.windows.com");
    esp_sntp_setservername(2, "time.apple.com");
    esp_sntp_init();

    // 等待同步完成
    while (esp_sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET) {
        ESP_LOGI(TAG, "Waiting for time sync...");
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    esp_sntp_stop();
    ESP_LOGI(TAG, "SNTP sync complete.");
}

/**
 * @brief 获取当前本地时间
 * @return struct tm 当前时间（本地时区）
 */
struct tm get_current_time(void) {
    time_t now;
    struct tm timeinfo;

    time(&now);
    localtime_r(&now, &timeinfo);

    return timeinfo;
}