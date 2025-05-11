//
// Created by Steins_Xin on 25-5-11.
//
#include <stdio.h>
#include "ntp_time.h"
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include <sys/time.h>
#include "esp_sntp.h"
static const char *TAG = "ntp";
static struct tm timeinfo;
static void esp_initialize_sntp(void)
{
    sntp_setoperatingmode(SNTP_OPMODE_POLL);    // 设置单播模式
    sntp_setservername(0, "cn.ntp.org.cn");	    // 设置访问服务器
    sntp_setservername(1, "ntp1.aliyun.com");
    sntp_setservername(2, "210.72.145.44");     // 国家授时中心服务器 IP 地址
    setenv("TZ", "CST-8", 1);                   //东八区
    sntp_init();                                //初始化
}

/**
 * @brief 启动sntp获取网络时间
 * @param 无
 * @return 无
 * @note tm_mon: 从0开始            tm_year: 距离1900年的差值，默认是70
 *       tm_yday: 一年的过去的天数   tm_isdst: 是否为夏时制
 */
void sntp_task(void *param)
{
    time_t now;
    esp_initialize_sntp();
    // 延时等待SNTP初始化完成
    do {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    } while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET);

    // 成功获取网络时间后停止NTP请求，不然设备重启后会造成获取网络时间失败的现象
    // 大概是服务器时根据心跳时间来删除客户端的，如果不是stop结束的客户端，下次连接服务器时就会出错
    esp_sntp_stop();
    while(1)
    {
        time(&now);                         // 获取网络时间, 64bit的秒计数
        tzset();                            // 更新本地C库时间
        localtime_r(&now, &timeinfo);       // 转换成具体的时间参数
        ESP_LOGI(TAG, "Now Time:");
        ESP_LOGI(TAG, "%4d-%02d-%02d %02d:%02d:%02d week:%d", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1,
                 timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_wday);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
struct tm* get_time(void)
{
    return &timeinfo;
}
