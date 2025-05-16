/**
 * @file wifi.c
 * @author Steins_Xin
 * @brief WiFi 连接管理实现
 * @version 1.0
 * @date 2024-05-11
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <string.h>
#include "wifi.h"

static const char *TAG = "wifi";                // 日志标签
uint16_t wifi_ip_addr[4];                       // 存储获取到的 IP 地址
bool wifi_ip_got_state = false;                 // IP 获取状态标志
static esp_netif_t* sta_netif = NULL;           // STA网络接口
static esp_netif_t* ap_netif = NULL;            // AP网络接口

/**
 * @brief WiFi STA 事件处理回调函数
 *
 * @param arg 用户自定义参数（未使用）
 * @param event_base 事件基类型（WIFI_EVENT / IP_EVENT）
 * @param event_id 具体事件ID
 * @param event_data 事件数据
 */
static void wifi_sta_event_handler(void *arg, esp_event_base_t event_base,
                                   int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "WiFi STA started");
                break;

            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "WiFi connected to AP");
                wifi_ip_got_state = false;      // 连接成功但未获取IP
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGI(TAG, "WiFi disconnected, reconnecting...");
                wifi_ip_got_state = false;
                esp_wifi_connect();             // 自动重连
                break;
        }
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;

        // 解析并存储IP地址
        wifi_ip_addr[0] = esp_ip4_addr1_16(&event->ip_info.ip);
        wifi_ip_addr[1] = esp_ip4_addr2_16(&event->ip_info.ip);
        wifi_ip_addr[2] = esp_ip4_addr3_16(&event->ip_info.ip);
        wifi_ip_addr[3] = esp_ip4_addr4_16(&event->ip_info.ip);
        wifi_ip_got_state = true;               // 标记已获取IP

        ESP_LOGI(TAG, "Got IP: %d.%d.%d.%d",
                 wifi_ip_addr[0], wifi_ip_addr[1],
                 wifi_ip_addr[2], wifi_ip_addr[3]);
    }
}

/**
 * @brief WiFi AP 事件处理回调函数
 *
 * @param arg 用户自定义参数（未使用）
 * @param event_base 事件基类型（WIFI_EVENT）
 * @param event_id 具体事件ID
 * @param event_data 事件数据
 */
static void wifi_ap_event_handler(void *arg, esp_event_base_t event_base,
                                  int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_AP_START:
                ESP_LOGI(TAG, "WiFi AP started");
                break;

            case WIFI_EVENT_AP_STOP:
                ESP_LOGI(TAG, "WiFi AP stopped");
                break;

            case WIFI_EVENT_AP_STACONNECTED:
            {
                wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
                ESP_LOGI(TAG, "Station connected, MAC: %02x:%02x:%02x:%02x:%02x:%02x, AID=%d",
                         event->mac[0], event->mac[1], event->mac[2],
                         event->mac[3], event->mac[4], event->mac[5],
                         event->aid);
            }
                break;

            case WIFI_EVENT_AP_STADISCONNECTED:
            {
                wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
                ESP_LOGI(TAG, "Station disconnected, MAC: %02x:%02x:%02x:%02x:%02x:%02x, AID=%d",
                         event->mac[0], event->mac[1], event->mac[2],
                         event->mac[3], event->mac[4], event->mac[5],
                         event->aid);
            }
                break;
        }
    }
}

/**
 * @brief 初始化 WiFi 模块
 *
 * 1. 初始化 NVS 存储
 * 2. 创建网络接口和事件循环
 * 3. 配置 WiFi
 */
void wifi_init(void) {
    // 初始化 NVS（非易失性存储）
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ESP_ERROR_CHECK(nvs_flash_init());
    }

    // 初始化网络栈和默认事件循环
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 创建STA和AP网络接口
    sta_netif = esp_netif_create_default_wifi_sta();
    ap_netif = esp_netif_create_default_wifi_ap();

    // WiFi 初始化配置
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 注册STA事件处理器
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
            WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_sta_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
            IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_sta_event_handler, NULL, NULL));

    // 注册AP事件处理器
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
            WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_ap_event_handler, NULL, NULL));
}

/**
 * @brief 启动 WiFi STA 连接
 */
void wifi_sta_start(void) {
    // 配置 WiFi STA 连接参数
    wifi_config_t wifi_config = {
            .sta = {
                    .ssid = WIFI_SSID,
                    .password = WIFI_PASSWORD,
                    .scan_method = WIFI_FAST_SCAN,
                    .sort_method = WIFI_CONNECT_AP_BY_SECURITY,
                    .threshold.rssi = -127,      // 最低信号强度阈值
                    .threshold.authmode = WIFI_AUTH_OPEN
            },
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}

/**
 * @brief 停止 WiFi STA 连接
 */
void wifi_sta_stop(void) {
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    ESP_ERROR_CHECK(esp_wifi_stop());
}

/**
 * @brief 启动 WiFi AP
 */
void wifi_ap_start(void) {
    // 配置 WiFi AP 参数
    wifi_config_t ap_config = {
            .ap = {
                    .ssid = AP_SSID,
                    .password = AP_PASSWORD,
                    .ssid_len = strlen(AP_SSID),
                    .channel = AP_CHANNEL,
                    .authmode = WIFI_AUTH_WPA2_PSK,
                    .max_connection = AP_MAX_CONN,
                    .pmf_cfg = {
                            .required = true,
                    },
            },
    };

    if (strlen(AP_PASSWORD) == 0) {
        ap_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    // 提供自定义MAC地址 CC:D8:43:3C:4C:B3 | 50-88-11-ae-77-25
    uint8_t custom_mac[] = {0xCC, 0xD8, 0x43, 0x3C, 0x4C, 0xB3};
    ESP_ERROR_CHECK(esp_wifi_set_mac(WIFI_IF_AP, custom_mac));

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

/**
 * @brief 停止 WiFi AP
 */
void wifi_ap_stop(void) {
    ESP_ERROR_CHECK(esp_wifi_stop());
}
